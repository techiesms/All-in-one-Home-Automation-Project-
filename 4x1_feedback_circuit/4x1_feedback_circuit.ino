/*************************************************************
  This is the code for All in one Home Automation project by techiesms.

  Features of this project:

  - Using this project, you can control 4 AC appliances and control speed of 1 AC fan.

  - It works with Internet using Blynk app and also without internet using manual switches
   and manual fan regulator.

  - You can also get real-time feedback of all the appliances on to your Blynk app.

  - Using IFTTT, you can control all the applicanes using Google Assistant and Alexa.

  A full tutorial video on this project is uploaded on my YouTube channel.
  Have a look over it to know more

  techiesms YouTube Channel - https://www.youtube.com/techiesms

  Code written on - 7.09.20

  Download latest Blynk library here:
  https://github.com/blynkkk/blynk-library/releases/latest

          techiesms
    explore | learn | share


 *************************************************************/

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;


#define DEBUG_SW 1


// Pins of Fan Regulator Knob
#define s1  27
#define s2  14
#define s3  12
#define s4  13


// Pins of Switches
#define S5 32
#define S6 35
#define S7 34
#define S8 39

// Pins of Relay (Appliances Control)
#define R5 15
#define R6 2
#define R7 4
#define R8 22

// Pins of Relay (Fan Speed Control)
#define Speed1 21
#define Speed2 19
#define Speed4 18

// By default the mode is with_internet
int MODE = 0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "AUTH_TOKEN";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID";
char pass[] = "PASS";

bool speed1_flag = 1;
bool speed2_flag = 1;
bool speed3_flag = 1;
bool speed4_flag = 1;
bool speed0_flag = 1;

int switch_ON_Flag1_previous_I = 0;
int switch_ON_Flag2_previous_I = 0;
int switch_ON_Flag3_previous_I = 0;
int switch_ON_Flag4_previous_I = 0;


BLYNK_WRITE(V0)
{
  int fan_speed = param.asInt(); // assigning incoming value from pin V1 to a variable
  if (fan_speed == 0)
  {
    speed0();
  }
  if (fan_speed == 1)
  {
    speed1();
  }
  if (fan_speed == 2)
  {
    speed2();
  }
  if (fan_speed == 3)
  {
    speed3();
  }
  if (fan_speed == 4)
  {
    speed4();
  }
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  digitalWrite(R5, pinValue);
  // process received value
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V2 to a variable
  digitalWrite(R6, pinValue);
  // process received value
}

BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V3 to a variable
  digitalWrite(R7, pinValue);
  // process received value
}

BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V4 to a variable
  digitalWrite(R8, pinValue);
  // process received value
}



void setup()
{
  // put your setup code here, to run once:
  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT_PULLUP);
  pinMode(s4, INPUT);
  pinMode(S5, INPUT);
  pinMode(S6, INPUT);
  pinMode(S7, INPUT);
  pinMode(S8, INPUT);

  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R8, OUTPUT);
  pinMode(Speed1, OUTPUT);
  pinMode(Speed2, OUTPUT);
  pinMode(Speed4, OUTPUT);

  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  timer.setInterval(3000L, checkBlynk); // check if connected to Blynk server every 3 seconds

  Blynk.config(auth);//, ssid, pass);
}


void loop()
{

  if (WiFi.status() != WL_CONNECTED)
  {
    if (DEBUG_SW) Serial.println("Not Connected");
  }
  else
  {
    if (DEBUG_SW) Serial.println(" Connected");
    Blynk.run();
  }


  timer.run(); // Initiates SimpleTimer

  if (MODE == 0)
    with_internet();
  else
    without_internet();
  // put your main code here, to run repeatedly:
}


