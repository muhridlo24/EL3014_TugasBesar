#include <Arduino.h>

int COM=14,counter=0;
int clock[6] = {0,0,0,0,0,0};
int alarm[6] = {0,0,0,0};
int clock_stopwatch[4] = {0,0,0,0};
int state_menit_detik = 0;
int digit1;
int digit2;
int digit3;
int digit4;
int button1=9;
int button2=10;
int button3=11;
int button4=12;
int buzzer=13;
int state_ganti_clock = 0;
int state_ganti_digit = 0;
int state_switch_seven_segment = 0;
String prev_state;
int segmen_kiri=0;
unsigned long myTime;
int temp_alarm=1;
int state_start_pause=0;
int state_quo=1;
int state_set_jam=0;
int state_penjumlahan=0;
int state_pengurangan=0;
int state_stopwatch=0;
int state_alarm=0;
int state_alarm_on=0;
int counter_alarm=0;
int state_quo_stopwatch = 0;
int state_set_stopwatch = 0;

/*
  PIN:
  - A0,A1,A2,A3 menunjukkan PIN COM1, COM2, COM3, COM4
  - 2 : segment C
  - 3 : semgent D
  - 4 : segment E
  - 5 : segment B
  - 6 : segment A
  - 7 : segment F
  - 8 : segment G
  - 9 : button 1
  - 10 : button 2
  - 11 : button 3
  - 12 : button 4
  - 13 : buzzer
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

  //STOPWATCH 2 (15,625 kHz dan prescaler 1024)
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A |= B00000010; //Set Register untuk Stopwatch 2 Prescaler 1024
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
void display_clock(int clock[]){
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
void setting_clock(int clock[]){
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

void setting_stopwatch(){
  if (clock_stopwatch[2]==5 && clock_stopwatch[3]==9){
    if(clock_stopwatch[0]==9 && clock_stopwatch[1]==9){
      clock_stopwatch[0]=9;
      clock_stopwatch[1]=9;
      clock_stopwatch[2]=5;
      clock_stopwatch[3]=9;
    }
    else if(clock[1]==9){
      clock_stopwatch[0]++;
      clock_stopwatch[1]=0;
      clock_stopwatch[2]=0;
      clock_stopwatch[3]=0;
      
    }
    else{
      clock_stopwatch[1]++;
      clock_stopwatch[2]=0;
      clock_stopwatch[3]=0;
    }
  }
  else if(clock_stopwatch[3]==9){
    clock_stopwatch[2]++;
    clock_stopwatch[3]=0;
  }
  else{
    clock_stopwatch[3]++;
  }
}

void set_jam(int clock[]){
  //digit yg diganti adalah digit ke 2 (jam satuan)
  if (state_switch_seven_segment==0){
    if (state_penjumlahan==1){
      if (clock[0]==2 && clock[1]==3){
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
        clock[1]=3;
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
        if(clock[0]==2 && clock[1]==3){
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

void alarm_on(){
  if(counter_alarm%125==0 && counter_alarm!=0){
    temp_alarm=!temp_alarm;
    digitalWrite(buzzer,temp_alarm);
  }
    
  if (counter_alarm==15625){
    digitalWrite(buzzer,LOW);
    counter_alarm=0;
    state_alarm_on = 0;
    alarm[0]=0;
    alarm[1]=0;
    alarm[2]=0;
    alarm[3]=0;
  }
}

/*
FSM:
state_ganti_digit = menyatakan jam digital menyala berganti digit setiap periode 1/625 sekon (tanpa melihat error)
state_menit_detik = menyatakan jam digital menyala dalam format menit:detik dengan pengecekan 100/625 sekon pada button.
State ini mengaktifkan digit2 yg menyala setiap periode (jam:menit untuk 0,1,2,3 dan menit:detik untuk 2,3,4,5).
state_set_jam = menyatakan jam digital dapat diubah secara manual dengan pengecekan 100/625 sekon.
State ini mengaktifkan mode set_jam dengan mematikan state_quo. Penjelasan lengkap di fungsi ISR
state_quo = menyatakan jam berada pada mode normal yaitu dengan menambah clock 1 detik setiap siklusnya selama 625 kali looping (dianggap ideal).
state_ganti_clock = state jam digital menambah clock 1 detik. State ini aktif ketika state_quo aktif dan ter-enable secara otomatis.
state_switch_seven_segment = state yang aktif ketika mode state_quo dan button 3 ditekan. State ini aktif ketika mode set jam
serta dengan menekan button 2.
*/

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
      if (digitalRead(button1)==LOW){ //tekan button 1
        state_menit_detik = !state_menit_detik;
      }
      else if (digitalRead(button2)==LOW){ //tekan button 2
        state_set_jam=1;
        state_quo=0;
      }
      else if (digitalRead(button3)==LOW){ //tekan button 3
        state_alarm=1;
        state_quo=0;
        state_alarm_on=0;
      }
      else if (digitalRead(button4)==LOW){ //tekan button 4
        state_quo_stopwatch=1;
        state_quo=0;
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
    else if(state_alarm==1){
      if (digitalRead(button1)==LOW){
        state_stopwatch=0;
        state_quo=1;
        state_switch_seven_segment=0;
        state_alarm_on=1;
      }
      else if (digitalRead(button2)==LOW){
        state_switch_seven_segment=!state_switch_seven_segment;
      }
      else if (digitalRead(button3)==LOW){
        state_penjumlahan=1;
      }
      else if (digitalRead(button4)==LOW){
        state_pengurangan=1;
      }
      state_menit_detik=0;
    }
    else if (state_quo_stopwatch){
      if (digitalRead(button1)==LOW){ //tekan button 1
        state_quo=1;
        state_quo_stopwatch=0;
        state_switch_seven_segment=0;
      }
      else if (digitalRead(button2)==LOW){ //tekan button 2
        state_set_stopwatch=1;
        state_quo_stopwatch=0;
      }
    }
    else if (state_set_stopwatch){
      if (digitalRead(button1)==LOW){ //tekan button 1
        state_quo_stopwatch=1;
        state_set_stopwatch=0;
      }
      else if (digitalRead(button2)==LOW){ //tekan button2
        state_start_pause=!state_start_pause;
      }
    }
  }
  if (abs(clock[0]-alarm[0])==0 && abs(clock[1]-alarm[1])==0 && abs(clock[2]-alarm[2])==0 && abs(clock[3]-alarm[3])==0 && state_alarm_on==1){  
    counter_alarm++;
  }

  if (counter==625){
    if (state_quo==1 || state_start_pause==1){
      state_ganti_clock = 1;    
    }
    counter=0;
  }
}

