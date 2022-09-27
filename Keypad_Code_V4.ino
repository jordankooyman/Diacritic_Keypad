// Diacritic Keypad program by Jordan Kooyman
// Completed 9/27/22
// Written for Arduino Pro Micro/Arduino Leonardo boards
// Intended for use with 17 buttons and a switch, all connected to the arduino via chained shift registers
// Uses 1 RGB LED (common negative) for status indication at runtime
// More information can be found at: https://github.com/jordankooyman/Diacritic_Keypad

// Only 1 sample output is provided using demo numbers, all the output cases (starting at line 180)
//   will need to be written manually once key layout is determined

// Basic Program Configuration:

//Debug Mode Compiler Controls
#define TestMode 1 // Comment out to disable test mode and enable normal keyboard output
#define ExtendedTestMode 1 // Comment out to disable per button press debug information

//Define where each pin is connected to the Ardunio-compatible board
#define PIN_CLOCK 14    // Shift Register Clock Pin
#define PIN_LOADDATA 16 // Shift Register Load Data Pin
#define PIN_DATAIN 15   // Shift Register Data Output Pin
                        // Shift Register Enable Pin -> connect to VCC (5V) to always enable
#define PIN_RED 6       // RGB LED Red leg
#define PIN_GREEN 7     // RGB LED Green leg
#define PIN_BLUE 8      // RGB LED Blue leg

// Keyboard ID/index layout: - Verify using extended test mode (dependent on wiring)
//    0     9   2  17  10
//    1     8   3  16  11
//          7   4  15  12
//          6   5  14  13
// Keyboard Prime Mapping:   - Rearrange as needed such that prime numbers are increasing as key id increases
//         19   2  53  23
//         17   3  47  29
//         13   5  43  31
//         11   7  41  37
// Keyboard Key Mapping:     - Fill in after desired key layout has been determined
// Shift    ?   ?   ?   ?
//  Mode    ?   ?   ?   ?
//          ?   ?   ?   ?
//          ?   ?   ?   ?

const int primeTable[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53}; // Used to decode combination in switch statement
  // Corresponding keys:  ?,?,?,?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
int buttonState[18] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; 
// Corresponding keys:  ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?

// Debugging Mode Indicator Outputs
#define WINDOWS_OUTPUT "Windows: "
#define LINUX_OUTPUT "Linux: "

// End of Basic Program Configuration

// Advanced Program Configuration, No Changes Recommended

// Define numpad constants for windows altcode usage - https://www.reddit.com/r/arduino/comments/c3cl41/keyboard_library_using_alt_keycodes_trouble/
// Custom data type to simplify handling of numpad inputs for Windows altcode combinations
enum NumPad {
  N_SKIP = -1,    // Skip given key for output
  N_1 = 0xE1, // Keypad 1 and End
  N_2 = 0xE2, // Keypad 2 and Down Arrow
  N_3 = 0xE3, // Keypad 3 and PageDn
  N_4 = 0xE4, // Keypad 4 and Left Arrow
  N_5 = 0xE5, // Keypad 5
  N_6 = 0xE6, // Keypad 6 and Right Arrow
  N_7 = 0xE7, // Keypad 7 and Home
  N_8 = 0xE8, // Keypad 8 and Up Arrow
  N_9 = 0xE9, // Keypad 9 and Page Up
  N_0 = 0xEA  // Keypad 0 and Insert
};


#ifndef TestMode
#include <Keyboard.h> // https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
#endif

bool Shift = false;
bool OS = false; //False: Windows  |  True: Linux
int value;
int total;
unsigned long clock;

// Windows altcodes, with support for 3 or four digits
void pressWindows(String output, NumPad key1, NumPad key2, NumPad key3, NumPad key4 = N_SKIP); 
// Linux altcode handled with unicode hex values (0-9 and A-F, case sensitive)
void pressLinux(String output, char key1, char key2, char key3, char key4);

void setup() { //Initialize pins as i or o for test-proofing
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_LOADDATA, OUTPUT);
  pinMode(PIN_DATAIN, INPUT);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  #ifdef TestMode
  Serial.begin(9600);
  #else
  Keyboard.begin();
  #endif
}

// End of Advanced Program Configuration