void with_internet()
{

  // FOR FAN
  if (digitalRead(s1) == LOW && speed1_flag == 1)
  {
    speed1();
    Blynk.virtualWrite(V0, 1);
    speed1_flag = 0;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 1;


  }
  if (digitalRead(s2) == LOW && digitalRead(s3) == HIGH && speed2_flag == 1)
  {
    speed2();
    Blynk.virtualWrite(V0, 2);
    speed1_flag = 1;
    speed2_flag = 0;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 1;

  }
  if (digitalRead(s2) == LOW && digitalRead(s3) == LOW && speed3_flag == 1)
  {
    speed3();
    Blynk.virtualWrite(V0, 3);
    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 0;
    speed4_flag = 1;
    speed0_flag = 1;
  }
  if (digitalRead(s4) == LOW  && speed4_flag == 1)
  {
    speed4();
    Blynk.virtualWrite(V0, 4);
    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 0;
    speed0_flag = 1;
  }
  if (digitalRead(s1) == HIGH && digitalRead(s2) == HIGH && digitalRead(s3) == HIGH && digitalRead(s4) == HIGH  && speed0_flag == 1)
  {
    speed0();
    Blynk.virtualWrite(V0, 0);
    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 0;
  }


  // FOR SWITCH
  if (digitalRead(S5) == LOW)
  {
    if (switch_ON_Flag1_previous_I == 0 )
    {
      digitalWrite(R5, HIGH);
      if (DEBUG_SW) Serial.println("Relay1- ON");
      Blynk.virtualWrite(V1, 1);
      switch_ON_Flag1_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch1 -ON");

  }
  if (digitalRead(S5) == HIGH )
  {
    if (switch_ON_Flag1_previous_I == 1)
    {
      digitalWrite(R5, LOW);
      if (DEBUG_SW) Serial.println("Relay1 OFF");
      Blynk.virtualWrite(V1, 0);
      switch_ON_Flag1_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch1 OFF");
  }


  if (digitalRead(S6) == LOW)
  {
    if (switch_ON_Flag2_previous_I == 0 )
    {
      digitalWrite(R6, HIGH);
      if (DEBUG_SW)  Serial.println("Relay2- ON");
      Blynk.virtualWrite(V2, 1);
      switch_ON_Flag2_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch2 -ON");

  }
  if (digitalRead(S6) == HIGH )
  {
    if (switch_ON_Flag2_previous_I == 1)
    {
      digitalWrite(R6, LOW);
      if (DEBUG_SW) Serial.println("Relay2 OFF");
      Blynk.virtualWrite(V2, 0);
      switch_ON_Flag2_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch2 OFF");
    //delay(200);
  }

  if (digitalRead(S7) == LOW)
  {
    if (switch_ON_Flag3_previous_I == 0 )
    {
      digitalWrite(R7, HIGH);
      if (DEBUG_SW) Serial.println("Relay3- ON");
      Blynk.virtualWrite(V3, 1);
      switch_ON_Flag3_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch3 -ON");

  }
  if (digitalRead(S7) == HIGH )
  {
    if (switch_ON_Flag3_previous_I == 1)
    {
      digitalWrite(R7, LOW);
      if (DEBUG_SW) Serial.println("Relay3 OFF");
      Blynk.virtualWrite(V3, 0);
      switch_ON_Flag3_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch3 OFF");
    //delay(200);
  }

  if (digitalRead(S8) == LOW)
  {
    if (switch_ON_Flag4_previous_I == 0 )
    {
      digitalWrite(R8, HIGH);
      if (DEBUG_SW) Serial.println("Relay4- ON");
      Blynk.virtualWrite(V4, 1);
      switch_ON_Flag4_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch4 -ON");

  }
  if (digitalRead(S8) == HIGH )
  {
    if (switch_ON_Flag4_previous_I == 1)
    {
      digitalWrite(R8, LOW);
      if (DEBUG_SW) Serial.println("Relay4 OFF");
      Blynk.virtualWrite(V4, 0);
      switch_ON_Flag4_previous_I = 0;
    }
    if (DEBUG_SW) Serial.println("Switch4 OFF");
    //delay(200);
  }


}


void without_internet()
{

  // FOR FAN
  if (digitalRead(s1) == LOW && speed1_flag == 1)
  {
    speed1();

    speed1_flag = 0;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 1;


  }
  if (digitalRead(s2) == LOW && digitalRead(s3) == HIGH && speed2_flag == 1)
  {
    speed2();

    speed1_flag = 1;
    speed2_flag = 0;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 1;

  }
  if (digitalRead(s2) == LOW && digitalRead(s3) == LOW && speed3_flag == 1)
  {
    speed3();

    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 0;
    speed4_flag = 1;
    speed0_flag = 1;
  }
  if (digitalRead(s4) == LOW  && speed4_flag == 1)
  {
    speed4();

    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 0;
    speed0_flag = 1;
  }
  if (digitalRead(s1) == HIGH && digitalRead(s2) == HIGH && digitalRead(s3) == HIGH && digitalRead(s4) == HIGH  && speed0_flag == 1)
  {
    speed0();


    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 0;
  }


  // FOR SWITCH
  digitalWrite(R5, !digitalRead(S5));
  digitalWrite(R6, !digitalRead(S6));
  digitalWrite(R7, !digitalRead(S7));
  digitalWrite(R8, !digitalRead(S8));
}



// Fan Speed Control

void speed0()
{
  //All Relays Off - Fan at speed 0
  if (DEBUG_SW)Serial.println("SPEED 0");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);

}

void speed1()
{
  //Speed1 Relay On - Fan at speed 1
  if (DEBUG_SW)Serial.println("SPEED 1");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed1, HIGH);
}

void speed2()
{
  //Speed2 Relay On - Fan at speed 2
  if (DEBUG_SW)Serial.println("SPEED 2");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed2, HIGH);
}

void speed3()
{
  //Speed1 & Speed2 Relays On - Fan at speed 3
  if (DEBUG_SW)Serial.println("SPEED 3");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed1, HIGH);
  digitalWrite(Speed2, HIGH);

}

void speed4()
{
  //Speed4 Relay On - Fan at speed 4
  if (DEBUG_SW)Serial.println("SPEED 4");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed4, HIGH);
}


//Check Blynk Connectivity every 3 seconds

void checkBlynk()
{
  bool isconnected = Blynk.connected();
  if (isconnected == false)
  {
    MODE = 1;
  }
  if (isconnected == true)
  {
    MODE = 0;
  }
}
