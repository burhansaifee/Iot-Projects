// import the Liquid Crystal display library
#include <LiquidCrystal.h>

// declare variables for keeping track of time and the number of study sessions
int seconds = 0;
int minutes = 0;
int count = 0;

// declare variables for the length of each period and the number of sessions before a longer break. Change these to customize your timer
const int study_minutes = 5;
const int short_break_minutes = 1;
const int long_break_minutes = 2;
const int repeats = 4;

// a variable used to time the breaks later
int break_duration;

// declare pins used for hardware
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int led1 = 6;
const int led2 = 7;
const int button = 8;
const int buzzer = 10;  // Add the buzzer pin

// melody notes and durations
int melody[] = {
  262, 294, 330, 349, 392, 440, 494, 523
}; // C4, D4, E4, F4, G4, A4, B4, C5
int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 4, 4
}; // Quarter notes

void setup()
{
  // Set up the number of columns and rows on the LCD.
  lcd.begin(16, 2); 
  
  // set LED pins as outputs, button pin as input, and buzzer pin as output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(button, INPUT);
  pinMode(buzzer, OUTPUT);  // Set buzzer pin as output

  // display initial message to user
  lcd.print("Press button");
  lcd.setCursor(0, 1);
  lcd.print("to start");

  // wait for button press to start
  while(digitalRead(button) == LOW) {
  }
}

void loop()
{
  count = 0;

  while (count < repeats) { 
    // alternate timing between study and breaks

    // print message and set LEDs for study time
    lcd.clear();
    lcd.print("Study time!");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    playMelody();  // Play the melody at the start of study time
    lcd.setCursor(0, 1);

    // reset minutes and seconds to zero
    seconds = 0;
    minutes = 0;

    // count up and display the timer during study period
    while (minutes < study_minutes) { 
      seconds = 0;

      while (seconds < 60) {
        lcd.setCursor(0, 1);

        if (minutes < 10) {  
          lcd.print("0");
        }

        lcd.print(minutes);
        lcd.print(":");

        if (seconds < 10) {
          lcd.print("0");
        }

        lcd.print(seconds);
        delay(1000);
        seconds++;
      }

      minutes++;
    }
    
    // now repeat the process for a study break
    lcd.clear();
    lcd.setCursor(0, 0);
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);

    if (count == (repeats - 1)) {
      break_duration = long_break_minutes;
      lcd.print("Long break!");
    } else {  
      break_duration = short_break_minutes;
      lcd.print("Short break!");
    }

    playMelody();  // Play the melody at the start of break time
    lcd.setCursor(0, 1);
    seconds = 0;
    minutes = 0;

    while (minutes < break_duration) {
      seconds = 0;

      while (seconds < 60) {
        lcd.setCursor(0, 1);

        if (minutes < 10) {
          lcd.print("0");
        }

        lcd.print(minutes);
        lcd.print(":");

        if (seconds < 10) {
          lcd.print("0");
        }

        lcd.print(seconds);
        delay(1000);
        seconds++;
      }

      minutes++;
    }

    count++;  // increment the counter for the number of study sessions
  }
}

void playMelody() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzer, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzer);
  }
}
