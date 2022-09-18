#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

const int WHITEBUTTON = 10;
const int BLACKBUTTON = 11;
const int SETBUTTON = 9;
const int CURSORMINUTES = 7;
const int CURSORSECONDS = 10;
const int NUMOFMODES = 5;

int TimeModes[NUMOFMODES] = {1, 3, 5, 10, 15};
unsigned long StartTime = 0;
int Mode = 3;
int Time[2][2] = {{TimeModes[Mode], 0}, {TimeModes[Mode], 0}}; //first and second player time: minutes, seconds
int Counts = 0;


void setup() {
  lcd.begin(16,2);
  lcd.clear();
  startDisplay();
  pinMode(WHITEBUTTON, INPUT_PULLUP);
  pinMode(BLACKBUTTON, INPUT_PULLUP);
  pinMode(SETBUTTON, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BLACKBUTTON) == LOW){
    StartTime = millis();
    while(digitalRead(WHITEBUTTON) == HIGH){
      if ((millis() - StartTime) >= 1000){
        StartTime = millis();
        updateTime(1);
      }
      if (digitalRead(SETBUTTON) == LOW){
        resetClock();
        delay(500);
        break;
      }
    }
  }
  if (digitalRead(WHITEBUTTON) == LOW){
    StartTime = millis();
    while(digitalRead(BLACKBUTTON) == HIGH){
      if ((millis() - StartTime) >= 1000){
        StartTime = millis();
        updateTime(2);
      }
      if (digitalRead(SETBUTTON) == LOW){
        resetClock();
        delay(500);
        break;
      }
    }
  }

  if(digitalRead(SETBUTTON) == LOW){
    delay(1000);
    Counts = Counts + 1;
    if (Counts == 2){
      while(digitalRead(SETBUTTON) == LOW){}
      setTime();
      Counts = 0;
    }
  }
  if(digitalRead(SETBUTTON) == HIGH){
    Counts = 0;
  }
}

/* functions */

void  updateTime(int player){
/* Updates time by 1 second for given player 1 or 2 */

  int minutes = Time[player - 1][0];
  int seconds = Time[player - 1][1];

  if (minutes != 0 || seconds != 0){ // if time is not over, update by 1 second
    if (seconds == 0){ // change minute
      seconds = 60;
      minutes = minutes - 1;
    }    
    seconds = seconds - 1; // change seconds

    Time[player - 1][0] = minutes;
    Time[player - 1][1] = seconds;
      
    displayTime(player); 
  
  } else{ // if time is over, print the winner
    lcd.clear();
    lcd.setCursor(0, 0);
    if (player == 1){
      lcd.print("Black wins!");
    } else if (player == 2){
      lcd.print("White wins!");
    }
  } 
}
 
void resetClock(void){
  /* Reset clock to chosen Mode start value */
  Time[0][0] = TimeModes[Mode];
  Time[0][1] = 0;
  Time[1][0] = TimeModes[Mode];
  Time[1][1] = 0;
  startDisplay();
}

void displayTime(int player){
  /* Display actual Time for given player 1 or 2 */
  int minutes;
  int seconds;

  minutes = Time[player - 1][0];
  seconds = Time[player - 1][1];

  lcd.setCursor(CURSORMINUTES, player - 1);
  if (minutes/10){
    lcd.print(minutes);
   }else{
    lcd.print(0);
    lcd.print(minutes);
  }
  lcd.setCursor(CURSORSECONDS, player - 1);
  if (seconds/10){
    lcd.print(seconds);
  } else{
    lcd.print(0);
    lcd.print(seconds);
  }
}

void setTime(void){
  /* Start choosing clock time mode and change the mode */
  while(digitalRead(SETBUTTON) == HIGH){ // if mode not confirmed wait for change
    lcd.setCursor(CURSORMINUTES + 1, 0); 
    lcd.blink();

    if(digitalRead(WHITEBUTTON) == LOW){ // change the mode
      lcd.noBlink();
      Mode = (Mode + 1) % 5;
      Time[0][0] = TimeModes[Mode];
      Time[1][0] = TimeModes[Mode];
      displayTime(1);
      displayTime(2);
    }
      delay(200);
  }
  lcd.noBlink();
  delay(500);
}

void startDisplay(void){
  /* Display basic view: White and Black clocks */
  lcd.setCursor(0, 0);
  lcd.print("White: ");
  lcd.setCursor(CURSORMINUTES + 2, 0);
  lcd.print(":"); 
  lcd.setCursor(0, 1);
  lcd.print("Black: ");
  lcd.setCursor(CURSORMINUTES + 2, 1);
  lcd.print(":");
  displayTime(1);
  displayTime(2);
}
