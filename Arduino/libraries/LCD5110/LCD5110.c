/*
This Code has extra features 
including a XY positioning function on Display
and a Line Draw function on Nokia 3310 LCD 
It is modded from the original 
http://playground.arduino.cc/Code/PCD8544
*/
int a = 0;



void LCD5110::SetPIN(char SCE, char RESET, char DC, char SDIN, char SCLK){
	PIN_SCE = SCE;
	PIN_RESET = RESET;
	PIN_DC = DC;
	PIN_SDIN = SDIN;
	PIN_SCLK = SCLK;
	pinMode(PIN_SCE,   OUTPUT);
	pinMode(PIN_RESET, OUTPUT);
	pinMode(PIN_DC,    OUTPUT);
	pinMode(PIN_SDIN,  OUTPUT);
	pinMode(PIN_SCLK,  OUTPUT);
}

void LCD5110::LcdInit(void){
	digitalWrite(PIN_RESET, LOW);
	// delay(1);
	digitalWrite(PIN_RESET, HIGH);
	LcdWrite( LCD_CMD, 0x21 );  // LCD Extended Commands.
	LcdWrite( LCD_CMD, 0xBf );  // Set LCD Vop (Contrast). //B1
	LcdWrite( LCD_CMD, 0x04 );  // Set Temp coefficent. //0x04
	LcdWrite( LCD_CMD, 0x14 );  // LCD bias mode 1:48. //0x13
	LcdWrite( LCD_CMD, 0x0C );  // LCD in normal mode. 0x0d for inverse
	LcdWrite(LCD_C, 0x20);
	LcdWrite(LCD_C, 0x0C);
}

void LCD5110::LcdWrite(byte dc, byte data){
	digitalWrite(PIN_DC, dc);
	digitalWrite(PIN_SCE, LOW);
	shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
	digitalWrite(PIN_SCE, HIGH);
}

void LCD5110::Print(char character){
	LcdWrite(LCD_D, 0x00);
	for(int index = 0; index < 5; index++){
		LcdWrite(LCD_D, ASCII[character - 0x20][index]);
	}
	LcdWrite(LCD_D, 0x00);
}

void LCD5110::Print(char *characters){
	while (*characters){
		LcdCharacter(*characters++);
	}
}
void LCD5110::LcdClear(void){
	for (int index = 0; index < LCD_X * LCD_Y / 8; index++){
		LcdWrite(LCD_D, 0x00);
	}
}


// gotoXY routine to position cursor . x - range: 0 to 84 : y - range: 0 to 5

void LCD5110::gotoXY(int x, int y){
	LcdWrite( 0, 0x80 | x);  // Column.
	LcdWrite( 0, 0x40 | y);  // Row.  
}



void drawLine(void)
{
  unsigned char  j;  
   for(j=0; j<84; j++) // top
	{
          gotoXY (j,0);
	  LcdWrite (1,0x01);
  } 	
  for(j=0; j<84; j++) //Bottom
	{
          gotoXY (j,5);
	  LcdWrite (1,0x80);
  } 	

  for(j=0; j<6; j++) // Right
	{
          gotoXY (83,j);
	  LcdWrite (1,0xff);
  } 	
	for(j=0; j<6; j++) // Left
	{
          gotoXY (0,j);
	  LcdWrite (1,0xff);
  }

}


void setup(void)
{

 LcdInitialise();
  LcdClear();

}

void loop(void)
{
  // Display some simple character animation
  //
  int a,b;
  char Str[15];
  // Draw a Box
  for(b=1000; b>0; b--){
  drawLine();
  for(a=0; a<=5 ; a++){
  gotoXY(4,1);
  // Put text in Box
  LcdString ("TestDisplay");
  gotoXY(24,3);
  LcdCharacter('H');
  LcdCharacter('E');
  LcdCharacter('L');
  LcdCharacter('L');
  LcdCharacter('O');
  LcdCharacter(' ');
  LcdCharacter('=');
  // Draw + at this position
  gotoXY(10,3);
  LcdCharacter('=');
  delay(500);
  gotoXY(24,3);
  LcdCharacter('h');
  LcdCharacter('e');
  LcdCharacter('l');
  LcdCharacter('l');
  LcdCharacter('o');
  LcdCharacter(' ');
  LcdCharacter('-');
  // Draw - at this position
  gotoXY(10,3);
  LcdCharacter('-');
  delay(500);
  }
  }
}