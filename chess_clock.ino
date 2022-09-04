#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#define whiteButton 10
#define blackButton 11
#define setButton 9

int Time1[2] = {10, 0};
int Time2[2] = {10, 0};
int TimeModes[5] = {1, 3, 5, 10, 15};
unsigned long StartTime = 0;
int Mode = 3;
int Counts = 0;


void setup() {
  lcd.begin(16,2);
  lcd.clear();
  startDisplay();
  pinMode(whiteButton, INPUT_PULLUP);
  pinMode(blackButton, INPUT_PULLUP);
  pinMode(setButton, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(blackButton) == LOW){
    StartTime = millis();
    while(digitalRead(whiteButton) == HIGH){
      if ((millis() - StartTime) >= 1000){
        StartTime = millis();
        updateTime(1);
      }
      if (digitalRead(setButton) == LOW){
        resetClock();
        delay(500);
        break;
      }
    }
  }
  if (digitalRead(whiteButton) == LOW){
    StartTime = millis();
    while(digitalRead(blackButton) == HIGH){
      if ((millis() - StartTime) >= 1000){
        StartTime = millis();
        updateTime(2);
      }
      if (digitalRead(setButton) == LOW){
        resetClock();
        delay(500);
        break;
      }
    }
  }

  if(digitalRead(setButton) == LOW){
    delay(1000);
    Counts = Counts + 1;
    if (Counts == 2){
      while(digitalRead(setButton) == LOW){}
      setTime();
      Counts = 0;
    }
  }
  if(digitalRead(setButton) == HIGH){
    Counts = 0;
  }
}

/* functions */

void  updateTime(int player){
/* Updates time by 1 second for given player 1 or 2 */
  int minutes;
  int seconds;

  if(player == 1){ // read time into local variables
    minutes = Time1[0];
    seconds = Time1[1];
  } else if(player == 2){
    minutes = Time2[0];
    seconds = Time2[1];   
  } else{
    minutes = 0;
    seconds = 0;
  }

  if (minutes != 0 || seconds != 0){ // if time is not over, update by 1 second
    if (seconds == 0){ // change minute
      seconds = 60;
      minutes = minutes - 1;
    }
    
    seconds = seconds - 1; // change seconds
    
    if (player == 1){ // update global variables
      Time1[0] = minutes;
      Time1[1] = seconds;
    } else if (player == 2){
      Time2[0] = minutes;
      Time2[1] = seconds;
    } else{
      Time1[0] = 0;
      Time1[1] = 0;
      Time2[0] = 0;
      Time2[1] = 0;
    }
      
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
  Time1[0] = TimeModes[Mode];
  Time1[1] = 0;
  Time2[0] = TimeModes[Mode];
  Time2[1] = 0;
  startDisplay();
}

void displayTime(int player){
  /* Display actual Time for given player 1 or 2 */
  int minutes;
  int seconds;

  if (player == 1){
    minutes = Time1[0];
    seconds = Time1[1];    
  } else if (player == 2){
    minutes = Time2[0];
    seconds = Time2[1];   
  } else {
    minutes = 0;
    seconds = 0;
  }

    lcd.setCursor(7, player - 1);
    if (minutes/10){
      lcd.print(minutes);
     }else{
      lcd.print(0);
      lcd.print(minutes);
    }
    lcd.setCursor(10, player - 1);
    if (seconds/10){
      lcd.print(seconds);
    } else{
      lcd.print(0);
      lcd.print(seconds);
    }
}

void setTime(void){
  /* Start choosing clock time mode and change the mode */
  while(digitalRead(setButton) == HIGH){ // if mode not confirmed wait for change
    lcd.setCursor(8, 0); 
    lcd.blink();

    if(digitalRead(whiteButton) == LOW){ // change the mode
      lcd.noBlink();
      Mode = Mode + 1;
      if (Mode > 4){
        Mode = 0;
      }
      Time1[0] = TimeModes[Mode];
      Time2[0] = TimeModes[Mode];
      displayTime(1);
      displayTime(2);
    }
      delay(200);
  }
  delay(500);
}

void startDisplay(void){
  /* Display basic view: White and Black clocks */
  lcd.setCursor(0, 0);
  lcd.print("White: ");
  lcd.setCursor(9, 0);
  lcd.print(":");
  lcd.setCursor(0, 1);
  lcd.print("Black: ");
  lcd.setCursor(9, 1);
  lcd.print(":");
  displayTime(1);
  displayTime(2);
}
