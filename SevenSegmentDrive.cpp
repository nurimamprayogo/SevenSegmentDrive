#include "SevenSegmentDrive.h"

SevenSegmentDrive::SevenSegmentDrive(int ledPin, int numSegment){
  pixels = new Adafruit_NeoPixel(numSegment*3, ledPin, NEO_RGB + NEO_KHZ800);
  mode=MPWM;
}

SevenSegmentDrive::SevenSegmentDrive(int _dinPin, int _enPin, int _setPin, int _clkPin)
  : dinPin(_dinPin), enPin(_enPin), setPin(_setPin), clkPin(_clkPin) {
	mode=SRC;
}

void SevenSegmentDrive::begin() {
  if (mode == MPWM) {
    pixels->begin();
  } else {
    pinMode(dinPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    pinMode(setPin, OUTPUT);
    pinMode(clkPin, OUTPUT);
	digitalWrite(enPin, LOW);
	digitalWrite(setPin, HIGH);
  }
  segBright(200);
}

void SevenSegmentDrive::sendByte(unsigned char _data) {
  shiftOut(dinPin, clkPin, LSBFIRST, _data);
}

void SevenSegmentDrive::setOut() {
  if (mode == MPWM)pixels->show();
  else{
	  digitalWrite(setPin, LOW);
	  delayMicroseconds(1);
	  digitalWrite(setPin, HIGH);
  }
}

void SevenSegmentDrive::segBright(unsigned char brightness) {
  if (brightness < 0) brightness = 0;
  else if (brightness > 255) brightness = 255;
  if(mode=SRC)analogWrite(enPin, 255 - brightness);
  else neoBright=brightness;
}

void SevenSegmentDrive::displayNeo(uint16_t digit, uint8_t position) {
  //uint8_t segments = sevenSegmentDigits[digit];
  
  pixels->setPixelColor( position * 3,     pixels->Color(((digit>>8)&1)*neoBright, ((digit>>0)&1)*neoBright, ((digit>>1)&1)*neoBright)); 
  pixels->setPixelColor((position * 3)+ 1, pixels->Color(((digit>>2)&1)*neoBright, ((digit>>3)&1)*neoBright, ((digit>>4)&1)*neoBright)); 
  pixels->setPixelColor((position * 3)+ 2, pixels->Color(((digit>>5)&1)*neoBright, ((digit>>6)&1)*neoBright, ((digit>>7)&1)*neoBright)); 
  
  // Mengatur segmen c,b,a pada WS2811(3)
  /*
  for (int i = 0; i < 3; i++) {
    if (segments & (1 << (i + 4))) {
      pixels->setPixelColor(position * 3 + 6 + i, pixels->Color(255, 0, 0)); 
    } else {
      pixels->setPixelColor(position * 3 + 6 + i, pixels->Color(0, 0, 0)); 
    }
  }

  // Mengatur segmen f,e,d pada WS2811(2)
  for (int i = 0; i < 3; i++) {
    if (segments & (1 << (i + 1))) {
      pixels->setPixelColor(position * 3 + 3 + i, pixels->Color(255, 0, 0)); 
    } else {
      pixels->setPixelColor(position * 3 + 3 + i, pixels->Color(0, 0, 0)); 
    }
  }

  // Mengatur segmen ":", ".", g pada WS2811(1)
  if (segments & 0b00000001) {
    pixels->setPixelColor(2, pixels->Color(255, 0, 0)); 
  } else {
    pixels->setPixelColor(2, pixels->Color(0, 0, 0)); 
  }

  pixels->show();
  */
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
	  
	if(*str != '\0'){
		if (mode == MPWM) {
		  if (*str == '.' || *str == ':') {
			if (*str == '.')digit |= 0b00000001;  
			if (*str == ':')digit |= 0b100000000;  
			str++;
		  }
		}
		else {
		  if (*str == '.' || *str == ':') {
			  digit |= 0b00000001;  
			  str++;
		  }
		}	
	}
	str++;
	if (mode == MPWM)displayNeo(digit, position); 
	else sendByte(digit);
	
	position++;
  }
}

/*
void displayString(const char* str) {
  while (*str) {
    if (*str >= '0' && *str <= '9') {
      uint8_t digit = sevenSegmentDigits[*str - '0'];
      str++;  // Lanjutkan ke karakter berikutnya

      // Jika karakter berikutnya adalah titik, aktifkan segmen dp
      if (*str == '.') {
        digit |= 0b10000000;  // Aktifkan dp (bit ke-7)
        str++;  // Lanjutkan ke karakter setelah titik
      }

      // Kirim byte digit ke display
      sendByte(digit);
      setOut();
    }
  }
}
*/