
#include<SoftwareSerial.h>
SoftwareSerial TRP(3,11);
#define reset 0
#define closed 0 //door conditions
#define opened 1
#define opening 2
#define closing 3

#define leddoor 12 //input pins
#define ledone 13
#define ledtwo 8 

#define bulb 7

#define motoropenhigh 2 //output pins
#define motorclosehigh 4
#define PWM 3

#define motorspeed 255 //0 to 255 for 0 to 5V

#define delayopen 2500
#define delayclose 2500

#define inattempt 1 //attempts
#define outattempt 2
#define noattempt 0

#define blocked HIGH
#define unblocked LOW




int in=0,out=0,door=closed, blockattempt=noattempt,unblockattempt=noattempt;
unsigned long time1b=reset,time2b=reset,time0=reset,time1u=reset,time2u=reset;
boolean laststatus1=unblocked,laststatus2=unblocked,laststatus0=unblocked;

void light();
void opendoor();
void closedoor();
void countpeople(); // to count people , call reset and closedoor
void timerecord();
void timeropen(unsigned long timercount);
void timerclose(unsigned long timercount);
void motoropendoor();
void motorclosedoor();
void motorstop();
void reset_data();

void setup() {
  // put your setup code here, to run once:
  TRP.begin(9600);
  pinMode(leddoor,INPUT);
  pinMode(ledone,INPUT);
  pinMode(ledtwo,INPUT);

  pinMode(motoropenhigh,OUTPUT);
  pinMode (motorclosehigh, OUTPUT);
  pinMode (PWM,OUTPUT);

  
  pinMode (bulb,OUTPUT);
  digitalWrite(bulb,LOW);
  digitalWrite(motoropenhigh,LOW);
  digitalWrite(motorclosehigh,LOW);
  analogWrite (PWM,motorspeed);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  timerecord();
  countperson();
  light(); 
  

if ((time1b>0 || time2b>0)  && door==closed)
     { opendoor();
     }
      
    } //loop terminates



void timerecord()
{
  if (digitalRead(ledone)==blocked && laststatus1==unblocked)
   {
    time1b=millis();
    laststatus1=blocked;
   }


   
   else if (digitalRead(ledone)==unblocked && laststatus1==blocked)
   {
    time1u=millis();
    laststatus1=unblocked;
   }
   
    if (digitalRead(ledtwo)==blocked && laststatus2==unblocked)
   {
    time2b=millis();
    laststatus2=blocked;
   }
   else if (digitalRead(ledtwo)==unblocked && laststatus2==blocked)
   {
    time2u=millis();
    laststatus2=unblocked;
   }

}

void light()
{
  if (in>out && bulb!=HIGH)
  digitalWrite(bulb,HIGH);
  else
  digitalWrite(bulb,LOW);
 int a=5;
 TRP.flush();
  TRP.print("in= ");
  TRP.print(in);
  TRP.print(",,out= ");
  TRP.print(out);
  TRP.print(",,blockattempt=");
  TRP.print(blockattempt);
  TRP.print(",,unblockattempt=");
  TRP.print(unblockattempt);
  TRP.print(",,time1b=");
  TRP.print(time1b);
    TRP.print(",,time1u=");
  TRP.print(time1u);
   TRP.print(",,time2b=");
  TRP.print(time2b);
    TRP.print(",,time2u=");
  TRP.println(time2u);
  
}

void opendoor()
{
  door=opening;
  motoropendoor();
  
    
    timeropen(millis()); //delay while timerecording and counting 
   motorstop();
  door=opened;
  return;
}



void closedoor()
{
  door=closing;
  motorclosedoor();
 
 
    timerclose(millis()); //delay while timerecording and counting and preventing accident
  
   motorstop();
   door=closed;
  return;
}


void timeropen(unsigned long timercount)
{
  do
  {
 timerecord();
 countperson();
 light();
  } while (millis() -timercount< delayopen);
  
}


void timerclose(unsigned long timercount)
{
  do
  {
    if (digitalRead(leddoor)==blocked)
    motorstop();
    else motorclosedoor();
 timerecord();
 countperson();
 light();
  } while (millis() -timercount< delayclose);
  
}

void motoropendoor()
{
  if (motoropenhigh!=HIGH || motorclosehigh !=LOW)
  {
  digitalWrite(motoropenhigh,HIGH);
  digitalWrite(motorclosehigh,LOW);
  }
}


void motorclosedoor()
  {
    if (motoropenhigh!=LOW || motorclosehigh !=HIGH)
    {
  digitalWrite(motoropenhigh,LOW);
  digitalWrite(motorclosehigh,HIGH);
    }
}


void motorstop()
  {
  digitalWrite(motoropenhigh,LOW);
  digitalWrite(motorclosehigh,LOW);
}

void countperson()
{


   if (time1b<time2b  && time1b>0 &&time2b>0 && blockattempt!=outattempt)
   blockattempt=inattempt;

    if ( time1b>time2b && time1b>0 &&time2b>0 && blockattempt!=inattempt)
   blockattempt=outattempt;

   if (time1u<time2u  && time1u>0 &&time2u>0 )
   unblockattempt=inattempt;

    if (time1u>time2u &&  time1u>0 &&time2u>0)
   unblockattempt=outattempt;

   if (blockattempt==inattempt && unblockattempt==inattempt &&digitalRead(ledone)==unblocked &&digitalRead(ledtwo)==unblocked &&digitalRead(leddoor)==unblocked)
   {
    in=in +1;
    reset_data();
    if (door==opened)
    closedoor();
   }



   else if (blockattempt==outattempt && unblockattempt==outattempt &&digitalRead(ledone)==unblocked &&digitalRead(ledtwo)==unblocked &&digitalRead(leddoor)==unblocked)
   {
    out=out +1;
    reset_data();
    if (door==opened)
    closedoor();
   }


   else if ((blockattempt==inattempt && unblockattempt==outattempt)||(unblockattempt==inattempt && blockattempt==outattempt) &&digitalRead(ledone)==unblocked &&digitalRead(ledtwo)==unblocked &&digitalRead(leddoor)==unblocked )
     {
          reset_data();
          if (door==opened)
          closedoor();
     }
     
           else if (digitalRead(ledone)==unblocked && digitalRead(ledtwo==unblocked) && digitalRead (leddoor)== unblocked && door== opened)
           {
           closedoor();
           reset_data();
           }
     
}



void reset_data()
{
   time0=time1b=time1u=time2u=time2b=reset;
   blockattempt=noattempt;
   unblockattempt=noattempt;
}



 


