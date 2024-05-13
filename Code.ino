#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <TM1637Display.h>
#include<Servo.h>

Servo s1;

#define pin0 A1
#define pin1 A2
#define pin2 A3
#define pin3 1
#define pin4 9
#define pin5 10
#define pin6 11
#define pin7 0

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


LiquidCrystal_I2C lcd(0x27, 16, 2);  // connect the "SDA = A4" and "SCL = A5" of I2C LCD

const byte ROWS = 5; // FOUR ROWS
const byte COLS = 4; // FOUR COLUMNS

//Define the keymap
char keys[ROWS][COLS] = {
  {'B','P','M','N'},
  {'9','8','7','z'},
  {'4','5','6','y'},
  {'1','2','3','w'},
  {'0','C','E','v'},
};

byte rowPins[ROWS] = {13,12,8,7,6}; //connect to the row pinsouts of the keypad
byte colPins[COLS] = {5,4,3,2}; //connect to the columns pinouts of the keypad

//create an object to keypad
Keypad kpd = Keypad(makeKeymap(keys), rowPins,colPins,ROWS,COLS);

float num1, num2, number;
int num11, num22;
char key, action;
boolean result = false;

const int min=1;       // for maximum number of menus
const int max=8;       // for minimum number of menus

const int exp_min=1;       // for maximum number of experiments
const int exp_max=18;         // for maximum number of experiments

int k=0;  // Balancing Variable for the LCD blinking

int x=min, expr = exp_min;    // for counting the value of the experiment number which is running on
int a=0, aa=0;        // variable of "all_menus()" function, which is used to acccess or terminate this function
int count=0, lab=0;   //for storing the secuence of execution (where the execution is going on), which help me to make multifunction only in 4 keys
int cal=0,tim=0, da=0, vol=0, abt=0;

void setup() {

 lcd.begin(16,2);               // Initializing LCD
 lcd.backlight(); 
}

void loop(){
  count=0;  
  lcd.setCursor(0,0); lcd.print("#####<HOME>#####"); detect_key(); Time();
}

/////////////////////////////////////////////////////////////////////////// BACK, MENU & KEY FUNCTIONS //////////////////////////////////////////////////////////////////////////////

