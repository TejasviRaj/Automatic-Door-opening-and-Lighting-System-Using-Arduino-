#define reset -100
#define closed 0
#define opened 1
#define leddoor 0
#define ledone 1
#define ledtwo 2
#define ledthree 3
#define ledfour 4
#define contactclose 5
#define contactopen 6
#define motorone 7
#define motortwo 8
#define bulb 9
#define delayopen 100
#define delayclose 100
#define inattempt 1
#define outattempt 2
#define noattempt 0
#define blocked HIGH
#define unblocked LOW
#define touched HIGH
#define untouched LOW

int in=0,out=0,door=closed, attempt=noattempt,lastattempt=noattempt;
unsigned long time0=reset,time1=reset,time2=reset,time3=reset,time4=reset,timercount;
boolean laststatus1=unblocked,laststatus2=unblocked,laststatus3=unblocked,laststatus4=unblocked,laststatus0=unblocked;

void light();
void opendoor();
void closedoor();
void countpeople();
//void timer();
//void securitycheck();
void timerecord();
void timeropen();
void timerclose();
void motoropendoor();
void motorclosedoor();
void motorstop();

void setup() {
  // put your setup code here, to run once:
  pinMode(leddoor,INPUT);
  pinMode(ledone,INPUT);
  pinMode(ledtwo,INPUT);
  pinMode(ledthree,INPUT);
  pinMode(ledfour,INPUT);
  pinMode(contactclose,INPUT);
  pinMode(contactopen, INPUT);
  pinMode(motorone,OUTPUT);
  pinMode (motortwo, OUTPUT);
  pinMode (bulb,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  countperson();
  light();
  timerecord();

 if (time1<time2&&time1>=0&&time2>=0&&door==closed)
    {
      opendoor();
      loop();
    }


    
     if (time1>time2&&time1>=0&&time2>=0&&door==opened)
    {
        closedoor();
     
      loop();
    }


    if (time3<time4&&time3>=0&&time4>=0&&door==opened)
    {
      closedoor();
      loop();
    }

    if (time3>time4&&time3>=0&&time4>=0&&door==closed)
    {
      opendoor();
      loop();
    }
    
    
} //loop terminates



void timerecord()
{
  if (digitalRead(ledone)==blocked && laststatus1==unblocked)
   {
    time1=millis();
    laststatus1=blocked;
   }
   else if (digitalRead(ledone)==unblocked)
   laststatus1=unblocked;

   
  if (digitalRead(ledtwo)==blocked && laststatus2==unblocked)
   {
    time2=millis();
    laststatus2=blocked;
   }
   else if (digitalRead(ledtwo)==unblocked)
   laststatus2=unblocked;

   
   if (digitalRead(ledthree)==blocked && laststatus3==unblocked)
   {
    time3=millis();
    laststatus3=blocked;
   }
   else if (digitalRead(ledthree)==unblocked)
   laststatus3=unblocked;

   
   if (digitalRead(ledfour)==blocked && laststatus4==unblocked)
   {
    time4=millis();
    laststatus4=blocked;
   }
   else if (digitalRead(ledfour)==unblocked)
   laststatus4=unblocked;

   
   if (digitalRead(leddoor)==blocked && laststatus0==unblocked)
   {
    time0=millis();
    laststatus0=blocked;
   }
   else if (digitalRead(leddoor)==unblocked)
   laststatus0=unblocked;

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


   if (time1<time2 && time2< time0 && time0<time3 && time3<time4 && time1>=0 &&time2>=0 &&time3>=0 &&time4>=0 &&time0>=0)
   attempt=inattempt;

    if (time1>time2 && time2> time0 && time0>time3 && time3>time4 && time1>=0 &&time2>=0 &&time3>=0 &&time4>=0 &&time0>=0)
   attempt=outattempt;

   if (attempt==inattempt && lastattempt!=outattempt &&digitalRead(ledone)==unblocked &&digitalRead(ledtwo)==unblocked &&digitalRead(ledthree)==unblocked &&digitalRead(ledfour)==unblocked &&digitalRead(leddoor)==unblocked)
   {
    in=in +1;
    time0=time1=time2=time3=time4=reset;
    lastattempt=noattempt;
    attempt=noattempt;
   }

   else if (attempt==inattempt && lastattempt!=outattempt) lastattempt=inattempt;

  if (attempt==outattempt && lastattempt!=inattempt &&digitalRead(ledone)==unblocked &&digitalRead(ledtwo)==unblocked &&digitalRead(ledthree)==unblocked &&digitalRead(ledfour)==unblocked &&digitalRead(leddoor)==unblocked )
   {
    out=out +1;
    time0=time1=time2=time3=time4=reset;
    lastattempt=noattempt;
    attempt=noattempt;
   }

   else if (attempt==outattempt && lastattempt!=inattempt) lastattempt=outattempt;


   if ((attempt==outattempt && lastattempt==inattempt)||(attempt==inattempt && lastattempt==outattempt) &&digitalRead(ledone)==unblocked &&digitalRead(ledtwo)==unblocked &&digitalRead(ledthree)==unblocked &&digitalRead(ledfour)==unblocked &&digitalRead(leddoor)==unblocked )
     {
            time0=time1=time2=time3=time4=reset;
           lastattempt=noattempt;
           attempt=noattempt;
     }
}



 


