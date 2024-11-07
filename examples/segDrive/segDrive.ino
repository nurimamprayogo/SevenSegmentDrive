void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

int dinPin, enPin, setPin, clkPin;

#define SRC  0
#define MPWM 1
void SegDrive(bool _mode, int _din, int _en, int _set, int _clk){
  pinMode(_din,OUTPUT);
  pinMode(_en,OUTPUT);
  pinMode(_set,OUTPUT);
  pinMode(_clk,OUTPUT);
  digitalWrite(_en,HIGH);
  digitalWrite(_set,HIGH);
  dinPin=_din;
  enPin=_en;
  setPin=_set;
  clkPin=_clk;
}

void sendByte(unsigned char _data){
  shiftOut(dinPin, clkPin, LSBFIRST, _data);
}

void setOut(){
  digitalWrite(setPin,LOW);
  delayMicroseconds(1);
  digitalWrite(setPin,HIGH);
}

void segBright(unsigned char _dat){
  if(_dat<0)_dat=0;
  else if(_dat>255)_dat=255;
  analogWrite(enPin, 255-_dat);
}

