#include <LiquidCrystal.h>
#include <Servo.h>

// This sketch uses an LCD display with buttons to control a PWM motor controller.

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

Servo MotorController;  // Create the Servo object for the motor controller

const int MOTORCONTROLLER = 2; // Define the pin used for the motor controller
const int BUTTONS = A0; // The Analog port reading the button values

// Set Button value constants
const int btnNONE = 0;
const int btnSELECT = 1;
const int btnLEFT = 2;
const int btnRIGHT = 3;
const int btnUP = 4;
const int btnDOWN = 5;

// Set the PWM range
const int pwmMIN = 1000;
const int pwmCENTER = 1500;
const int pwmMAX = 2000;


// Set the initial servo position and motor speed values
int Position = pwmCENTER;
int MotorSpeed = 0;


// flag used determine if there was a change in the keypresses
int lastbutton = 0;

// Function to read the button values
int read_LCD_buttons() {
  int buttonval = analogRead(BUTTONS);

  // Default value if no button press was detected
  int returnval = btnNONE;

  if (buttonval < 800) returnval = btnSELECT;
  if (buttonval < 600) returnval = btnLEFT;
  if (buttonval < 400) returnval = btnDOWN;
  if (buttonval < 200) returnval = btnUP;
  if (buttonval < 100) returnval = btnRIGHT;

  return returnval;

}

// Setup function
void setup() {

  // Setup the LCD display size (16 characters, two lines)
  lcd.begin(16, 2);

  // Setup teh LCD brightness
  analogWrite(10, 100);

  // Attach the servo and set the PWM limits
  MotorController.attach(2, pwmMIN, pwmMAX);
  MotorController.writeMicroseconds(Position);

  // Show the starting values
  lcd.print(Position);
  lcd.print(" ");
  lcd.print(MotorSpeed);

}

// Program loop
void loop() {

  // Read the current buttons status
  int button = read_LCD_buttons();

  // If there was a change in the button state, make a change.
  if (button != lastbutton)
  {
    // Clear the LCD screen
    lcd.clear();

    // To avoid transient readings, make sure we read a btnNONE before processing a new press
    if (lastbutton == btnNONE)
    {
      // Switch on the value of the button pressed
      switch (button)
      {
        case btnNONE:
          // Do nothing.
          break;
        case btnSELECT:
          // The Select button was pressed, reset controller to 0
          lcd.setCursor(0, 1);
          lcd.print("Select");
          MotorSpeed = 0;
          break;
        case btnLEFT:
          // Left cursor button was pressed, reduce motor speed by 10%
          lcd.setCursor(0, 1);
          lcd.print("Left");
          MotorSpeed -= 10;
          break;
        case btnRIGHT:
          // Right cursor button was pressed, increase motor speed by 10%
          lcd.setCursor(0, 1);
          lcd.print("Right");
          MotorSpeed += 10;
          break;
        case btnUP:
          // Up cursor button was pressed, increase motor speed by 1%
          lcd.setCursor(0, 1);
          lcd.print("Up");
          MotorSpeed += 1;
          break;
        case btnDOWN:
          // Down cursor button was pressed, decrease motor speed by 1%
          lcd.setCursor(0, 1);
          lcd.print("Down");
          MotorSpeed -= 1;
          break;
      }
      // End Switch
    }
    
    // Make sure we don't exceed our minimum and maximum values
    if (MotorSpeed > 100) MotorSpeed = 100;
    if (MotorSpeed < -100) MotorSpeed = -100;

    // Map the selected percentage to the PWM cycle
    Position = map(MotorSpeed, -100, 100, pwmMIN, pwmMAX);

    // Show new values
    lcd.setCursor(0, 0);
    lcd.print(Position);
    lcd.print(" ");
    lcd.print(MotorSpeed);

    // Commit the new values to the MotorController
    MotorController.writeMicroseconds(Position);

    lastbutton = button;

    //delay(100);

  }


  // End Function
}