void detect_key(){
   key = kpd.getKey(); 
 if(key!=NO_KEY){

   if(key=='P'){
               if(count==2){x--;maintain_x();lcd.clear();all_menus();}
               if(count==4){expr--;maintain_exp();lcd.clear();all_experiment();}
               }
   else if(key=='N'){
                    if(count==2){x++;maintain_x();lcd.clear();all_menus();}
                    if(count==4){expr++;maintain_exp();lcd.clear();all_experiment();}
                    }
   else if(key=='M'){lcd.clear(); if(count==0){all_menus();} 
   
                                  if((count==2) && (a==1)){all_experiment();}
                                  if((count==2) && (a==2)){calculattor();}
                                  if((count==2) && (a==3)){time_date();}
                                  if((count==2) && (a==4)){day();}
                                  if((count==2) && (a==5)){voltmeter();}
                                  if((count==2) && (a==6)){voltmeter();}
                                  if((count==2) && (a==7)){about_us();}
                                  if((count==2) && (k==6)){Time(); time_date(); day(); about_us(); mulSegment();}     // For balancing the LCD blinking
                                  if((count==2) && (k==5)){Time(); time_date(); day(); about_us(); mulSegment();}     // For balancing the LCD blinking
                                  
                                  if((count==4) && (aa==1)){ledBlink1();}
                                  if((count==4) && (aa==2)){ledBlink2();}
                                  if((count==4) && (aa==3)){ledFading();}
                                  if((count==4) && (aa==4)){ledScrolling();}
                                  if((count==4) && (aa==5)){switchLed();}
                                  if((count==4) && (aa==6)){switchInrDcr();}
                                  if((count==4) && (aa==7)){segment1();}
                                  if((count==4) && (aa==8)){segment2();}
                                  if((count==4) && (aa==9)){mulSegment();}
                                  if((count==4) && (aa==10)){lm35();}
                                  if((count==4) && (aa==11)){IRsensor();}
                                  if((count==4) && (aa==12)){ultrasonic();}
                                  if((count==4) && (aa==13)){relay();}
                                  if((count==4) && (aa==14)){buzzer();}
                                  if((count==4) && (aa==15)){ldr();}
                                  if((count==4) && (aa==16)){DCmotor();}
                                  if((count==4) && (aa==17)){servo();}
                                  if((count==4) && (aa==18)){stepper();}
     }
  else if(key=='B'){lcd.clear();
                  
                    if(count==2){a=0;}
                    if(count==4){aa=0;}
                    if(count==6){cal=0;}
                    if(count==8){tim=0;}
                    if(count==10){da=0;}
                    if(count==12){abt=0;}
                    if(count==14){vol=0;}
                    if(count==18){lab=0;}
  }

 else if(key=='C') {if(count==6) {number=num1=num2=0; result=false;lcd.clear();}}
 else if(key=='1') {if(count==6) {if (number==0) number=1;else number = (number*10) + 1;}}
 else if(key=='4') {if(count==6) {if(number==0) number=4; else number = (number*10) + 4;}}
 else if(key=='7') {if(count==6) {if (number==0) number=7; else number =(number*10) + 7;}}
 else if(key=='0') {if(count==6) {if (number==0) number=0; else number = (number*10) + 0;}}
 else if(key=='2') {if(count==6) {if (number==0) number=2; else number = (number*10)  + 2;}}
 else if(key=='5') {if(count==6) {if(number==0) number=5; else number=(number*10) + 5;}}
 else if(key=='8') {if(count==6) {if(number==0) number=8; else number=(number*10) + 8;}}
 else if(key=='E') {if(count==6) {num2=number; result=true;}}
 else if(key=='3') {if(count==6) {if(number==0) number=3; else number = (number*10) + 3;}}
 else if(key=='6') {if(count==6) {if(number==0) number=6; else number=(number*10) + 6;}}
 else if(key=='9') {if(count==6) {if(number==0) number=9; else number=(number*10) + 9;}}
 else if(key=='v' || key=='w' || key=='y' || key=='z') if(count==6){
  {
    num1=number;
    number=0;
    if(key=='v')
    {Serial.println("Addition"); action='+';}
    if(key=='w')
    {Serial.println("Subtraction"); action='-';}
    if(key=='y')
    {Serial.println("Multiplication"); action='*';}
    if(key=='z')
    {Serial.println("Devesion"); action='/';}
    delay(100);
      }  
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////// ALL MENUS //////////////////////////////////////////////////////////////////////////////////////

void all_menus(){
  count=2;                                                              
  a=x;
  
  while(a==1){ lcd.setCursor(6,0); lcd.print("MENU"); lcd.setCursor(0,1); lcd.print("All Experiments"); detect_key();}             // menu location
  while(a==2){ lcd.setCursor(6,0); lcd.print("MENU"); lcd.setCursor(3,1); lcd.print("Calculator"); detect_key();}
  while(a==3){ lcd.setCursor(6,0); lcd.print("MENU"); lcd.setCursor(2,1); lcd.print("Time & Date"); detect_key();}
  while(a==4){ lcd.setCursor(6,0); lcd.print("MENU"); lcd.setCursor(3,1); lcd.print("Current Day"); detect_key();}
  while(a==5){ lcd.setCursor(6,0); lcd.print("MENU"); lcd.setCursor(1,1); lcd.print("Voltmeter (DC)"); detect_key();}
  while(a==6){ lcd.setCursor(6,0); lcd.print("MENU"); lcd.setCursor(1,1); lcd.print("Voltmeter (AC)"); detect_key();}  
  while(a==7){ lcd.setCursor(6,0); lcd.print("MENU"); lcd.setCursor(4,1); lcd.print("About us"); detect_key();}
  while(a==8){ lcd.setCursor(6,0); lcd.print("MENU"); lcd.setCursor(4,1); lcd.print("Others"); detect_key();}
  while((a>8) || (a<1)){count=0; x=1; lcd.clear(); break;}
 
}

////////////////////////////////////////////////////////////////////////////////// ALL EXPERIMENTS ///////////////////////////////////////////////////////////////////////////////

void all_experiment(){
  count=4;
  aa=expr;

  while(aa==1) { lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(1,1); lcd.print("LED Blinking 1"); detect_key();} 
  while(aa==2) { lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(1,1); lcd.print("LED Blinking 2"); detect_key();}
  while(aa==3) { lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(3,1); lcd.print("LED Fading"); detect_key();}
  while(aa==4) { lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(1,1); lcd.print("LED Scrolling"); detect_key();}
  while(aa==5) { lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(2,1); lcd.print("Switch & LED"); detect_key();}
  while(aa==6) { lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(0,1); lcd.print("Switch inr & dcr"); detect_key();}
  while(aa==7) { lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(0,1); lcd.print("7-Segmnt(Cathod)"); detect_key();}
  while(aa==8) { lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(0,1); lcd.print("7-Segmnt (Anode)"); detect_key();}
  while(aa==9) { lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(0,1); lcd.print("Multi 7 Segment"); detect_key();}
  while(aa==10){ lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(0,1); lcd.print("Temp-Sensor-LM35"); detect_key();}
  while(aa==11){ lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(3,1); lcd.print("IR Sensor"); detect_key();}
  while(aa==12){ lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(0,1); lcd.print("Ultrasnic Sensor"); detect_key();}
  while(aa==13){ lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(0,1); lcd.print("Relay Intrfacing"); detect_key();}
  while(aa==14){ lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(0,1); lcd.print("Buzzer Intrfcing"); detect_key();}
  while(aa==15){ lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(0,1); lcd.print("LDR Interfacing"); detect_key();}
  while(aa==16){ lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(4,1); lcd.print("DC Motor"); detect_key();}
  while(aa==17){ lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(2,1); lcd.print("Servo Motor"); detect_key();}
  while(aa==18){ lcd.setCursor(2,0); lcd.print("EXPERIMENTS"); lcd.setCursor(1,1); lcd.print("Stepper Motor"); detect_key();}
  while((aa>18) || (aa<1)){count=2; expr=1; lcd.clear(); break;}   

}

void maintain_x(){
  if(x>max) x=min;
  if(x<min) x=max;
}

void maintain_exp(){
  if(expr>exp_max) expr=exp_min;
  if(expr<exp_min) expr=exp_max;
}

void others(){

while(a==2){ lcd.setCursor(6,0); lcd.print("MENU"); lcd.setCursor(3,1); lcd.print("Calculator"); detect_key();}
}

////////////////////////////////////////////////////////////// DATE & TIME, CALCULATOR, DELAY_MS & ALL_CLEAR FUNCTIONS //////////////////////////////////////////////////////////////

void voltmeter(){
   count=14; vol=1;
   float input_volt = 0.0; float temp=0.0; float r1=575.0; float r2=98.0;  //r1 = 560k Ohm    //r2 = 100k Ohm
 while(vol){
   lcd.setCursor(0,0); lcd.print("DIGITAL VOLTMTER");
   int analogvalue = analogRead(A0); temp = (analogvalue * 5.0) / 1024.0; input_volt = temp / (r2/(r1+r2));  // Calculation                          // Digital Voltmeter Function
  if (input_volt < 0.1) {input_volt=0.0;}
   lcd.setCursor(0, 1); lcd.print("Voltage = "); lcd.print(input_volt); delay_ms(300);
  if(vol==0){count=2; lcd.clear(); break;}}
}

void Time(){                                                               // Time Function

if (! rtc.begin()) { lcd.print("Couldn't find RTC"); while (1);}
if (! rtc.isrunning()) { lcd.print("RTC is NOT running!");}

   DateTime now = rtc.now();
    lcd.setCursor(4, 1);
    lcd.print(now.hour());
    lcd.print(':');
    lcd.print(now.minute());
    lcd.print(':');
    lcd.print(now.second());
    lcd.print("  ");
}

void day(){                                                               // Day Function
  count=10;
  da=1;

if (! rtc.begin()) { lcd.print("Couldn't find RTC"); while (1);}
if (! rtc.isrunning()) { lcd.print("RTC is NOT running!");}

while(da){
 DateTime now = rtc.now();
    lcd.setCursor(4,1);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);

    detect_key();
    if(da==0){count=2; lcd.clear(); break;}
  }
}

void time_date(){                                                       // Time & Date Function

count=8;
tim=1;

if (! rtc.begin()) { lcd.print("Couldn't find RTC"); while (1);}
if (! rtc.isrunning()) { lcd.print("RTC is NOT running!");}
while(tim) 
{
    DateTime now = rtc.now();
    lcd.setCursor(0, 1);
    lcd.print("TIME");
    lcd.print(" ");
    lcd.print(now.hour());
    lcd.print(':');
    lcd.print(now.minute());
    lcd.print(':');
    lcd.print(now.second());
    lcd.print("  ");

    lcd.setCursor(0, 0);
    lcd.print("DATE");
    lcd.print(" ");
    lcd.print(now.day());
    lcd.print('/');
    lcd.print(now.month());
    lcd.print('/');
    lcd.print(now.year());
    lcd.print("  ");  

    detect_key();  
    if(tim==0){count=2; lcd.clear(); break;}
 }
}

void about_us(){
  count=12;
  abt=1;
while(abt) {
   lcd.clear(); lcd.setCursor(2,0); lcd.print("This project"); lcd.setCursor(0,1); lcd.print("(LAB  ASSISTANT)"); delay_ms(3000);            // About us
 if(abt==0){count=2; lcd.clear(); break;}
   lcd.clear(); lcd.setCursor(7,0); lcd.print("is"); lcd.setCursor(4,1); lcd.print("made by"); delay_ms(3000);
 if(abt==0){count=2; lcd.clear(); break;}
   lcd.clear(); lcd.setCursor(2,0); lcd.print("Dharani  and"); lcd.setCursor(2,1); lcd.print("Dipendranath"); delay_ms(3000);
 if(abt==0){count=2; lcd.clear(); break;}
   lcd.clear(); lcd.setCursor(1,0); lcd.print("Circuit Globe"); lcd.setCursor(5,1); lcd.print("Group"); delay_ms(3000);
 if(abt==0){count=2; lcd.clear(); break;}
 lcd.clear(); lcd.setCursor(2,0); lcd.print("(2020-2023)"); lcd.setCursor(5,1); lcd.print("Batch"); delay_ms(3000);
 if(abt==0){count=2; lcd.clear(); break;} }
}

void calculattor(){                                                 // Calculator Function
count=6;
cal=1;

while(1) {
 detect_key();
 if(result==true)
 CalculateResult();
 DisplayResult();

 //detect_key()
 if(cal==0){count=2; lcd.clear(); break;}
 }
}

void CalculateResult()
   {
     if(action=='+')
     number=num1+num2;
     if(action=='-')
     number=num1-num2;
     if(action=='*')
     number=num1*num2;
     if(action=='/')
     number=num1/num2;
   }

   void DisplayResult()
   {
     num11=(int)num1;
     num22=(int)num2;

   lcd.setCursor(0,0); //set the cursor to column 0, line 0
   lcd.print(num11); lcd.print(action); lcd.print(num22);

   if(result==true)
   {lcd.print("="); lcd.print(number);} //Display the result

   lcd.setCursor(0,1); //set the cursor to column 0, line 1
   lcd.print(number); //Display the result  
 }

 void delay_ms(unsigned int time){                                            // DELAY FUNCTION
  for(unsigned int i=0; i<time; i++){
     for(unsigned int j=0; j<135; j++){
  delayMicroseconds(1);
  detect_key();
     }
  }
}

void allClear(){                                                              // ALL CLEAR FUNCTION
  digitalWrite(pin0,LOW);
  digitalWrite(pin1,LOW);
  digitalWrite(pin2,LOW);
  digitalWrite(pin3,LOW);
  digitalWrite(pin4,LOW);
  digitalWrite(pin5,LOW);
  digitalWrite(pin6,LOW);
  digitalWrite(pin7,LOW);
}

////////////////////////////////////////////////////////////// FROM HERE THE EXPERIMENT PROGRAMS ARE WRITTEN /////////////////////////////////////////////////////////////////////

//                                                                    (experiment program location)                                                                             //



void ledBlink1() {
  lab=1; count=18; pinMode(pin5,OUTPUT);
  while(1) { lcd.setCursor(2,0); lcd.print("LED BLINKING"); lcd.setCursor(3,1); lcd.print("(1 Second)");
  digitalWrite(pin5,HIGH); delay_ms(1000); digitalWrite(pin5,LOW); delay_ms(1000);                                                    // LED Blinking (1 Second)
  if(lab==0){lcd.clear(); allClear(); count=4; break;} }
}

void ledBlink2() {
  lab=1; count=18; pinMode(pin5,OUTPUT);
  while(1) { lcd.setCursor(2,0); lcd.print("LED BLINKING"); lcd.setCursor(0,1); lcd.print("(200 miliSecond)");
  digitalWrite(pin5,HIGH); delay_ms(200); digitalWrite(pin5,LOW); delay_ms(200);                                                    // LED Blinking (200 mS)
  if(lab==0){lcd.clear(); allClear(); count=4; break;} }
}

void ledFading() {
  lab=1; count=18; analogWrite(pin5,OUTPUT);
 while(1) { lcd.setCursor(3,0);  lcd.print("LED FADING"); lcd.setCursor(4,1); lcd.print("Program");
 for(int i=0; i<250; i=i+10){ analogWrite(pin5,i); delay_ms(50);}                                                                                  // LED Fading
 for(int i=250; i>=10; i=i-10){ analogWrite(pin5,i); delay_ms(50); }
 if(lab==0){lcd.clear(); allClear(); count=4; break;}}
}

void ledScrolling(){
  lab=1; count=18;
  pinMode(pin0,OUTPUT); pinMode(pin1,OUTPUT); pinMode(pin2,OUTPUT); pinMode(pin3,OUTPUT); pinMode(pin4,OUTPUT); pinMode(pin5,OUTPUT);               // LED Scrolling
 while(1) {
  lcd.setCursor(1,0); lcd.print("LED SCROLLING"); lcd.setCursor(4,1); lcd.print("Program");
  digitalWrite(pin0,HIGH); delay_ms(100); digitalWrite(pin1,HIGH); delay_ms(100); digitalWrite(pin2,HIGH); delay_ms(100);
  digitalWrite(pin3,HIGH); delay_ms(100); digitalWrite(pin4,HIGH); delay_ms(100); digitalWrite(pin5,HIGH); delay_ms(100);
  digitalWrite(pin5,LOW); delay_ms(100); digitalWrite(pin4,LOW); delay_ms(100); digitalWrite(pin3,LOW); delay_ms(100);
  digitalWrite(pin2,LOW); delay_ms(100); digitalWrite(pin1,LOW); delay_ms(100); digitalWrite(pin0,LOW); delay_ms(100);
 if(lab==0){lcd.clear(); allClear(); count=4; break;}}
}

void switchLed() {
  lab=1;count=18; pinMode(pin0,INPUT_PULLUP); pinMode(pin1,OUTPUT);                                                                              // Switch and LED
 while(1) {
  lcd.setCursor(2,0); lcd.print("SWITCH & LED"); lcd.setCursor(2,1); lcd.print("Interfacing"); 
  if(digitalRead(pin0) == 0) digitalWrite(pin1,HIGH); else digitalWrite(pin1,LOW);
  detect_key(); if(lab==0){lcd.clear(); allClear(); count=4; break;}} 
}

void switchInrDcr() {
  lab=1; count=18; int pp=0;
  pinMode(pin0,INPUT_PULLUP); pinMode(pin1,INPUT_PULLUP);                                                                                         // Switch Inr & Dcr
 while(1) {
  lcd.setCursor(1,0); lcd.print("SWITCH INR-DCR");
 if(digitalRead(pin0)==LOW){
  pp++; delay_ms(200); lcd.clear(); lcd.setCursor(8,1); lcd.print(pp);}
 else{
  if(digitalRead(pin1)==LOW){
  pp--; delay_ms(200); lcd.clear(); lcd.setCursor(8,1); lcd.print(pp);}}
  detect_key(); if(lab==0){lcd.clear(); allClear(); count=4; break;}}
}

void segment1() {
   lab=1; count=18; int COUNT=0;
  pinMode(pin0,OUTPUT); pinMode(pin1,OUTPUT); pinMode(pin2,OUTPUT); pinMode(pin3,OUTPUT); pinMode(pin4,OUTPUT); pinMode(pin5,OUTPUT); pinMode(pin6,OUTPUT); //7 Segment (Common Cathode)
while(1){
  lcd.setCursor(0,0); lcd.print("7 SEGMENT DISPLY"); lcd.setCursor(0,1); lcd.print("(Common Cathode)"); 
 switch (COUNT){
  case 0: digitalWrite(pin0,1); digitalWrite(pin1,1); digitalWrite(pin2,1); digitalWrite(pin3,1); digitalWrite(pin4,1); digitalWrite(pin5,1); digitalWrite(pin6,0); break;
  case 1: digitalWrite(pin0,0); digitalWrite(pin1,1); digitalWrite(pin2,1); digitalWrite(pin3,0); digitalWrite(pin4,0); digitalWrite(pin5,0); digitalWrite(pin6,0); break;
  case 2: digitalWrite(pin0,1); digitalWrite(pin1,1); digitalWrite(pin2,0); digitalWrite(pin3,1); digitalWrite(pin4,1); digitalWrite(pin5,0); digitalWrite(pin6,1); break;
  case 3: digitalWrite(pin0,1); digitalWrite(pin1,1); digitalWrite(pin2,1); digitalWrite(pin3,1); digitalWrite(pin4,0); digitalWrite(pin5,0); digitalWrite(pin6,1); break;
  case 4: digitalWrite(pin0,0); digitalWrite(pin1,1); digitalWrite(pin2,1); digitalWrite(pin3,0); digitalWrite(pin4,0); digitalWrite(pin5,1); digitalWrite(pin6,1); break;
  case 5: digitalWrite(pin0,1); digitalWrite(pin1,0); digitalWrite(pin2,1); digitalWrite(pin3,1); digitalWrite(pin4,0); digitalWrite(pin5,1); digitalWrite(pin6,1); break;
  case 6: digitalWrite(pin0,1); digitalWrite(pin1,0); digitalWrite(pin2,1); digitalWrite(pin3,1); digitalWrite(pin4,1); digitalWrite(pin5,1); digitalWrite(pin6,1); break;
  case 7: digitalWrite(pin0,1); digitalWrite(pin1,1); digitalWrite(pin2,1); digitalWrite(pin3,0); digitalWrite(pin4,0); digitalWrite(pin5,0); digitalWrite(pin6,0); break;
  case 8: digitalWrite(pin0,1); digitalWrite(pin1,1); digitalWrite(pin2,1); digitalWrite(pin3,1); digitalWrite(pin4,1); digitalWrite(pin5,1); digitalWrite(pin6,1); break;
  case 9: digitalWrite(pin0,1); digitalWrite(pin1,1); digitalWrite(pin2,1); digitalWrite(pin3,1); digitalWrite(pin4,0); digitalWrite(pin5,1); digitalWrite(pin6,1); break;}
if (COUNT<10){COUNT++; delay_ms(500);}
if (COUNT==10){COUNT=0;}
if(lab==0){lcd.clear(); allClear(); count=4; break;}
}}

void segment2() {
   lab=1; count=18; int COUNT=0;
  pinMode(pin0,OUTPUT); pinMode(pin1,OUTPUT); pinMode(pin2,OUTPUT); pinMode(pin3,OUTPUT); pinMode(pin4,OUTPUT); pinMode(pin5,OUTPUT); pinMode(pin6,OUTPUT);    //7 Segment (Common Anode)
while(1){
  lcd.setCursor(0,0); lcd.print("7 SEGMENT DISPLY"); lcd.setCursor(1,1); lcd.print("(Common Anode)"); 
 switch (COUNT){
  case 0: digitalWrite(pin0,0); digitalWrite(pin1,0); digitalWrite(pin2,0); digitalWrite(pin3,0); digitalWrite(pin4,0); digitalWrite(pin5,0); digitalWrite(pin6,1); break;
  case 1: digitalWrite(pin0,1); digitalWrite(pin1,0); digitalWrite(pin2,0); digitalWrite(pin3,1); digitalWrite(pin4,1); digitalWrite(pin5,1); digitalWrite(pin6,1); break;
  case 2: digitalWrite(pin0,0); digitalWrite(pin1,0); digitalWrite(pin2,1); digitalWrite(pin3,0); digitalWrite(pin4,0); digitalWrite(pin5,1); digitalWrite(pin6,0); break;
  case 3: digitalWrite(pin0,0); digitalWrite(pin1,0); digitalWrite(pin2,0); digitalWrite(pin3,0); digitalWrite(pin4,1); digitalWrite(pin5,1); digitalWrite(pin6,0); break;
  case 4: digitalWrite(pin0,1); digitalWrite(pin1,0); digitalWrite(pin2,0); digitalWrite(pin3,1); digitalWrite(pin4,1); digitalWrite(pin5,0); digitalWrite(pin6,0); break;
  case 5: digitalWrite(pin0,0); digitalWrite(pin1,1); digitalWrite(pin2,0); digitalWrite(pin3,0); digitalWrite(pin4,1); digitalWrite(pin5,0); digitalWrite(pin6,0); break;
  case 6: digitalWrite(pin0,0); digitalWrite(pin1,1); digitalWrite(pin2,0); digitalWrite(pin3,0); digitalWrite(pin4,0); digitalWrite(pin5,0); digitalWrite(pin6,0); break;
  case 7: digitalWrite(pin0,0); digitalWrite(pin1,0); digitalWrite(pin2,0); digitalWrite(pin3,1); digitalWrite(pin4,1); digitalWrite(pin5,1); digitalWrite(pin6,1); break;
  case 8: digitalWrite(pin0,0); digitalWrite(pin1,0); digitalWrite(pin2,0); digitalWrite(pin3,0); digitalWrite(pin4,0); digitalWrite(pin5,0); digitalWrite(pin6,0); break;
  case 9: digitalWrite(pin0,0); digitalWrite(pin1,0); digitalWrite(pin2,0); digitalWrite(pin3,0); digitalWrite(pin4,1); digitalWrite(pin5,0); digitalWrite(pin6,0); break;}
if (COUNT<10){COUNT++; delay_ms(500);}
if (COUNT==10){COUNT=0;}
if(lab==0){lcd.clear(); allClear(); count=4; break;}
}}


void mulSegment() {
  lab=1; count=18;
  const int CLK = pin6; //Set the CLK pin connection to the display
  const int DIO = pin5; //Set the DIO pin connection to the display
  const uint8_t blank[] = {0x00, 0x00, 0x00,0x00};
  TM1637Display display(CLK, DIO); //set up the 4-Digit Display.
  int num = 0; pinMode(pin0, INPUT_PULLUP); pinMode(pin1,INPUT_PULLUP);
  display.setBrightness(0x0a); display.setSegments(blank);//clear display 
while(1){
  lcd.setCursor(0,0); lcd.print("UP/DOWN by using"); lcd.setCursor(4,1); lcd.print("(TM1637)");                                         // 4 Seven Segment Display 
  display.showNumberDec(num, true, 4, 0); delay_ms(150);
 if( digitalRead(pin0)){ num++; if(num > 9999) num = 0;}
 if( digitalRead(pin1)){num--; if(num < 0) num = 9999;}
 if(lab==0){lcd.clear(); allClear(); count=4; break;}}
}

void lm35() {
  lab=1; count=18;
 while(1) {
  int reading = analogRead(pin0); //Get the voltage from the LM35
  float voltage = reading * (5.0/1024.0); //Convert the reading into voltage 
  float tempc= voltage * 100; //Convert the voltage into the temperature in Celcius

  lcd.setCursor(0,0); lcd.print("Temperature:");
  lcd.setCursor(0,1); lcd.print((int)tempc); lcd.print("`"); lcd.print('C'); lcd.print("  "); delay_ms(1000);                                  // LM35 Temperature Sensor
 if(lab==0){lcd.clear(); allClear(); count=4; break;} }
  }

void IRsensor() {
  lab=1; count=18; pinMode(pin0,INPUT_PULLUP); pinMode(pin1,OUTPUT);
 while(1){
  lcd.setCursor(3,0); lcd.print("IR SENSOR"); lcd.setCursor(2,1); lcd.print("Interfacing");                                                   // IR Sensor Interfacing
  int val= digitalRead(pin0);
 if (val==0){ digitalWrite(pin1,HIGH);} else{ digitalWrite(pin1,LOW);} delay_ms(500);
 if(lab==0){lcd.clear(); allClear(); count=4; break;} }
}

void ultrasonic(){
  lab=1; count=18; pinMode(pin5,OUTPUT); pinMode(pin6,INPUT);    // pin5 = trigger pin & pin6 = echo pin
 while(1) { 
  long highPulseDuration; int calculation;
  digitalWrite(pin5, LOW); //set the trigger pin to low, before setting it to high for the pulse                                             // Ultrasonic Interfacing
  delayMicroseconds(5);
  digitalWrite(pin5, HIGH); //create the 10 microseconds pulse on the trigger pin
  delayMicroseconds(10); digitalWrite (pin5,LOW);//set the pin to low to end the pulse
  highPulseDuration = pulseIn(pin6,HIGH); //read the duration of high pulse on the echo pin
  //calculating the distance
  calculation=highPulseDuration*0.034/2; //speed of sound wave divided by 2 (go and back)
  lcd.setCursor(0,0); lcd.print("Distance(in cm):"); lcd.setCursor(0,1); lcd.print(calculation); delay_ms(500); lcd.clear();
  if(lab==0){lcd.clear(); allClear(); count=4; break;} }
}

void relay() {
  lab=1; count=18; pinMode(pin0,OUTPUT);
 while(1) { lcd.setCursor(0,0); lcd.print("RELAY INTRFACING"); lcd.setCursor(4,1); lcd.print("Program");
  digitalWrite(pin0,HIGH); delay_ms(1000); digitalWrite(pin0,LOW); delay_ms(1000);                                                          // Relay Interfacing
 if(lab==0){lcd.clear(); allClear(); count=4; break;} }
}

void buzzer() {
 lab=1; count=18; analogWrite(pin0,OUTPUT);
 while(1) { lcd.setCursor(5,0); lcd.print("BUZZER"); lcd.setCursor(2,1); lcd.print("Interfacing");                                          // Buzzer Interfacing
  digitalWrite(pin0,HIGH); delay_ms(1000); digitalWrite(pin0,LOW); delay_ms(1000);
 if(lab==0){lcd.clear(); allClear(); count=4; break;}}
}

void ldr() {
  lab=1; count=18; pinMode(pin1,OUTPUT);
 while(1) { lcd.setCursor(0,0); lcd.print("LDR INTERFACING"); lcd.setCursor(4,1); lcd.print("Program");
  int val=analogRead(pin0);
 if(val<250){ digitalWrite(pin1,HIGH); } else{ digitalWrite(pin1,LOW);}                                                                     // LDR Interfacing
  detect_key(); if(lab==0){lcd.clear(); allClear(); count=4; break;} }
}

void DCmotor() {
  lab=1; count=18; analogWrite(pin5,OUTPUT); analogWrite(pin6,OUTPUT);
 while(1) { lcd.setCursor(4,0); lcd.print("DC MOTOR"); lcd.setCursor(4,1); lcd.print("Control");                                           // DC Motor Control
 analogWrite(pin6,0); for(int i=0; i<250; i=i+10){ analogWrite(pin5,i); delay_ms(50);} for(int i=250; i>=10; i=i-10){ analogWrite(pin5,i); delay_ms(50);}
 if(lab==0){lcd.clear(); allClear(); count=4; break;}
 analogWrite(pin5,0); for(int i=0; i<250; i=i+10){ analogWrite(pin6,i); delay_ms(50);} for(int i=250; i>=10; i=i-10){ analogWrite(pin6,i); delay_ms(50);}
 if(lab==0){lcd.clear(); allClear(); count=4; break;}}
}

void servo() {
  lab=1; count=18; s1.attach(pin5);
 while(1) { lcd.setCursor(2,0); lcd.print("SERVO MOTOR"); lcd.setCursor(2,1); lcd.print("Interfacing");                                   // Servo Motor Interfacing 
 for (int i=0; i<=180; i++){s1.write(i); delay_ms(15);}
 if(lab==0){lcd.clear(); allClear(); count=4; break;}   
 for (int i=180; i>=0; i--){s1.write(i); delay_ms(15);}
 if(lab==0){lcd.clear(); allClear(); count=4; break;} }
}

void stepper() {
  lab=1; count=18; pinMode(pin0,OUTPUT); pinMode(pin1,OUTPUT); pinMode(pin2,OUTPUT); pinMode(pin3,OUTPUT);
 while(1) { lcd.setCursor(1,0); lcd.print("STEPPER MOTOR"); lcd.setCursor(2,1); lcd.print("Interfacing");
  digitalWrite(pin0,HIGH); digitalWrite(pin1,HIGH); digitalWrite(pin2,LOW);  digitalWrite(pin3,LOW);  delay_ms(5);                      // Stepper Motor Interfacing
  digitalWrite(pin0,LOW);  digitalWrite(pin1,HIGH); digitalWrite(pin2,HIGH); digitalWrite(pin3,LOW);  delay_ms(5);
  digitalWrite(pin0,LOW);  digitalWrite(pin1,LOW);  digitalWrite(pin2,HIGH); digitalWrite(pin3,HIGH); delay_ms(5);
  digitalWrite(pin0,HIGH); digitalWrite(pin1,LOW);  digitalWrite(pin2,LOW);  digitalWrite(pin3,HIGH); delay_ms(5);
 if(lab==0){lcd.clear(); allClear(); count=4; break;} }
}