//LCD initial setup
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensorPin = 33; //pin that reads moisture sensor

const int motorB_1A = 26;
const int motorB_1B = 25;

//standard time for checking moisture
int time_check = 3600000; // 1 hours which should display as 60 min

//setting numbers and values that define moisture sensitivity
int low_moist = 2000; // dry
int high_moist = 1200; //wet

void setup() {
  
  Serial.begin(9600);// Initialize serial communication at 9600 baud rate

  //buttons
  pinMode(27,INPUT_PULLUP); //plus button
  pinMode(12,INPUT_PULLUP); //minus button
  pinMode(19,INPUT_PULLUP); //no water/clear button

  //dial
  pinMode(21,INPUT_PULLUP); //auto/moisture display for user
  pinMode(14,INPUT_PULLUP); //off
  pinMode(32,INPUT_PULLUP); //how often to check moisture
  pinMode(7,INPUT_PULLUP); //(how long) set moisture level
  pinMode(8,INPUT_PULLUP); //start time/start operations

  //LCD setup
  lcd.init();       // initialize the LCD
  lcd.clear();      // clear the LCD display
  lcd.backlight();  // Make sure backlight is on

}

void loop() {
  Serial.println("HI there");
  // Serial.println(analogRead(sensorPin)); // The smaller the value, the higher the soil moisture level
  // delay(500);// Wait for 500 milliseconds before taking the next reading

  //reading current dial position
  int sensorVal14 = digitalRead(14); //off read
  int sensorVal21 = digitalRead(21); //auto/moisture display read
  int sensorVal32 = digitalRead(32); //how often read
  int sensorVal7 = digitalRead(7); //how long/set moisture read
  int sensorVal8 = digitalRead(8); //start time/start operations read

  while (sensorVal14 == LOW) { //off commands, essentially only shows that the device is off
    Serial.println("SUPER OFF");
    lcd.clear();
    lcd.setCursor(1, 0);  //Set cursor to character 1 on line 0
    lcd.print("Current Mode:");
    lcd.setCursor(6, 1);  //Set cursor to character 6 on line 1
    lcd.print("OFF");
    sensorVal14 = digitalRead(14); 
  }

  while (sensorVal21 == LOW) { //auto/moisture display commands
    Serial.println("moisture");
    if(analogRead(sensorPin) < 1700) //Displays that there is a high amount of moisture
    {
     lcd.clear();
      lcd.setCursor(0, 0);  //Set cursor to character 2 on line 0
      lcd.print("Moisture: High");
    }
    if(analogRead(sensorPin) > 2000){ //Displays that there is a low amount of moisture
      lcd.clear();
      lcd.setCursor(0, 0);  //Move cursor to character 2 on line 1
      lcd.print("Moisture: Low");
    }
    sensorVal21 = digitalRead(21);
  }

  while (sensorVal32 == LOW) { //how often commands
    Serial.println("how often");
    Serial.println(time_check/60000);

    int sensorVal12 = digitalRead(27); //plus
    int sensorVal27 = digitalRead(12); //minus
    int sensorVal19 = digitalRead(19); //clear

    if (sensorVal12 == LOW){ //plus button adds 5 min between check
      time_check = time_check + 300000;
      delay(500);
    }
    if (sensorVal27 == LOW){ //minus button reduces 5 min between check
      time_check = time_check - 300000;
      delay(500);
    }
    if (sensorVal19 == LOW){ //clear button sets value to standard amount
      time_check = 3600000;
      delay(500);
    }

    //Displays the current time between checks in minutes
    lcd.setCursor(0, 0);  //Set cursor to character 0 on line 0
    lcd.print("Minutes:");
    lcd.setCursor(6, 1);  //Set cursor to character 0 on line 0
    lcd.print(time_check/60000);
    
    sensorVal32 = digitalRead(32);
  }

  while (sensorVal7 == LOW) { //how long/set moisture commands
    Serial.println("how long");
    int sensorVal12 = digitalRead(27); //plus
    int sensorVal27 = digitalRead(12); //minus
    int sensorVal19 = digitalRead(19); //clear
    
    if (sensorVal12 == LOW){ //plus button makes it more sensitive ()
      low_moist = low_moist - 50; // dry
      high_moist = high_moist + 50; //wet
      delay(500);
    }
    if (sensorVal27 == LOW){ //minus button makes it less sensitive ()
      low_moist = low_moist + 50; // dry
      high_moist = high_moist - 50; //wet
      delay(500);
    }
    if (sensorVal19 == LOW){ //clear button sets value to standard amount
      low_moist = 2000; // dry
      high_moist = 1200; //wet
      delay(500);
    }

    if (low_moist == 1200) & (high_moist == 2000){
      lcd.setCursor(0, 0);  //Set cursor to character 0 on line 0
      lcd.print("moisture");
      lcd.setCursor(0, 1);  //Set cursor to character 0 on line 0
      lcd.print("sensitivity: med");
    }

    if (low_moist <= 800) & (high_moist <= 2400){
      lcd.setCursor(0, 0);  //Set cursor to character 0 on line 0
      lcd.print("moisture");
      lcd.setCursor(0, 1);  //Set cursor to character 0 on line 0
      lcd.print("sensitivity: low");
    }

    if (low_moist >= 1500) & (high_moist >= 1700){
      lcd.setCursor(0, 0);  //Set cursor to character 0 on line 0
      lcd.print("moisture");
      lcd.setCursor(0, 1);  //Set cursor to character 0 on line 0
      lcd.print("sensitivity: hi");

    sensorVal7 = digitalRead(7);
  }

  while (sensorVal8 == LOW) { //start time/start operations commands
    Serial.println("start time");
    // delay(time_check);
    delay(1000);
  
    if (low_moist<(analogRead(sensorPin))) { //if sensor detects low moisture, spray water
      Serial.println("shooting water");
      Serial.println((analogRead(sensorPin)));
      digitalWrite(motorB_1A, HIGH);
      digitalWrite(motorB_1B, LOW);
      delay(5000);
    }

    if(high_moist>(analogRead(sensorPin))) { //if sensor detects high moisture, stop water
      Serial.println("stoppy water");
      Serial.println((analogRead(sensorPin)));
      digitalWrite(motorB_1A, LOW);
      digitalWrite(motorB_1B, HIGH);
      delay(5000);
    }

    //LCD should display current stats 
    lcd.setCursor(0, 0);  //Set cursor to character 0 on line 0
    lcd.print("uhhh");

    sensorVal8 = digitalRead(8);
  }

}
