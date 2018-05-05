#include"HX711.h"
#include<Servo.h>
#include<SoftwareSerial.h>

#define RELAY1 5
#define RELAY2 6
SoftwareSerial mySerial(9,10);
HX711 cell(3,2);
Servo myServo;

long val=0;
float count=0;
int actualWeight=0;
int percentage=0;
int estimatedWeight=0;
char msg=0;

void setup()
{
  Serial.begin(9600);
  myServo.attach(11);
  mySerial.begin(9600);
  pinMode(RELAY1,OUTPUT);
  pinMode(RELAY2,OUTPUT);
  delay(100);
 }

 void loop()
 {
  count=count+1;
 //val=((count-1)/count)*val  + (1/count)*cell.read();
  val = cell.read();
  estimatedWeight=(val-136601)/119.848f;
  Serial.print(" estimated Weight : ");
  Serial.print(estimatedWeight);
  percentage= map( estimatedWeight,0,500,0,100);
  Serial.print("  ");
  Serial.print(" percentage : ");
  Serial.println(percentage);

  if(percentage>30)
  {
    myServo.write(142);
    delay(1000);
    Serial.print(" now the saline  is at : ");
    Serial.println(percentage);
    digitalWrite(RELAY1,HIGH);
    digitalWrite(RELAY2,LOW);
    delay(1000);
   }
   
   else if(17<percentage<=25)
   {
    myServo.write(149);
    delay(1000);
    Serial.print(" now the saline  is at : ");
    Serial.println(percentage);
    digitalWrite(RELAY1,LOW);
    digitalWrite(RELAY2,HIGH);
    delay(1000);
    if(percentage==20)
    {
      Serial.println(" your saline at about to end. its at 20%  ");
      sendMessage1();
    }
   }

   else if(10<percentage<=17)
   {
    myServo.write(158);
    delay(1000);
    Serial.print(" now the saline  is at : ");
    Serial.println(percentage);
    digitalWrite(RELAY1,LOW);
    digitalWrite(RELAY2,HIGH);
    delay(1000);
    if(percentage==15)
    {
      Serial.println(" your saline at about to end. its at 15%  ");
      warning();
    }
    }

    else if(5<percentage<=10)
   {
    myServo.write(168);
    delay(1000);
    Serial.print(" now the saline  is at : ");
    Serial.println(percentage);
    digitalWrite(RELAY1,LOW);
    digitalWrite(RELAY2,HIGH);
    delay(1000);
    if(percentage==8)
    {
      Serial.println(" your saline at about to end. its at 8%  ");
      FinalWarning();
    }
    }

    else if(percentage<=5)
   {
    myServo.write(170);
    delay(1000);
    Serial.print(" now the saline  is at : ");
    Serial.println(percentage);
    digitalWrite(RELAY1,LOW);
    digitalWrite(RELAY2,HIGH);
    delay(1000);
    if(percentage==4)
    {
      Serial.println(" your saline at about to end. its at less than 5%  ");
      Warningwarning();
    }
    }
      
 }

 void sendMessage1()
{
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+919292307899\"\r");
  delay(1000);
  mySerial.println("your saline at about to end. its at 20% , plz be in ward to change the saline  ");
  delay(1000);
  mySerial.println((char)26);
  delay(1000);
}

void warning()
{
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+917986057623\"\r");
  delay(1000);
  mySerial.println("your saline at about to end. its at 15% , plz be in ward to change the saline or ' if you want stop the saline replay yes '  ");
  delay(1000);
  mySerial.println((char)26);
  delay(1000);
  for(int i=0;i<=60;i++)
  {
   msg = mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
   if(msg=="yes")
   {
    myServo.write(170);
    exit(0);
   }
   delay(1000);
  }
}

void FinalWarning()
{
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+917986057623\"\r");
  delay(1000);
  mySerial.println("your saline at about to end. its at 8% , plz be in ward to change the saline or ' if you want stop the saline replay (yes) or it will be automatically stoped  '  ");
  delay(1000);
  mySerial.println((char)26);
  delay(1000);
  for(int i=0;i<=60;i++)
  {
   msg = mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
   if(msg=="yes")
   {
    myServo.write(170);
    exit(0);
   }
   if(i==10)
   {
    mySerial.println("ATD+917986057623;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
    delay(100);
   }
    delay(1000);
   }
}

void Warningwarning()
{
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+917986057623\"\r");
  delay(1000);
  mySerial.println("its critical saline is automatically shutdown   ");
  delay(1000);
  mySerial.println((char)26);
  delay(1000);
  myServo.write(170);
  delay(1000);
  exit(0);

}











 

