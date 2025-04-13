//LCD initial setup
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensorPin = 14; //pin that reads moisture sensor

const int motorB_1A = 10;
const int motorB_1B = 9;

//standard time for checking moisture
int time_check = 3600000; // 1 hours which should display as 60 min

//setting numbers and values that define moisture sensitivity
int low_moist = 2000; // dry
int high_moist = 1200; //wet

void setup() {
  Serial.begin(9600);// Initialize serial communication at 9600 baud rate

  //buttons
  pinMode(12,INPUT_PULLUP); //plus button
  pinMode(27,INPUT_PULLUP); //minus button
  pinMode(34,INPUT_PULLUP); //clear button

  //dial
  pinMode(37,INPUT_PULLUP); //auto/moisture display for user
  pinMode(36,INPUT_PULLUP); //off
  pinMode(39,INPUT_PULLUP); //how often to check moisture
  pinMode(7,INPUT_PULLUP); //(how long) set moisture level
  pinMode(8,INPUT_PULLUP); //start time/start operations

  //LCD setup
  lcd.init();       // initialize the LCD
  lcd.clear();      // clear the LCD display
  lcd.backlight();  // Make sure backlight is on

}

void loop() {

  // Serial.println(analogRead(sensorPin)); // The smaller the value, the higher the soil moisture level
  // delay(500);// Wait for 500 milliseconds before taking the next reading

  //reading current dial position
  int sensorVal36 = digitalRead(36); //off read
  int sensorVal37 = digitalRead(37); //auto/moisture display read
  int sensorVal39 = digitalRead(39); //how often read
  int sensorVal7 = digitalRead(7); //how long/set moisture read
  int sensorVal8 = digitalRead(8); //start time/start operations read

  while (sensorVal36 == LOW) { //off commands, essentially only shows 
    lcd.clear();
    lcd.setCursor(1, 0);  //Set cursor to character 1 on line 0
    lcd.print("Current Mode:");
    lcd.setCursor(6, 1);  //Set cursor to character 6 on line 1
    lcd.print("OFF");
    sensorVal36 = digitalRead(36); 
  }

  while (sensorVal37 == LOW) { //auto/moisture display commands
    if(analogRead(sensorPin) < 1700)
    {
     lcd.clear();
      lcd.setCursor(0, 0);  //Set cursor to character 2 on line 0
      lcd.print("Moisture: High");
    }
    if(analogRead(sensorPin) > 2000){
      lcd.clear();
      lcd.setCursor(0, 0);  //Move cursor to character 2 on line 1
      lcd.print("Moisture: Low");
    }
  }

  while (sensorVal39 == LOW) { //how often commands
    int sensorVal12 = digitalRead(12); //plus
    int sensorVal27 = digitalRead(27); //minus
    int sensorVal34 = digitalRead(34); //clear

    if (sensorVal12 == LOW){ //plus button adds 5 min between check
      time_check = time_check + 300000;
    }
    if (sensorVal27 == LOW){ //minus button reduces 5 min between check
      time_check = time_check - 300000;
    }
    if (sensorVal34 == LOW){ //clear button sets value to standard amount
      time_check = 3600000;
    }
    sensorVal39 = digitalRead(39);
    //lcd code goes here
  }

  while (sensorVal7 == LOW) { //how long/set moisture commands
    int sensorVal12 = digitalRead(12); //plus
    int sensorVal27 = digitalRead(27); //minus
    int sensorVal34 = digitalRead(34); //clear
    
    if (sensorVal12 == LOW){ //plus button makes it more sensitive ()
      low_moist = low_moist - 50; // dry
      high_moist = high_moist + 50; //wet
    }
    if (sensorVal27 == LOW){ //minus button makes it less sensitive ()
      low_moist = low_moist + 50; // dry
      high_moist = high_moist - 50; //wet
    }
    if (sensorVal34 == LOW){ //clear button sets value to standard amount
      low_moist = 2000; // dry
      high_moist = 1200; //wet
    }
    sensorVal7 = digitalRead(7);
  }

  while (sensorVal8 == LOW) { //start time/start operations commands
    // delay(time_check);
    delay(10000);

    if (low_moist<(analogRead(sensorPin))) { //if sensor detects low moisture, spray water
      digitalWrite(motorB_1A, LOW);
      digitalWrite(motorB_1B, HIGH);
    }

    if(high_moist>(analogRead(sensorPin))) { //if sensor detects high moisture, stop water
      digitalWrite(motorB_1A, HIGH);
      digitalWrite(motorB_1B, LOW);
    }

    sensorVal8 = digitalRead(8);
  }

}
