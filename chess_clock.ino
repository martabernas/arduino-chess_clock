#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#define whiteButton 10
#define blackButton 11
#define setButton 9

int time1[2] = {10, 0};
int time2[2] = {10, 0};
int timeModes[5] = {1, 3, 5, 10, 15};
unsigned long startTime = 0;
int mode = 3;
int counts = 0;


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
    startTime = millis();
    while(digitalRead(whiteButton) == HIGH){
      if ((millis() - startTime) >= 1000){
        startTime = millis();
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
    startTime = millis();
    while(digitalRead(blackButton) == HIGH){
      if ((millis() - startTime) >= 1000){
        startTime = millis();
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
    counts = counts + 1;
    if (counts == 2){
      while(digitalRead(setButton) == LOW){}
      setTime();
      counts = 0;
    }
  }
  if(digitalRead(setButton) == HIGH){
    counts = 0;
  }
}

  
void  updateTime(int player){
  int minutes;
  int seconds;

  if(player == 1){
    minutes = time1[0];
    seconds = time1[1];
  } else if(player == 2){
    minutes = time2[0];
    seconds = time2[1];   
  } else{
    minutes = 0;
    seconds = 0;
  }

  if (minutes != 0 || seconds != 0){
    if (seconds == 0){
      seconds = 60;
      minutes = minutes - 1;
    }
    
    seconds = seconds - 1;
    
    if (player == 1){
      time1[0] = minutes;
      time1[1] = seconds;
    } else if (player == 2){
      time2[0] = minutes;
      time2[1] = seconds;
    } else{
      time1[0] = 0;
      time1[1] = 0;
      time2[0] = 0;
      time2[1] = 0;
    }
      
    displayTime(player);
  
  } else{ 
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
  time1[0] = timeModes[mode];
  time1[1] = 0;
  time2[0] = timeModes[mode];
  time2[1] = 0;
  startDisplay();
}

void displayTime(int player){
  int minutes;
  int seconds;

  if (player == 1){
    minutes = time1[0];
    seconds = time1[1];    
  } else if (player == 2){
    minutes = time2[0];
    seconds = time2[1];   
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
  while(digitalRead(setButton) == HIGH){
    lcd.setCursor(8, 0); 
    lcd.blink();

    if(digitalRead(whiteButton) == LOW){
      lcd.noBlink();
      mode = mode + 1;
      if (mode > 4){
        mode = 0;}

      time1[0] = timeModes[mode];
      time2[0] = timeModes[mode];
      displayTime(1);
      displayTime(2);
    }
      delay(200);
  }
  lcd.noBlink();
  delay(500);
}

void startDisplay(void){
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
