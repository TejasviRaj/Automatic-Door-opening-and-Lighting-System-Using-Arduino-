
#define reset 0
#define closed 0 //door conditions
#define opened 1
#define opening 2
#define closing 3

#define leddoor 1 //input pins
#define ledone 0
#define ledtwo 2
#define contactclose 5 
#define contactopen 6

#define motorone 8 //output pins
#define motortwo 9
#define bulb 10

#define delayopen 100
#define delayclose 100

#define inattempt 1 //attempts
#define outattempt 2
#define noattempt 0

#define blocked HIGH
#define unblocked LOW

#define touched HIGH
#define untouched LOW



int in=0,out=0,door=closed, blockattempt=noattempt,unblockattempt=noattempt;
unsigned long time1b=reset,time2b=reset,time0=reset,time1u=reset,time2u=reset,timercount;
boolean laststatus1=unblocked,laststatus2=unblocked,laststatus0=unblocked;

void light();
void opendoor();
void closedoor();
void countpeople(); // to count people , call reset and closedoor
void timerecord();
void timeropen();
void timerclose();
void motoropendoor();
void motorclosedoor();
void motorstop();
void reset_data();

void setup() {
  // put your setup code here, to run once:
  pinMode(leddoor,INPUT);
  pinMode(ledone,INPUT);
  pinMode(ledtwo,INPUT);
  pinMode(contactclose,INPUT);
  pinMode(contactopen, INPUT);
  pinMode(motorone,OUTPUT);
  pinMode (motortwo, OUTPUT);
  pinMode (bulb,OUTPUT);
  digitalWrite(bulb,LOW);
  digitalWrite(motorone,LOW);
  digitalWrite(motortwo,LOW);
  
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
  if (in>out)
  digitalWrite(bulb,HIGH);
  else
  digitalWrite(bulb,LOW);
}

void opendoor()
{
  door=opening;
  motoropendoor();
  
   do
   {
    timercount=millis();
    timeropen(); //delay while timerecording and counting 
   } while (digitalRead(contactopen)!=touched);
   motorstop();
  door=opened;
  return;
}



void closedoor()
{
  door=closing;
  motorclosedoor();
 
  do
   {
    timercount=millis();
    timerclose(); //delay while timerecording and counting and preventing accident
   } while (digitalRead(contactclose)!=touched);
   motorstop();
   door=closed;
  return;
}


void timeropen()
{
  do
  {
 timerecord();
 countperson();
 light();
  } while (millis() -timercount< delayopen);
  
}


void timerclose()
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
  if (motorone!=HIGH || motortwo !=LOW)
  {
  digitalWrite(motorone,HIGH);
  digitalWrite(motortwo,LOW);
  }
}


void motorclosedoor()
  {
    if (motorone!=LOW || motortwo !=HIGH)
    {
  digitalWrite(motorone,LOW);
  digitalWrite(motortwo,HIGH);
    }
}


void motorstop()
  {
  digitalWrite(motorone,LOW);
  digitalWrite(motortwo,LOW);
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
   blockattempt==noattempt;
   unblockattempt==noattempt;
}



 


