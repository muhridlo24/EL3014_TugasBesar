#include <Arduino.h>

int COM,counter;
int state_jam_temp;
int clock[6] = {0,0,0,0,0,0};

/*
  PIN:
  - 13,12,11,10 menunjukkan PIN COM1, COM2, COM3, COM4
  - 3 : segment C
  - 4 : semgent D
  - 5 : segment E
  - 6 : segment B
  - 7 : segment A
  - 8 : segment F
  - 9 : segment G

  - 2 : buzzer

  - A0 : button 1
  - A1 : button 2
  - A2 : button 3
  - A3 : button 4
*/


void setup(){
  for (int i=2;i<10;i++){
    pinMode(i,INPUT_PULLUP);
  }

  for (int i=10;i<=13;i++){
    pinMode(i,OUTPUT);
  }

  digitalWrite(2,HIGH);
  Serial.begin(9600);


  //TImer yang digunakan: TIMER 2 dengan Prescaler 1024
  cli();
  //Inisasi agar isi register 0
  TCCR2A = 0;
  TCCR2B = 0;

  //Set Register untuk Timer 2 Prescaler 1024
  TCCR2B |= B00000101;
    //Set register untuk mengaktifkan COMPARE
  TIMSK1 |= B00000010;
  OCR1A=625;

  //Set Register untuk Timer 10ms Seven Segment
  TCCR2B |= B00000000;

  sei();
}

void loop(){
  if (state_jam_temp==1){
    if (COM==13){
      COM = 12;
    }
    else if (COM==12){
      COM = 11;
    }
    else if (COM==11){
      COM = 10;
    }
    else if (COM==10){
      COM = 13;
    }

    display_clock();
    counter++;

    if (counter==100){
      setting_clock();
      counter = 0;
    }
    
    state_jam_temp=0;
  }
}

//Fungsi Interrupt setiap 10ms
ISR(TIMER2_COMPA_vect){
  state_jam = 1;
  TCNT1 = 0; 
}

//Fungsi penampilan Jam, yang nanti dioper ke fungsi mux()
void display_clock(){
  digitalWrite(COM,HIGH);

  if (COM==13){
    mux(clock[0]);
  }

  else if (COM==12){
    mux(clock[1]);
  }
  else if (COM==11){
    mux(clock[2]);
  }
  else if (COM==10){
    mux(clock[3]);
  }
}

//fungsi Multiplexer 1 digit sevent segment untuk nentuin segment mana aja yg nyala
void mux(int clock_1){
    switch (clock_1){
      case 0:
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
      
      case 1:
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
      
      case 2:
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
      
      case 3:
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
      
      case 4:
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
      
      case 5:
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
      
      case 6:
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);

      case 7:
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);

      case 8:
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);

      case 9:
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
    }
}

//Fungsi penambahan Clock
void setting_clock(){
  //jika jam menunjukkan 23:59:59
  if(clock[0]==2 && clock[1]==3 && clock[2]==5 && clock[3]==9 && clock[4]==5 && clock[5]==9){
    clock[0] = 0;
    clock[1] = 0;
    clock[2] = 0;
    clock[3] = 0;
    clock[4] = 0;
    clock[5] = 0;
  }

  //Jika jam menunjukkan XX:59:59
  else if(clock[2]==5 && clock[3]==9 && clock[4]==5 && clock[5]==9){
    //Jika jam X9:59:59
    if (clock[1]==9){
      clock[0]++;
      clock[1] = 0;
    }
    //jika jam XX:59:59
    else{
      clock[1]++;
    }
    clock[2] = 0;
    clock[3] = 0;
    clock[4] = 0;
    clock[5] = 0;
  }

  //Jika jam XX:XX:59
  else if(clock[4]==5 && clock[5]==9){
    //Jika jam XX:X9:59
    if (clock[3]==9){
      clock[2]++;
      clock[3] = 0;
    }
    //Jika jam XX:XX:59
    else{
      clock[3]++;
    }  
    clock[4] = 0;
    clock[5] = 0;
  }

  
  else{
    //Jika Jam XX:XX:X9
    if (clock[5]==9){
      clock[4]++;
      clock[5] = 0;
    }
    //Detik bertambah
    else{
      clock[5]++;
    }
  }
}

