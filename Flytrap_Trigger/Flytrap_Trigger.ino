//Flytrap Sketch
#include <Servo.h>
#define threshold 100

// Initialize servo
Servo myservo;
int pos = 0;

// Initialize Digital Pin Positions
int light = 13;
int serv = 11;
int air = 12;

// Initialize Sensor Variables
int g [6] = {0, 0, 0, 0, 0, 0};
int old [6];
int oldc [6] = {0, 0, 0, 0, 0, 0};;
int c [6];
int tot;


// Initialize Clock
unsigned long t = millis();
bool timecheck;
bool noisecheck;


void setup()
{
  // Set up pins and servo connection
  pinMode(light, OUTPUT);
  //pinMode(serv, OUTPUT);
  pinMode(air, OUTPUT);
  Serial.begin(115200);
  myservo.attach(serv);

}

void servoOpen()
{
  // Move servo from closed to open position

  for (pos = 45; pos <= 135; pos += 1)
  { // goes from 45 degrees to 135 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void servoClose()
{
  // Move Servo from open to closed position
  for (pos = 135; pos >= 45; pos -= 1)
  { // goes from 45 degrees to 135 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void digPulse(unsigned long millisecs, int pin)
{
  // set digital pin to high
  unsigned long timer = millis();
  bool timing = checktime(timer, millisecs);
  digitalWrite(pin, HIGH);
  while ( timing == false)
  {
    timing = checktime(t, millisecs);

  }
  digitalWrite(pin, LOW);
}


int oncheck(int check1, int check2, int thresh, int prev)
{
  // Check if signal from vibe sensors is high enough to trigger
  int ret;
  int vol = check1 - check2;
	
  if (vol > thresh or prev == 1)
  {
    ret = 1;
  }
  else
  {
    ret = 0;
  }

  return ret;
}

bool noiseTrigger( int* values, int* oldvalues, int trigthresh )
{
	int trig = 0;
	for ( int i = 0; i<= 5; i++)
	{
		if (values[i] - oldvalues[i] == 1)
		{
			trig ++;
		}
	}
	
	if (trig >= trigthresh)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

bool checktime(unsigned long timer, unsigned long s)
{
  // Determine if s time has gone by
  unsigned long tnew = millis() - timer;
  if (tnew >= s)
  {
    return true;
  }
  else
  {
    return false;
  }
}


void loop()
{
  // Set old values to last g values
  for (int a = 0; a <= 5; a++)
  {

    old[a] = g[a];
  }

  //Get new sensor values
  for (int i = 0; i <= 5; i++)
  {
    g[i] = analogRead(i+1);

  }


  // Check for sensor values
  for (int k = 0; k <= 5; k++)
  {
	 
    c[k] = oncheck(g[k], old[k], threshold, c[k]);
  }

	noisecheck = noiseTrigger(c, oldc, 2);
  // Count number of sensor ons
  if (noisecheck == false)
  {
      for (int k = 0; k <= 5; k++)
      {
        oldc[k] = c[k];
       }
	  tot = c[0] + c[1] + c[2] + c[3] + c[4] + c[5];
    Serial.println("Number of jawns Triggered:");
    Serial.println(tot);
  }
  else
  {
    Serial.println("Noise");
	  for (int k = 0; k <= 5; k++)
	  {
		  c[k] = oldc[k];
	  }  
  }

  // Check elapsed time
  timecheck = checktime(t, 10000);

  if (tot >= 3)
  {
    Serial.println("trap was triggered");
    //Triggers activated Trap

    //Deflate balloon
    servoOpen();

    //Wait 10 seconds
    delay(1000);

    //Close Servo
    servoClose();
    //delay(1000);

    //Reinflate balloon
    Serial.println("Pumping the air");
    digPulse(2000, air);

    // clear all the parameters, reset time
    t = millis();
    for ( int a = 0; a <= 5; a++ )
    {
      c[a] = 0;
    }
    Serial.println("Rock hard for you baby");
  }
  else if (timecheck == true)
  {
    //clear all the parameters, reset time
	
    t = millis();

    for ( int z = 0; z <= 5; z++)
    {
      c[z] = 0;
    }

  }
  else
  {
	  delay(200);
  }
}