/*
Fungsi Utama
Memberikan eksekusi fungsi-fungsi yang state-nya telah dikonfigurasikan di dalam ISR
*/
void loop(){
  if (state_quo_stopwatch || state_set_stopwatch){
    digit1=0;
    digit2=1;
    digit3=2;
    digit4=3;
    if (state_ganti_digit==1){
      display_clock(clock_stopwatch);
      state_ganti_digit = 0;
    }
    if (state_quo_stopwatch){
      clock_stopwatch[0]=0;
      clock_stopwatch[1]=0;
      clock_stopwatch[2]=0;
      clock_stopwatch[3]=0;
    }
    if (state_set_stopwatch){
      if (state_start_pause){
        if (state_ganti_clock){
          setting_stopwatch();
          state_ganti_clock=0;
        }
      }
    }
  }
  else{
    if (state_ganti_digit==1){
      if (state_quo==1 || state_set_jam==1){
        display_clock(clock);
      }
      else if (state_alarm==1){
        display_clock(alarm);
      }
      state_ganti_digit = 0;
    }
    if (state_alarm_on==1){
      alarm_on();
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
      set_jam(clock);
    }
    else if (state_quo==1){
      if (state_ganti_clock==1){
        setting_clock(clock);
        state_ganti_clock = 0;
      }
    }
    else if (state_alarm==1){
      set_jam(alarm);
    }
  }
}
