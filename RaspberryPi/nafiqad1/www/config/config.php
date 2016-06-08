<?php
$db_host = "localhost"; // Giữ mặc định là localhost
$db_name    = 'hbiot';// Can thay doi
$db_username    = 'root'; //Can thay doi
$db_password    = '!()(!(*(';//Can thay doi
$conn=mysql_connect("{$db_host}", "{$db_username}", "{$db_password}") or die("Không thể kết nối database");
mysql_select_db("{$db_name}") or die("Không thể chọn database");
mysql_set_charset('utf8',$conn);
$locahost = "../pkn/";
$tb_prefix="pkn_";

function RenameDevice($_id,$_newname){	// valid content
	$sql="UPDATE device SET name = '{$_newname}' WHERE id = '{$_id}'";
	return mysql_query($sql)or die(mysql_error());
}
function GetDeviceName($__id){
	$trave="";
	$sql="SELECT name FROM device WHERE id = '{$__id}'";
		$results = mysql_query($sql)or die(mysql_error());
		while ($row = mysql_fetch_assoc($results)){$trave=$row['name'];}
	return $trave;
}
function GetDeviceSerial($__id){
	$trave="";
	$sql="SELECT serial FROM device WHERE id = '{$__id}'";
		$results = mysql_query($sql)or die(mysql_error());
		while ($row = mysql_fetch_assoc($results)){$trave=$row['serial'];}
	return $trave;
}
function GetDeviceNameFromId($__id){
	$trave="";
	$sql="SELECT name FROM device WHERE id = '{$__id}'";
	$results = mysql_query($sql)or die(mysql_error());
	while ($row = mysql_fetch_assoc($results)){$trave=$row['name'];}
	return $trave;
}
function GetDeviceIdFromSerial($__serial){
	$trave="";
	$sql="SELECT id FROM device WHERE serial = '{$__serial}'";
	$results = mysql_query($sql)or die(mysql_error());
	while ($row = mysql_fetch_assoc($results)){$trave=$row['id'];}
	return $trave;
}
function CheckExistDeviceSerial($__serial){
	$sql="SELECT id FROM device WHERE serial = '{$__serial}'";
	$results = mysql_query($sql)or die(mysql_error());
	if(mysql_num_rows($results)>0) 
		return TRUE; 
	else 
		return FALSE;
}
function NewDevice($_serial,$_name){
	$sql="INSERT INTO device(serial,name) VALUE ('{$_serial}','{$_name}')";
	return mysql_query($sql)or die(mysql_error());
}
function NewHumiSensor($_deviceid){
	$sql="INSERT INTO sensor_humi(device,alert) VALUE ({$_deviceid},1)";
	return mysql_query($sql)or die(mysql_error());
}
function NewTempSensor($_deviceid){
	$sql="INSERT INTO sensor_temp(device,alert) VALUE ({$_deviceid},1)";
	return mysql_query($sql)or die(mysql_error());
}

function UpdateHumiValue($_serial,$_value){	// valid content
	$_deviceid=GetDeviceIdFromSerial($_serial);
	$sql="UPDATE sensor_humi SET humi = '{$_value}' WHERE device = '{$_deviceid}'";
	return mysql_query($sql);
}
function UpdateHumiRange($_id,$_min,$_max){	// valid content
	$sql="UPDATE sensor_humi SET min_humi = '{$_min}',max_humi = '{$_max}' WHERE id = '{$_id}'";
	return mysql_query($sql);
}
function CheckHumiInRange($_serial,$_value){	// valid content
	$_deviceid=GetDeviceIdFromSerial($_serial);
	$results = mysql_query("SELECT * FROM sensor_humi WHERE device = '{$_deviceid}' AND (min_humi>'{$_value}' OR max_humi<'{$_value}') AND alert = 1");
	if(mysql_num_rows($results)>0) 
		return TRUE; 
	else 
		return FALSE;
}

function UpdateTempValue($_serial,$_value){	// valid content
	$_deviceid=GetDeviceIdFromSerial($_serial);
	$sql="UPDATE sensor_temp SET temp = '{$_value}' WHERE device = '{$_deviceid}'";
	return mysql_query($sql);
}
function UpdateTempRange($_id,$_min,$_max){	// valid content
	$sql="UPDATE sensor_temp SET min_temp = '{$_min}',max_temp = '{$_max}' WHERE id = '{$_id}'";
	return mysql_query($sql);
}
function CheckTempInRange($_serial,$_value){	// valid content
	$_deviceid=GetDeviceIdFromSerial($_serial);
	$results = mysql_query("SELECT * FROM sensor_temp WHERE device = '{$_deviceid}' AND (min_temp>'{$_value}' OR max_temp<'{$_value}') AND alert = 1");
	if(mysql_num_rows($results)>0) 
		return TRUE; 
	else 
		return FALSE;
}

