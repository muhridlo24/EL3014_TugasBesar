#include <Arduino.h>

int state=1,jam,menit,detik;

void setup(){
    pinMode(3,INPUT_PULLUP);
    pinMode(4,INPUT_PULLUP);
    pinMode(5,INPUT_PULLUP);
    pinMode(6,INPUT_PULLUP);
    pinMode(7,INPUT_PULLUP);
    pinMode(8,INPUT_PULLUP);
    pinMode(9,INPUT_PULLUP);
    pinMode(10,INPUT_PULLUP);

    digitalWrite(2,HIGH);

    Serial.begin(9600);
    //Diketahui Clock pada Arduino adalah 16MHz
    //Dapat dgunakan Overflow, CTC.

    cli();
    //Inisasi agar isi register 0
    TCCR1A = 0;
    TCCR1B = 0;

    //Set Register untuk Timer 1 sekon

    TCCR1B |= B00000100;
    TIMSK1 |= B00000010;
    OCR1A=62500;

    //Set Register untuk Timer 40ms Seven Segment
    TCCR2B |= B00000000;

    sei();
    //Untuk menghasilkan 1 sekon, digunakan Prescaler sebesar [].
}

void loop(){
  display_clock();
}

//Fungsi Interrupt saat
ISR(TIMER1_COMPA_vect){
  setting_clock(jam,menit,detik);
  TCNT1 = 0;
  state=!(state);
}

void display_clock(){

}

void setting_clock(int jam, int menit, int detik){
  if(jam==23 && menit==59 && detik==59){
    jam=0;
    menit=0;
    detik=0;
  }
  else if(menit==59 && detik==59){
    jam+=1;
    menit=0;
    detik=0;
  }
  else if(detik==59){
    menit+=1;
    detik=0;
  }
  else{
    detik+=1;
  }
}
