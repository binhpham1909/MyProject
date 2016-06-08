#include <sensor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// Init global variables
String _dv_ip="",_dv_gateway="",_dv_mask="";
String inputString = "";         // a string to hold incoming data
String respone ="";

// flag
boolean _stringComplete = false;  // whether the string is complete
boolean _sesion=false; // restore over serial
boolean _login=false; // restore over serial
// DDNS server
const unsigned long __period_ddns=5*60000;  // 5' up ip/times to get dhcp ip of modem
String __ddns_host_ = "192.168.1.2";
String __ddns_link = "/index.php";
String _ddns_receive="";
String save_result="";
// Restore
unsigned long _restore_time=0;


ESPHB esp;
configs rom;
defaults _factory;

// Create wifi server
WiFiServer server(80);

void setup() {
  Serial.begin(115200); // Open serial communications and wait for port to open:
  while (!Serial) {}// wait for serial port to connect.
  esp.init_store(512);  // Open EEPROM with size 512 byte 
  pinMode(0, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  WiFi.mode(WIFI_STA);

  _factory.serial="A10000";
  _factory.key="12345";
  _factory.admin="6789";
  _factory.apssid="A10000";
  _factory.appassword="12345678";
// Restore if press GPIO 0 >5s and press reset
/*
  _restore_time=millis();
  while(digitalRead(0)==LOW){
    _current_time=millis();
    if((_current_time-_restore_time)>=5000){
      esp.restore();
      ESP.restart();
    };
  };
*/
  esp.debug("\n Wellcome to wifi switch control.");
  esp.debug(ESP.getChipId());
  esp.debug("Read configs.");
  esp.read_configs(&rom,&_factory);
  esp.debug("Read Complete.");
  esp.debug("SSID: "+rom.ssid);
  esp.debug("password: "+rom.password);
  esp.IPtoString(rom.ip,&_dv_ip);
  esp.IPtoString(rom.gateway,&_dv_gateway);
  esp.IPtoString(rom.mask,&_dv_mask);
  esp.debug("Device IP: "+_dv_ip);
  esp.debug("Gateway: "+_dv_gateway);
  esp.debug("Mask: "+_dv_mask);  
  if(esp.wifi_connect()){esp.wifi_APconnect(&_factory);} // init wifi
  server.begin();    // Start the server
  esp.debug("Server started");
  }

void loop() {
    serialEvent();
    WiFiClient client = server.available(); // Check if a client has connected
    if (!client) {
      esp.DynDNS(__ddns_host_,__ddns_link,__period_ddns);
// process main until client receive data
      return; // return at here is code bellow not run and re loop() when if condition true
    }
// Wait until the client sends some data
    esp.debug("New client connecting..");
    while (!client.available()) { 
      delay(1); 
    }
    String req = client.readStringUntil('\r');  // Read the request
    client.flush();
    esp.debug("Incomming requests:");
    esp.debug(req);
//First build the  response header
    respone="";
    request _get;
    esp.getDecode(&_get,req);
    esp.debug("IP to control: ");
    esp.debug(_get.ip);
    if(_get.ip==_dv_ip){  // check ip, if ip==device ip then process request else forward request to this ip
      process(&_get);
      esp.debug(&respone);
      client.print(respone); 
    }
    else{
      String receive_forward="";
      forward(_get.ip,&req,&receive_forward);
      esp.debug(&receive_forward);
      client.print(receive_forward);  
    };
    client.stop();
    req="";
}
void serialEvent() {
  while (Serial.available()){
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {_stringComplete = true;}   
    if(_stringComplete){
      if(inputString=="help\n"){
        Serial.println("Type 'login'  : for login to system.");
        Serial.println("Type 'status' : for view device status.");
        Serial.println("Type 'debug'  : for debug mode.");
        Serial.println("Type 'normal' : for nomarl mode, debug will disable.");
        Serial.println("Type 'reboot' : for reboot device.");
        Serial.println("Type 'restore': for resrore to factory seting.");
        }
      if(inputString=="login\n"){Serial.println("Admin password need for restore?");_sesion=true;}
      if((inputString==(rom.admin+"\n"))&&_sesion){
        Serial.println("You had login to admin system.");
        _login=true;
        _sesion=false;
        }
      if((inputString=="reboot\n")&&_login){Serial.println(_label_reboot);ESP.restart();}
      if(inputString=="status\n"&&_login){
        Serial.print(_label_serial+": ");Serial.println(rom.serial);
        Serial.print(_label_ssid+": ");Serial.println(rom.ssid);
        Serial.print(_label_password+": ");Serial.println(rom.password);
        Serial.print(_label_key+": ");Serial.println(rom.key);
        Serial.print("Save IP: ");Serial.println(_dv_ip);
        Serial.print("Now IP: ");Serial.println(WiFi.localIP());
        Serial.print("Save gateway: ");Serial.println(_dv_gateway);
        Serial.print("Now gateway: ");Serial.println(WiFi.gatewayIP());
        Serial.print("Save subnet mask: ");Serial.println(_dv_mask);
        Serial.print("Now subnet mask: ");Serial.println(WiFi.subnetMask());
      }
      if((inputString==("restore\n"))&&_login){
        _login=false;
        Serial.println("Restoring...");
        esp.restore();
        ESP.restart();
      }
      if((inputString==("debug\n"))&&_login){
        Serial.println("Enable debug mode");
        esp.set_debug(true);
        ESP.restart();
        }
      if((inputString==("normal\n"))&&_login){
        Serial.println("Disable debug mode");
        esp.set_debug(false);
        ESP.restart();
        }
      inputString="";
      _stringComplete = false;
      }
  }
}
void process(request * _get){
  if(_get->key==rom.key){
    if(_get->command=="status"){
      // Add status content at here
      esp.jsonEncode(ONEJSON,&respone,_label_status,_label_online);
    }
  else if(_get->command=="control"){
    String _tmp_key="",_tmp_val="";
    esp.toggle_pin(_get->pin,_get->value,&_tmp_key,&_tmp_val);
    esp.jsonEncode(FIRSTJSON,&respone,_label_result,_label_complete);
    esp.jsonEncode(LASTJSON,&respone,_tmp_key,_tmp_val);
  }
  else if((_get->command=="wifi")&&(_get->ssid!="")){
    String _tmp_ip="";
    if(esp.wifi_connect(_get->ssid,_get->password, &_tmp_ip,&save_result)){
      esp.jsonEncode(FIRSTJSON,&respone,_label_result,save_result);
      esp.jsonEncode(NEXTJSON,&respone,_label_ssid,_get->ssid);
      esp.jsonEncode(LASTJSON,&respone,_label_password,_get->password);
    }else{
      esp.jsonEncode(FIRSTJSON,&respone,_label_result,save_result);
      esp.jsonEncode(NEXTJSON,&respone,_label_ip,_tmp_ip);
      esp.jsonEncode(NEXTJSON,&respone,_label_ssid,_get->ssid);
      esp.jsonEncode(LASTJSON,&respone,_label_password,_get->password);
    }
  }
  else if((_get->command=="key")&&(_get->newkey!="")){
    if(esp.save(__key,_get->newkey,&save_result)){
      esp.jsonEncode(FIRSTJSON,&respone,_label_result,_label_complete);
      esp.jsonEncode(LASTJSON,&respone,_label_key,save_result);
    }else {
      esp.jsonEncode(FIRSTJSON,&respone,_label_result,_label_error);
      esp.jsonEncode(LASTJSON,&respone,_label_key,_get->newkey);
    };
  }
  else if((_get->command=="ip")&&(_get->newip!="")){
    save_result="";
    if(esp.save_ip(_get->newip,&save_result)){
      esp.jsonEncode(FIRSTJSON,&respone,_label_result,_label_complete);
      esp.jsonEncode(LASTJSON,&respone,_label_ip,save_result);
    }else{
     esp.jsonEncode(FIRSTJSON,&respone,_label_result,_label_complete);
    esp.jsonEncode(LASTJSON,&respone,_label_ip,_get->newip);     
    }
  }
  else if(_get->command=="reboot"){
    esp.jsonEncode(ONEJSON,&respone,_label_result,_label_reboot);
    ESP.restart();
  }
  else if(_get->command=="restore"){
    esp.restore();
    ESP.restart();
  };
  }
  else if(_get->command=="serial"){
    esp.jsonEncode(ONEJSON,&respone,_label_serial,rom.serial);
  }        
}
void forward(String _address,String * _send,String *_receive){  
  char ip_forward[_address.length()+1];
  _address.toCharArray(ip_forward,_address.length()+1);
  WiFiClient client2;
  if (!client2.connect(ip_forward, httpPort)) {
    esp.debug(_label_conn_fail);
    return;
  }
  esp.debug(_label_conn_ok);
  esp.debug(*_send);
  client2.print(*_send);
  while(client2.available()){
    *_receive = client2.readString();
  }
  client2.stop();    
}


