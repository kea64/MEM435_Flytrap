//Flytrap Sketch

#include <Servo.h>

#define threshold 600
Servo myservo;
int pos = 0;

int g [6] = {0,0,0,0,0,0};
int old [6];
int c [6];
int tot;


unsigned long t = millis();
bool timecheck;

void setup() 
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(115200);
  myservo.attach(12);
  
}

void sweep()
{
    for (pos = 45; pos <= 135; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 135; pos >= 45; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

int oncheck(int check1, int check2, int thresh)
{
    int ret;
    int vol = check1 - check2;
    
    if (vol > thresh)
    {
        ret = 1;
    }
    else
    {
      ret = 0;
    }
    
    return ret;
}


bool checktime(unsigned long timer, int seconds)
{
  unsigned long tnew = millis() - timer;
  
  if (tnew >= seconds)
  {
    return true;
  }
  else
  {
    return false;
  }
  
}


void release(int t)
{
    // open servo motor and close it after time t
}

void digOn(int millisecs, int pin)
{
    // turn on nano light for t seconds
    unsigned long timer = millis();
    bool timing = checktime(timer,millisecs);
    digitalWrite(pin, HIGH);
    sweep();
    while ( timing == false)
    {
      timing = checktime(t,millisecs);
      
    }

    digitalWrite(13, LOW);

}

void loop() {
 for (int a = 0; a<=5; a++)
 {
    old[a] = g[a];  
 }


  for (int i=0; i<=5; i++)
  {
    g[i] = analogRead(i);
  }
  for (int k =0; k<=5; k++)
  {
    c[k] = oncheck(g[k],old[k], threshold);
  }
  
  tot = c[0]+c[1]+c[2]+c[3]+c[4]+c[5];
  timecheck = checktime(t,4000);

  if (tot >=3)
  {
    
    // close the trap:
        // release()
        // pump()
    digOn(1000, 13);


    // clear all the parameters, reset time
    t = millis();
    for ( int a =0; a<=5; a++ )
    {
        c[a] = 0;
    }
  }
  else if (timecheck == true)
  {
    //clear all the parameters, reset time
    t = millis();
    
    for ( int z =0; z<=5; z++)
    {
        c[z] = 0;
    }
    
  }
  
}
