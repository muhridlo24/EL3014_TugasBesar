#include <Arduino.h>

int COM=14,counter=0;
int clock[6] = {0,0,0,0,0,0};
int state_menit_detik = 0;
int digit1=2;
int digit2=3;
int digit3=4;
int digit4=5;
int button1=9;
int button2=10;
int button3=11;
int button4=12;
String state="jam digital quo";
String prev_state;
int segmen_kiri=0;


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
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);


  Serial.begin(9600);

  //TIMER 2 (15,625 kHz dan prescaler 1024)
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A |= B00000010; //Set Register untuk Timer 2 Prescaler 1024
  TCCR2B |= B10000111;
  OCR2A=25;
  TIMSK2 |= (1 << OCIE2A); //Set register untuk mengaktifkan COMPARE
  
  //TIMER 1 (100 Hz dan prescaler 256)
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= B00000100;//Set Register untuk Timer 1 Prescaler 256
  OCR1A=625;
  TIMSK1 |= (1 << OCIE1A);
  sei();

  Serial.print("OCR2A: "); 
  Serial.println(OCR2A, HEX);
  Serial.print("TCCR2A: "); 
  Serial.println(TCCR2A, HEX);
  Serial.print("TCCR2B: ");
  Serial.println(TCCR2B, HEX);
  Serial.print("TIMSK2: "); 
  Serial.println(TIMSK2, HEX);
  Serial.println("TIMER2 Setup Finished.");
}

//fungsi Multiplexer 1 digit sevent segment untuk nentuin segment mana aja yg nyala
void mux(int clock_1){
    if (clock_1==0){
      digitalWrite(2,LOW);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,HIGH);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);
      digitalWrite(8,LOW);
    }      
    else if (clock_1==1){
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
      digitalWrite(4,LOW);
      digitalWrite(5,HIGH);
      digitalWrite(6,HIGH);
      digitalWrite(7,HIGH);
      digitalWrite(8,LOW);
    }      
    else if (clock_1==2){
      digitalWrite(2,LOW);
      digitalWrite(3,LOW);
      digitalWrite(4,HIGH);
      digitalWrite(5,LOW);
      digitalWrite(6,HIGH);
      digitalWrite(7,LOW);
      digitalWrite(8,LOW);
    }
    else if (clock_1==3){
      digitalWrite(2,HIGH);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,HIGH);
      digitalWrite(7,LOW);
      digitalWrite(8,LOW);
    }
    else if (clock_1==4){
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(7,HIGH);
      digitalWrite(8,LOW);
    }
    else if (clock_1==5){
      digitalWrite(2,HIGH);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);
      digitalWrite(8,HIGH);
    }
    else if (clock_1==6){
      digitalWrite(2,LOW);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);
      digitalWrite(8,HIGH);
    }
    else if (clock_1==7){
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
      digitalWrite(4,LOW);
      digitalWrite(5,HIGH);
      digitalWrite(6,HIGH);
      digitalWrite(7,LOW);
      digitalWrite(8,LOW);
    }
    else if (clock_1==8){
      digitalWrite(2,LOW);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);
      digitalWrite(8,LOW);
    }
    else if (clock_1==9){
      digitalWrite(2,HIGH);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);
      digitalWrite(8,LOW);
    }
}

//Fungsi penampilan Jam, yang nanti dioper ke fungsi mux()
void display_clock(){
  if (COM==14){
    digitalWrite(14,HIGH);
    digitalWrite(15,LOW);
    digitalWrite(16,LOW);
    digitalWrite(17,LOW);
    mux(clock[digit4]);
  }
  else if (COM==15){
    digitalWrite(15,HIGH);
    digitalWrite(14,LOW);
    digitalWrite(16,LOW);
    digitalWrite(17,LOW);
    mux(clock[digit3]);
  }
  else if (COM==16){
    digitalWrite(16,HIGH);
    digitalWrite(17,LOW);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);
    mux(clock[digit2]);
  }
  else if (COM==17){
    digitalWrite(17,HIGH);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);
    digitalWrite(16,LOW);
    mux(clock[digit1]);
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

// Fungsi Interrupt setiap 10ms
ISR(TIMER2_COMPA_vect){
  //Fungsi Ganti digit jam
  if (COM==14){
    COM = 15;
  }
  else if (COM==15){
    COM = 16;
  }
  else if (COM==16){
    COM = 17;
  }
  else if (COM==17){
    COM = 14;
  }
  display_clock();
  counter++;
  if (counter%2==0){
    if (digitalRead(9)==LOW){
      state_menit_detik = !state_menit_detik;
    }
  }
  if (counter==625 && state=="jam digital quo"){
    setting_clock();
    counter=0;
  }
}

ISR(TIMER1_COMPA_vect){
  if (state=="jam digital quo"){
    Serial.println(state);
    prev_state="jam digital quo";
    if (digitalRead(button1)==LOW){ //tekan button 1
      state="menit detik";
    }
    if (digitalRead(button2)==LOW){
      state="set jam digital";
    }
  }
  if (state=="set jam digital"){
    Serial.println(state);
    prev_state="set jam digital";
    if (digitalRead(button1)==LOW){ //tekan button 1
      state="jam digital quo";
    }
    if (digitalRead(button2)==LOW){ //tekan button 2
      state="switch seven segment";
    }
    if (digitalRead(button3)==LOW){ //tekan button 3
      state="penjumlahan";
    }
    if (digitalRead(button4)==LOW){ //tekan button 4
      state="pengurangan";
    } 
  }
  if (state=="switch seven segment"){
    Serial.println(state);
    state=prev_state;
    prev_state="switch seven segment";
    segmen_kiri = !segmen_kiri;
  }
  if (state=="penjumlahan"){
    Serial.println(state);
    state=prev_state;
    prev_state="penjumlahan";
    if (segmen_kiri){
      clock[1]++;
    }
    else{
      clock[3]++;
    }
  }
  if (state=="pengurangan"){
    Serial.println(state);
    state=prev_state;
    prev_state="pengurangan";
    if (segmen_kiri){
      clock[1]--;
    }
    else{
      clock[3]--;
    }
  }
  if (state=="menit detik"){
    Serial.println(state);
    state=prev_state;
    prev_state="menit detik";
  }
}



void loop(){

}
