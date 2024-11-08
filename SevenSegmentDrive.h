#ifndef SevenSegmentDrive_h
#define SevenSegmentDrive_h

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define SRC  0
#define MPWM 1

class SevenSegmentDrive {
  public:
    // Constructor untuk WS2811
    SevenSegmentDrive(int dinPin, int enPin, int setPin, int clkPin, int numSegment);
	
	SevenSegmentDrive(int dinPin, int enPin, int numSegment);

    // Fungsi untuk mengatur mode (SRC atau MPWM)
    void begin();

    // Fungsi untuk menampilkan angka float
    void displayNumber(float num,unsigned char decimal);
	void displayNumber(int num);

    // Fungsi untuk menampilkan string
    void displayString(const char* str);

    // Fungsi untuk mengatur brightness
    void setBright(unsigned char brightness);

	void show();
void sendByte(unsigned char _data);

  private:
    // Pin
    int dinPin, enPin, setPin, clkPin, numSegment;
	bool mode;
	unsigned char neoBright=128;
	
    // Fungsi untuk mengirim byte menggunakan shiftOut (untuk mode SRC)
    // Fungsi untuk mengatur output (untuk mode SRC)
    

    // Fungsi untuk menampilkan digit pada NeoPixel (untuk mode MPWM)
    void displayNeo(uint16_t digit, uint8_t position);

    // WS2811 (NeoPixel) related
    Adafruit_NeoPixel pixels;

    // Tabel heksadesimal untuk setiap digit (0-9)
    const uint8_t sevenSegmentDigits[10] = {
      0b11111100,  // 0 -> a,b,c,e,f,g
      0b01100000,  // 1 -> b,c
      0b11011010,  // 2 -> a,b,g,e,d
      0b11110010,  // 3 -> a,b,c,d,g
      0b01100110,  // 4 -> b,c,f,g
      0b10110110,  // 5 -> a,f,g,c,d
      0b10111110,  // 6 -> a,f,g,e,d,c
      0b11100000,  // 7 -> a,b,c
      0b11111110,  // 8 -> a,b,c,d,e,f,g
      0b11110110   // 9 -> a,b,c,d,f,g,d
    };
	
	// a, b, c, d, e, f, g -> 0b(gfedcba)
	const uint8_t sevenSegmentAlphabet[26] = {
	  // a, b, c, d, e, f, g, dp
	  0b11101110, // A
	  0b00111110, // B
	  0b10011100, // C
	  0b01111010, // D
	  0b10011110, // E
	  0b10001110, // F
	  0b10111110, // G
	  0b01101110, // H
	  0b01100000, // I
	  0b01110000, // J
	  0b10101110, // K (Note: limited by 7-segment)
	  0b00011100, // L
	  0b10101000, // M (limited by 7-segment)
	  0b11101100, // N (limited by 7-segment)
	  0b11111100, // O
	  0b11001110, // P
	  0b11100110, // Q
	  0b00001010, // R
	  0b10110110, // S
	  0b00011110, // T
	  0b01111100, // U
	  0b00111000, // V (same as U due to 7-segment)
	  0b00000000, // W (limited by 7-segment)
	  0b01101110, // X
	  0b01110110, // Y
	  0b11011010  // Z
	};

};

#endif