void loop() {
  value = -1;
  digitalWrite(PIN_CLOCK, 1); //Initializes pins to low each cycle to avoid false positives
  digitalWrite(PIN_RED, 0);
  digitalWrite(PIN_GREEN, 0);
  digitalWrite(PIN_BLUE, 1);
  digitalWrite(PIN_LOADDATA, 0); // Load data into shift registers
  digitalWrite(PIN_CLOCK, 0);
  digitalWrite(PIN_CLOCK, 1);
  digitalWrite(PIN_LOADDATA, 1);
  
  //Update LED Status to default (red) and reset running total to 1 if timer expires (no longer greater than system timer)
  if (clock <= millis())
  {
    digitalWrite(PIN_RED, 1);
    digitalWrite(PIN_GREEN, 0);
    digitalWrite(PIN_BLUE, 0);
    total = 1;
    #ifdef TestMode
    Serial.println("LED Red");
    #endif
  }

  for (int button = 0; button < 18; button++) { // Load in 18 bits of data from the 3 shift registers connected as 1 24 bit register
    if (button == 2) { // skip the unused pins on the first shift register
      digitalWrite(PIN_CLOCK, 0);
      digitalWrite(PIN_CLOCK, 1); 
      digitalWrite(PIN_CLOCK, 0);
      digitalWrite(PIN_CLOCK, 1);
      digitalWrite(PIN_CLOCK, 0);
      digitalWrite(PIN_CLOCK, 1);
      digitalWrite(PIN_CLOCK, 0);
      digitalWrite(PIN_CLOCK, 1);
      digitalWrite(PIN_CLOCK, 0);
      digitalWrite(PIN_CLOCK, 1);
      digitalWrite(PIN_CLOCK, 0);
      digitalWrite(PIN_CLOCK, 1);
    }
    // Read in the next bit of data from the shift registers
    value = digitalRead(PIN_DATAIN);

    //Determine if the button state has changed, if so store the change and optionally output debug info to serial monitor
    if(buttonState[button] != value)
  { //Potentially add a flag to send to processing
      buttonState[button] = value;
    if (button == 0) Shift = value; // If shift button state changes, update the shift flag
    else if (button == 1) OS = value; // If the OS switch state changes, update the os flag
    else if (value) // If a regular button is pressed, multiply the running total by its prime number and update LED
    {
      total *= primeTable[button-2]; 
      clock = millis() + 1000; // Add 1,000 milliseconds to the timer and set LED to blue
      digitalWrite(PIN_RED, 0);
      digitalWrite(PIN_GREEN, 0);
      digitalWrite(PIN_BLUE, 1);
      #ifdef TestMode
      Serial.println("LED Blue");
      #endif
    }
  // Print out extra debug information if enabled
  #ifdef ExtendedTestMode
    Serial.print("Button Index: ");
    Serial.print(button);
    Serial.print("    Value: ");
    Serial.println(value);
  #endif
    }

    //Pulse the clock to get next bit of data
    digitalWrite(PIN_CLOCK, 0);
    digitalWrite(PIN_CLOCK, 1);
  }
  
  
  //Start processing the data here  
  bool validCombo = false;


  // Every combination needs to be added within the following statement (inside the { })
  // Each different combination needs a new case, but uppercase and lowercase are handled in the same block
  // It is recommended to copy everything starting at 'case (...' to '...break; // end ...' and paste as needed, changing each subsequent block for the next combination
  
  switch (total)
  {
  case (19*17): // begin ae - 19 * 17 [Example case, numbers likely need to be changed]
    if(Shift)       // Uppercase
      if (OS) // Linux
        pressLinux("AE - 00C6", '0', '0', 'C', '6');
      else    // Windows
        pressWindows("AE - 146", N_1, N_4, N_6);
    else            // Lowercase
      if (OS) // Linux
        pressLinux("ae - 00E6", '0', '0', 'E', '6');
      else    // Windows
        pressWindows("ae - 145", N_1, N_4, N_5);
    validCombo = true; // Update Status LED after logic
    break; // end ae - 19 * 17 [Example case, numbers likely need to be changed]
  case 2: // Keep logic going here (remove this line)
    break; // Remove this line as well
  }


  
  
  if(validCombo) // Update status LED if combo found
  {
    total = 1; // Reset running total to 1 to prevent sending repeated inputs
    clock = millis() + 10000; // Add 10,000 milliseconds (10 seconds) to the timer and set LED to green
    digitalWrite(PIN_RED, 0);
    digitalWrite(PIN_GREEN, 1);
    digitalWrite(PIN_BLUE, 0);
    #ifdef TestMode
    Serial.println("LED Green");
    #endif
  }

  delay(100); // Pause execution for 100 milliseconds to keep program running at a reasonable speed (1000/100 = runs nearly 10 times per second)
}


/**
* Processes given altcode to output to Windows through keyboard emulation, with support for 3 or 4 digit altcodes
* Additionally, has a debug mode that can be enabled globally at the top of the code
* @param output - character combination altcode represents, for function debugging and readability
* @param key1 - first number in altcode sequence
* @param key2 - second number in altcode sequence
* @param key3 - third number in altcode sequence
* @param key4 - (optional) fourth number in altcode sequence
* @return void
*/
void pressWindows(String output, NumPad key1, NumPad key2, NumPad key3, NumPad key4) {
  #ifdef TestMode
  Serial.println(WINDOWS_OUTPUT + output);
  #else // Keyboard Output
  Keyboard.press(KEY_RIGHT_ALT);
  
  if (key1 != KEY_SKIP)
    Keyboard.write(key1);
  if (key2 != KEY_SKIP)
    Keyboard.write(key2);
  if (key3 != KEY_SKIP)
    Keyboard.write(key3);
  if (key4 != KEY_SKIP)
    Keyboard.write(key4);
  
  Keyboard.releaseAll();
  #endif
  return;
}


/**
* Processes given hexadecimal unicode to output to many Linux distros through keyboard emulation
* Additionally, has a debug mode that can be enabled globally at the top of the code
* @param output - character combination unicode represents, for function debugging and readability
* @param key1 - first number in hexadecimal unicode sequence
* @param key2 - second number in hexadecimal unicode sequence
* @param key3 - third number in hexadecimal unicode sequence
* @param key4 - fourth number in hexadecimal unicode sequence
* @return void
*/
void pressLinux(String output, char key1, char key2, char key3, char key4) {
  #ifdef TestMode
  Serial.println(LINUX_OUTPUT + output);
  #else // Keyboard Output
  Keyboard.press(KEY_RIGHT_SHIFT);
  Keyboard.press(KEY_RIGHT_CTRL);
  Keyboard.press(KEY_U);
  
  Keyboard.write(key1);
  Keyboard.write(key2);
  Keyboard.write(key3);
  Keyboard.write(key4);
  
  Keyboard.press(KEY_RETURN); // Enter key to close combination
  Keyboard.releaseAll();
  #endif
}
