//#include<SoftwareSerial.h>
//SoftwareSerial TRP(3,11);
#define reset 0
#define closed 0 //door conditions
#define opened 1
#define opening 2
#define closing 3

#define leddoor 7 //input pins
#define ledone 8
#define ledtwo 4

#define bulb 2

#define motoropenhigh 12 //output pins
#define motorclosehigh 13
#define PWM 3

#define motorspeed 255 //0 to 255 for 0 to 5V

#define delayopen 2300
#define delayclose 2300

#define inattempt 1 //attempts
#define outattempt 2
#define noattempt 0

#define blocked HIGH
#define unblocked LOW




int in=0,out=0,door=closed, blockattempt=noattempt,unblockattempt=noattempt;
unsigned long time1b=reset,time2b=reset,time0b=reset,time0u=reset, time1u=reset,time2u=reset;
boolean laststatus1=unblocked,laststatus2=unblocked,laststatus0=unblocked,flag;

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
  Serial.begin(9600);
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


if (time1b>0 || time2b>0 && door==closed)       // point to be noted
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

   if (digitalRead(leddoor)==blocked && laststatus0==unblocked)
   {
    time0b=millis();
    laststatus0=blocked;
   }



   else if (digitalRead(leddoor)==unblocked && laststatus0==blocked)
   {
    time0u=millis();
    laststatus0=unblocked;
   }

}

void light()
{
  if (in>out )
  digitalWrite(bulb,HIGH);
  else
  digitalWrite(bulb,LOW);


  Serial.print("in= ");
  Serial.print(in);
  Serial.print(",,out= ");
  Serial.print(out);
  Serial.print(",,blockattempt=");
  Serial.print(blockattempt);
  Serial.print(",,unblockattempt=");
  Serial.print(unblockattempt);
  Serial.print(",,time1b=");
  Serial.print(time1b);
    Serial.print(",,time1u=");
  Serial.print(time1u);
  Serial.print(",,time0b=");
  Serial.print(time0b);
    Serial.print(",,time0u=");
  Serial.print(time0u);
   Serial.print(",,time2b=");
  Serial.print(time2b);
    Serial.print(",,time2u=");
  Serial.println(time2u);



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
    if (digitalRead(leddoor)==blocked ||digitalRead(ledone)==blocked ||digitalRead(ledtwo)==blocked  )
    {
    motorstop();
    flag=1;
    }
    else motorclosedoor();
 timerecord();
 countperson();
 light();
  } while (millis() -timercount< delayclose);

}

void motoropendoor()
{

  {
  digitalWrite(motoropenhigh,HIGH);
  digitalWrite(motorclosehigh,LOW);
  }
}


void motorclosedoor()
  {

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


   if (time1b<time0b && time0b< time2b && time1b>0 &&time2b>0 &&time0b>0 && blockattempt!=outattempt)
   blockattempt=inattempt;

    if ( time1b>time0b && time0b>time2b && time1b>0 &&time2b>0 && time0b>0 && blockattempt!=inattempt)
   blockattempt=outattempt;

   if (time1u<time0u && time0u<time2u  && time1u>0 &&time2u>0 && time0u>0 && unblockattempt!=outattempt)
   unblockattempt=inattempt;

    if (time1u>time0u  && time0u>time2u &&  time1u>0 &&time2u>0 && time0u>0 && unblockattempt!=inattempt)
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



           else if (digitalRead(ledone)==unblocked && digitalRead(ledtwo)==unblocked && digitalRead (leddoor)== unblocked && door== opened)
           {
            reset_data();
           closedoor();
           }

           else if (digitalRead(ledone)==unblocked && digitalRead(ledtwo)==unblocked && digitalRead(leddoor)==unblocked && flag==1)
           {
            reset_data();
            flag==0;
            closedoor();
           }
           
     else if (digitalRead(ledone)==unblocked && digitalRead(ledtwo)==unblocked && digitalRead (leddoor)== unblocked )
           {

           reset_data();
           }

            
}



void reset_data()
{
   time0b=time0u=time1b=time1u=time2u=time2b=reset;
   blockattempt=noattempt;
   unblockattempt=noattempt;
}





