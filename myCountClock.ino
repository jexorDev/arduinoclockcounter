//www.elegoo.com
//2018.10.24
#include <Wire.h>
#include "DS3231.h"

DS3231 clock;
RTCDateTime dt;

#define LATCH 9  //74HC595  pin 9 STCP
#define CLOCK 10 //74HC595  pin 10 SHCP
#define DATA 8   //74HC595  pin 8 DS

#define COM4 5
#define COM3 4
#define COM2 3
#define COM1 2
#define TIME 2

const static uint8_t num_buf[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void setup()
{
  Serial.begin(9600);

  Serial.println("Initialize RTC module");
  // Initialize DS3231
  clock.begin();

  
  // Manual (YYYY, MM, DD, HH, II, SS
  // clock.setDateTime(2016, 12, 9, 11, 46, 00);
  
  // Send sketch compiling time to Arduino
  clock.setDateTime(__DATE__, __TIME__);    
  /*
  Tips:This command will be executed every time when Arduino restarts. 
       Comment this line out to store the memory of DS3231 module
  */
  pinMode(LATCH,OUTPUT);
  pinMode(CLOCK,OUTPUT);
  pinMode(DATA,OUTPUT);

    pinMode(COM1,OUTPUT);
  pinMode(COM2,OUTPUT);
  pinMode(COM3,OUTPUT);
  pinMode(COM4,OUTPUT);
}

void loop()
{
  dt = clock.getDateTime();

  // For leading zero look to DS3231_dateformat example

  Serial.print("Raw data: ");
  Serial.print(dt.year);   Serial.print("-");
  Serial.print(dt.month);  Serial.print("-");
  Serial.print(dt.day);    Serial.print(" ");
  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");
  //string currentTime = dt.hour
  display_num(dt.minute);
  delay(1000);
}

void display_num(uint16_t num)
{
  uint8_t com=0;
  uint8_t polling=0;
  uint8_t bit_num=0;
  uint8_t num_place[4]={0};
  
  if(num>=1000&&num<=9999)
  {
   polling=4;
   bit_num=3;
   num_place[3]=num/1000;
   num_place[2]=num/100%10;
   num_place[1]=num/10%10;
   num_place[0]=num%10;
     for(int i=0;i<4;i++)
     {clear_bit_display();
      choose_com(polling);
      polling--;
      display(num_place[bit_num--]);
      delay(TIME);
     }
   }
   else if(num>=100&&num<1000)
   {
    polling=3;
    bit_num=2;
    num_place[2]=num/100;
    num_place[1]=num/10%10;
    num_place[0]=num%10;
      for(int i=0;i<3;i++)
     {clear_bit_display();
      choose_com(polling);
      polling--;
      display(num_place[bit_num--]);
      delay(TIME);
     }
    }
    else if(num>=10&&num<100)
   {clear_bit_display();
    polling=2;
    bit_num=1;
    num_place[1]=num/10;
    num_place[0]=num%10;
       for(int i=0;i<2;i++)
       {
        choose_com(polling);
        polling--;
        display(num_place[bit_num--]);
        delay(TIME);
       }
    }
    else if(num>=0 && num<=9)
    { clear_bit_display();
        num_place[0]=num%10;
        choose_com(1);
        display(num_place[0]);
        delay(TIME);
     }
    else
     {num=9999;}
 
  }

void display(uint8_t num)
{
  digitalWrite(LATCH,LOW);
  shiftOut(DATA,CLOCK,MSBFIRST,num_buf[num]);
  digitalWrite(LATCH,HIGH);
}

void choose_com(uint8_t com)
 {
  switch(com)
  {
    case 1:choose_com1();
            
            break;
    case 2:choose_com2();

            break;
   case 3:choose_com3();

            break;
    case 4:choose_com4();
            
            break;         
    default :break;
    }
  }

void choose_com1()
{
    digitalWrite(COM1,LOW);
    digitalWrite(COM2,HIGH);
    digitalWrite(COM3,HIGH);
    digitalWrite(COM4,HIGH);   
 }

 void choose_com2()
{
    digitalWrite(COM1,HIGH);
    digitalWrite(COM2,LOW);
    digitalWrite(COM3,HIGH);
    digitalWrite(COM4,HIGH);  
 }
 void choose_com3()
{
    digitalWrite(COM1,HIGH);
    digitalWrite(COM2,HIGH);
    digitalWrite(COM3,LOW);
    digitalWrite(COM4,HIGH);  
 }
 void choose_com4()
{
    digitalWrite(COM1,HIGH);
    digitalWrite(COM2,HIGH);
    digitalWrite(COM3,HIGH);
    digitalWrite(COM4,LOW);    
 }

void clear_bit_display()
{
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, 0);
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
  delay(TIME);
}


 void clear_display()
{
  for(int i=0;i<4;i++)
 {
    choose_com(i);
    digitalWrite(LATCH,LOW);
    shiftOut(DATA,CLOCK,MSBFIRST,0);
    digitalWrite(LATCH,HIGH);
    digitalWrite(LATCH,LOW);
    delay(TIME);
  }
}