function ReadConfig(){
	$trave=array();
	$sql="SELECT * FROM setting";
	$results = mysql_query($sql)or die(mysql_error());
	while ($row = mysql_fetch_assoc($results)){
		$trave[$row['name']]=$row['value'];
	}
	return $trave;
}
function ReadConfigByName($_name){
	$trave;
	$sql="SELECT value FROM setting WHERE name = '{$_name}'";
	$results = mysql_query($sql)or die(mysql_error());
	while ($row = mysql_fetch_assoc($results)){
		if($row['value']=='1'){$trave=TRUE;}else{$trave=FALSE;};
	}
	return $trave;
}
function SetConfig($_key,$_value){
	$sql="UPDATE setting SET value = {$_value} WHERE name = '{$_key}'";
	return mysql_query($sql)or die(mysql_error());
}


function ReadUserReceiveSms(){
	$trave=array();
	$sql="SELECT * FROM receive_sms";
	$results = mysql_query($sql)or die(mysql_error());
	while ($row = mysql_fetch_assoc($results)){
		array_push($trave,array('id'=>$row['id'],'name'=>$row['name'],'phone'=>$row['phone']));
	}
	return $trave;
}
function EditUserReceiveSms($_id,$_newname,$_newphone){
	$sql="UPDATE receive_sms SET name = '{$_newname}',phone= '{$_newphone}' WHERE id = '{$_id}'";
	return mysql_query($sql)or die(mysql_error());
}
function AddUserReceiveSms($_newname,$_newphone){
	$sql="INSERT INTO receive_sms(name,phone) VALUE ('{$_newname}','{$_newphone}')";
	return mysql_query($sql)or die(mysql_error());
}
function DelUserReceiveSms($_id){
	$sql="DELETE FROM receive_sms WHERE id = '{$_id}'";
	return mysql_query($sql)or die(mysql_error());
}

function ReadUserReceiveEmail(){
	$trave=array();
	$sql="SELECT * FROM receive_email";
	$results = mysql_query($sql)or die(mysql_error());
	while ($row = mysql_fetch_assoc($results)){
		array_push($trave,array('id'=>$row['id'],'name'=>$row['name'],'email'=>$row['email']));
	}
	return $trave;
}
function EditUserReceiveEmail($_id,$_newname,$_newemail){
	$sql="UPDATE receive_email SET name = '{$_newname}',email= '{$_newemail}' WHERE id = '{$_id}'";
	return mysql_query($sql)or die(mysql_error());
}
function AddUserReceiveEmail($_newname,$_newemail){
	$sql="INSERT INTO receive_email(name,email) VALUE ('{$_newname}','{$_newemail}')";
	return mysql_query($sql)or die(mysql_error());
}
function DelUserReceiveEmail($_id){
	$sql="DELETE FROM receive_email WHERE id = '{$_id}'";
	return mysql_query($sql)or die(mysql_error());
}
// Convert range warning to string
function RangeTempToString($_serial){
	$_deviceid=GetDeviceIdFromSerial($_serial);
	$sql_query = mysql_query("SELECT min_temp,max_temp FROM sensor_temp WHERE device='{$_deviceid}'");
    $row = mysql_fetch_array( $sql_query );
	return "\nNhiet do MIN: ".$row['min_temp']."\nNhiet do MAX: ".$row['max_temp'];	
}
function RangeHumiToString($_serial){
	$_deviceid=GetDeviceIdFromSerial($_serial);
	$sql_query = mysql_query("SELECT min_humi,max_humi FROM sensor_humi WHERE device='{$_deviceid}'");
    $row = mysql_fetch_array( $sql_query );
	return "\nDo am MIN: ".$row['min_humi']."\nDo am MAX: ".$row['max_humi'];
}
// Update warning
function UpdateWarningLog($_device,$_content){
	$time = date('Y-m-d H:i:s');
	$sql="INSERT INTO warning_log(device,time,content) VALUE ('{$_device}','{$time}','{$_content}')";
	return mysql_query($sql)or die(mysql_error());
}
function stripslashes_deep($value)
{
	$value = is_array($value) ?array_map('stripslashes_deep', $value):stripslashes($value);
    return $value;
}
?>