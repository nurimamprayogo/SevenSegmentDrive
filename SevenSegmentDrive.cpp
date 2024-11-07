#include "SevenSegmentDrive.h"

SevenSegmentDrive::SevenSegmentDrive(int _dinPin, int _enPin, int _numSegment)
  : pixels(_numSegment * 3, _dinPin, NEO_RGB + NEO_KHZ800), enPin(_enPin){
  mode=MPWM;
  numSegment=_numSegment;
}

SevenSegmentDrive::SevenSegmentDrive(int _dinPin, int _enPin, int _setPin, int _clkPin, int _numSegment)
  : dinPin(_dinPin), enPin(_enPin), setPin(_setPin), clkPin(_clkPin) {
	mode=SRC;
	numSegment=_numSegment;
}

void SevenSegmentDrive::begin() {
  if (mode == MPWM) {
    pixels.begin();
	pinMode(enPin, OUTPUT);
	digitalWrite(enPin, HIGH);
	
	for(uint8_t x=0;x<numSegment;x++){displayNeo(0,x);}
  } else {
    pinMode(dinPin, OUTPUT);
    
    pinMode(setPin, OUTPUT);
    pinMode(clkPin, OUTPUT);
	digitalWrite(enPin, HIGH);
	pinMode(enPin, OUTPUT);
	digitalWrite(setPin, HIGH);
	for(uint8_t x=0;x<numSegment;x++){sendByte(0);}
  }
  show();
  setBright(200);
}

void SevenSegmentDrive::sendByte(unsigned char _data) {
  shiftOut(dinPin, clkPin, LSBFIRST, _data);
}

void SevenSegmentDrive::show() {
  if (mode == MPWM)pixels.show();
  else{
	  digitalWrite(setPin, LOW);
	  delayMicroseconds(1);
	  digitalWrite(setPin, HIGH);
  }
}

void SevenSegmentDrive::setBright(unsigned char brightness) {
  if (brightness < 0) brightness = 0;
  else if (brightness > 255) brightness = 255;
  if(mode==SRC)analogWrite(enPin, 255 - brightness);
  else neoBright=brightness;
}

void SevenSegmentDrive::displayNeo(uint16_t digit, uint8_t position) {
  pixels.setPixelColor( position * 3,     pixels.Color(((digit>>8)&1)*neoBright, ((digit>>0)&1)*neoBright, ((digit>>1)&1)*neoBright)); 
  pixels.setPixelColor((position * 3)+ 1, pixels.Color(((digit>>2)&1)*neoBright, ((digit>>3)&1)*neoBright, ((digit>>4)&1)*neoBright)); 
  pixels.setPixelColor((position * 3)+ 2, pixels.Color(((digit>>5)&1)*neoBright, ((digit>>6)&1)*neoBright, ((digit>>7)&1)*neoBright)); 

}

void SevenSegmentDrive::displayNumber(float num,unsigned char decimal) {
  char buffer[10];
  dtostrf(num, 1, decimal, buffer); 
  displayString(buffer);
}

void SevenSegmentDrive::displayNumber(int num){
  char buffer[10];
  itoa(num, buffer, 10); 
  displayString(buffer);
}

void SevenSegmentDrive::displayString(const char* str) {
  uint8_t position = 0;
  uint16_t digit = 0;  
  int length = strlen(str);
  
  char buffer[length];
  
  while (*str){
	digit = 0;   
    if (*str >= '0' && *str <= '9') {
	  digit = sevenSegmentDigits[*str - '0'];
	  }
	if (*str >= 'a' && *str <= 'z') {
	  digit = sevenSegmentAlphabet[*str - 32 - 'A'];
	  }
	if (*str >= 'A' && *str <= 'Z') {
	  digit = sevenSegmentAlphabet[*str - 'A'];
	  }
	if (*str == '-') {
	  digit = 0b00000010;
	  }
	if (*str == '_') {
	  digit = 0b00010000;
	  }
	if (*str == ' ') {
	  digit = 0b00000000;
	  }
	  
	if(*str != '\0'){
		if (mode == MPWM) {
		  if (*(str+1) == '.' || *(str+1) == ':') {
			if (*(str+1) == '.')digit |= 0b00000001;  
			if (*(str+1) == ':')digit |= 0b100000000;  
			str++;
		  }
		}
		else {
		  if (*(str+1) == '.' || *(str+1) == ':') {
			  digit |= 0b00000001;  
			  str++;
		  }
		}	
	}
	buffer[position]=digit;
	str++;
	if (mode == MPWM)displayNeo(digit, position); 
	
	position++;
  }
  if(mode==SRC){
	  for(int x=length;x>0;x--){
		sendByte(buffer[x-1]);
	  }
  }
}