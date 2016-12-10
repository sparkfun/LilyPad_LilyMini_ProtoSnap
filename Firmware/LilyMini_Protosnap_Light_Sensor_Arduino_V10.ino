#define LIGHTSENSOR 1
#define BUTTON 2
#define LED3 3
#define LED4 4
#define RED 5
#define GREEN 6
#define BLUE 7

#define TEST_MODE 1
#define BREATHE_MODE 2
#define NIGHTLIGHT_MODE 3
volatile int mode = TEST_MODE;

void buttonPress()
{
  static unsigned long int lastchange = 0L;
  static boolean pressed = false;
  
  for (unsigned int x = 0; x < 48000; x++) __asm__ __volatile__(""); // 1ms

  if (digitalRead(BUTTON) == LOW)
  {
    if (!pressed)
    {
      if ((millis()-lastchange) > 10)
      {
        pressed = true;
        mode = mode + 1;
        if (mode > 3)
        {
          mode = 1;
        }
      }
    }
  }
  else // button is up
  {
    if (pressed)
    {
      if ((millis()-lastchange) > 20)
      {
        pressed = false;
      }
    }
  }
  lastchange = millis();
}

void setup() 
{
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(BUTTON,INPUT_PULLUP);

//  Serial.begin(9600);
  attachInterrupt(BUTTON,buttonPress,CHANGE);
}

void loop() 
{
  int led = 1;
  int brightness = 0;
  int up;
  int lightlevel;
  int speed;
  boolean dark;
  int delayvalue;

  // Make the RGB LED white by turning on all the colors
  digitalWrite(RED,HIGH);
  digitalWrite(GREEN,HIGH);
  digitalWrite(BLUE,HIGH);
  // Turn on all the LEDs
  digitalWrite(LED3,HIGH);
  digitalWrite(LED4,HIGH);
  while (mode == TEST_MODE)
  {
    // do nothing
  }

  digitalWrite(RED,HIGH);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,HIGH);
  for (brightness = 255; brightness > 0; brightness--)
  {
    analogWrite(LED3,brightness);
    analogWrite(LED4,brightness);
    delay(1);
  }
  while (mode == BREATHE_MODE)
  {
    for (led = 1; led <= 2; led++)
    {
      if (mode != BREATHE_MODE) continue;
      for (up = 1; up <= 2; up++)
      {
        if (mode != BREATHE_MODE) continue;
        speed = analogRead(LIGHTSENSOR) * 5 + 300;
        
        if (up == 1)
        {
          for (brightness = 0; brightness <= 255; brightness++)
          {
            if (mode != BREATHE_MODE) continue;

            if (led == 1)
            {
              analogWrite(LED3,brightness);
            }
            else
            {
              analogWrite(LED4,brightness);
            }
            delayMicroseconds(speed);
          }
        }
        else
        {
          for (brightness = 255; brightness >= 0; brightness--)
          {
            if (mode != BREATHE_MODE) continue;

            if (led == 1)
            {
              analogWrite(LED3,brightness);
            }
            else
            {
              analogWrite(LED4,brightness);
            }
            delayMicroseconds(speed);
          }
        }        
      }
    }
  }

  digitalWrite(RED,LOW);
  digitalWrite(BLUE,HIGH);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);
  while (mode == NIGHTLIGHT_MODE)
  {
    // Make the RGB LED blue

    lightlevel = analogRead(LIGHTSENSOR);

//    Serial.println(lightlevel);
    if (lightlevel > 30) dark = false;
    if (lightlevel < 25) dark = true;
    if (dark)
    {
      if (random(1000) == 0)
      {
        for (brightness = 0; brightness <= 255; brightness++)
        {
          analogWrite(LED3,brightness);
          delayMicroseconds(400);
        }
        for (brightness = 255; brightness >= 0; brightness--)
        {
          analogWrite(LED3,brightness);
          delayMicroseconds(400);
        }
      }
      else
      {
          analogWrite(LED3,0);
      }
      
      if (random(2000) == 0)
      {
        for (brightness = 0; brightness <= 255; brightness++)
        {
          analogWrite(LED4,brightness);
          delayMicroseconds(400);
        }
        for (brightness = 255; brightness >= 0; brightness--)
        {
          analogWrite(LED4,brightness);
          delayMicroseconds(400);
        }
      }
      else
      {
          analogWrite(LED4,0);
      }

      digitalWrite(GREEN,LOW);
    }
    else
    {
      digitalWrite(GREEN,HIGH);
    }
  }
}


