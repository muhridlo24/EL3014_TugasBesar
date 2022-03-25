#include <Arduino.h>

int COM=14,counter=0;
int clock[6] = {0,0,0,0,0,0};
int state_menit_detik = 0;
int digit1;
int digit2;
int digit3;
int digit4;
int button1=9;
int button2=10;
int button3=11;
int button4=12;
int state_ganti_clock = 0;
int state_ganti_digit = 0;
int state_switch_seven_segment = 0;
String prev_state;
int segmen_kiri=0;
unsigned long myTime;

int state_quo=1;
int state_set_jam=0;
int state_penjumlahan=0;
int state_pengurangan=0;


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
  Serial.begin(115200);

  //TIMER 2 (15,625 kHz dan prescaler 1024)
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A |= B00000010; //Set Register untuk Timer 2 Prescaler 1024
  TCCR2B |= B10000111;
  OCR2A=25;
  TIMSK2 |= (1 << OCIE2A); //Set register untuk mengaktifkan COMPARE
  
  sei();

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

void set_jam(){
  //digit yg diganti adalah digit ke 2 (jam satuan)
  if (state_switch_seven_segment==0){
    if (state_penjumlahan==1){
      if (clock[0]==2 && clock[1]==4){
        clock[0]=0;
        clock[1]=0;
      }
      else if(clock[1]==9){
        clock[0]++;
        clock[1]=0;
      }
      else{
        clock[1]++;
      }
      state_penjumlahan=0;
    }
    else if(state_pengurangan==1){
      if (clock[0]==0 && clock[1]==0){
        clock[0]=2;
        clock[1]=4;
      }
      else if(clock[1]==0){
        clock[0]--;
        clock[1]=9;
      }
      else{
        clock[1]--;
      }
      
      state_pengurangan=0;
    }
  }
  //digit yg diganti adalah digit ke 4
  else{
    if (state_penjumlahan==1){
      if (clock[2]==5 && clock[3]==9){
        if(clock[0]==2 && clock[1]==4){
          clock[0]=0;
          clock[1]=0;
          clock[2]=0;
          clock[3]=0;
        }else if(clock[1]==9){
          clock[0]++;
          clock[1]=0;
          clock[2]=0;
          clock[3]=0;
          
        }
        else{
          clock[1]++;
          clock[2]=0;
          clock[3]=0;
        }
      }
      else if(clock[3]==9){
        clock[2]++;
        clock[3]=0;
      }
      else{
        clock[3]++;
      }
      state_penjumlahan=0;
    }
    else if(state_pengurangan==1){
      if (clock[2]==0 && clock[3]==0){
        if (clock[0]==0 && clock[1]==0){
          clock[0]=2;
          clock[1]=3;
          clock[2]=5;
          clock[3]=9;
        }
        else if(clock[1]==0){
          clock[0]--;
          clock[1]=9;    
          clock[2]=5;
          clock[3]=9;
        }
        else{
          clock[1]--;
          clock[2]=5;
          clock[3]=9;
        }
      }
      else if(clock[3]==0){
        clock[2]--;
        clock[3]=9;
      }
      else{
        clock[3]--;
      }
      state_pengurangan=0;
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
  state_ganti_digit = 1;

  counter++;

  //FSM
  if (counter%200==0){
    if (state_quo==1){
      if (digitalRead(button2)==LOW){
        state_set_jam=1;
        state_quo=0;
      }
      else if (digitalRead(button1)==LOW){
        state_menit_detik = !state_menit_detik;
      }
    }
    else if (state_set_jam==1){
      if (digitalRead(button1)==LOW){ //tekan button 1
        state_quo=1;
        state_set_jam=0;
        state_switch_seven_segment=0;
      }
      else if (digitalRead(button2)==LOW){ //tekan button 2
        state_switch_seven_segment =!state_switch_seven_segment;
      }
      else if (digitalRead(button3)==LOW){ //tekan button 3
        state_penjumlahan=1;
      }
      else if (digitalRead(button4)==LOW){ //tekan button 4
        state_pengurangan=1;
      }
      state_menit_detik=0;
    }
  }

  if (counter==625){
    if (state_quo==1 && state_set_jam==0){
      state_ganti_clock = 1;
    }
    counter=0;
  }
}

void loop(){
  if (state_ganti_digit==1){
    display_clock();
    state_ganti_digit = 0;
  }
  
  if(state_menit_detik==0){
    digit1=0;
    digit2=1;
    digit3=2;
    digit4=3;
  } else{
    digit1=2;
    digit2=3;
    digit3=4;
    digit4=5;
  }
  
  if (state_set_jam==1){
    set_jam();
  }

  else if (state_quo==1){
    if (state_ganti_clock==1){
      setting_clock();
      state_ganti_clock = 0;
    }
  }
  Serial.print("Quo: ");
  Serial.print(state_quo);
  Serial.print("  Set Jam: ");
  Serial.print(state_set_jam);
  Serial.print("switch seven segment: ");
  Serial.print(state_switch_seven_segment);
  Serial.print("  penjumlahan: ");
  Serial.print(state_penjumlahan);
  Serial.print("  pengurangan: ");
  Serial.println(state_pengurangan);
}
