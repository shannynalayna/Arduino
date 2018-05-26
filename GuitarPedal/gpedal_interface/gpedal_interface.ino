/*Source code below is written for an interactive guitar pedal
*/


#include <UTFT.h>
#include <URTouch.h>


#define player1 A0
#define player2 A1
#define player1Led A3
#define player2Led A4
#define twoPlayers A5
long previousP1LowMicros = 0;
long previousP1HighMicros = 0;
unsigned long currentP1LowMicros;
unsigned long currentP1HighMicros;
bool player1Bool;
bool player2Bool;
long p1Htime;
long p1Ltime;
long p2Htime;
long p2Ltime;
float player1Freq;
float player2Freq;
int two_playersValue;
bool two_playersGame;
int x;
int y;
char currentPage;
char notePage;
char scalePage;
char majorModePage;
char minorModePage;
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
//create and define freqVals array, each index is a frequency
float freqVals[] = {82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185,
                    196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415,
                    440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932,
                    988, 1047
                   };
//create and define caseVal array, each index represents a frequency
int caseVal[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                 44, 45
                };
char selectedSet[] = {currentPage, notePage, scalePage, majorModePage, minorModePage};

byte model = ITDB24;

UTFT myGLCD(model, 38,39,40,41); // Instantiate lcd screen instance

URTouch myTouch(6, 5, 4, 3, 2); // Instantiate touche screen


//--------------------------------------------------------------------------------
void setup() {
  myGLCD.InitLCD();//initialize lcd screen
  myGLCD.clrScr();//clear lcd
  myTouch.InitTouch();//initialize touch screen
  myTouch.setPrecision(PREC_MEDIUM);//set touch precision to medium
  drawHomeScreen();//call/draw homescreen
  currentPage = '0';//currentPage is 0
  notePage = '0';//
  scalePage = '0';
  majorModePage = '0';
  minorModePage = '0';
}
//--------------------------------------------------------------------------------
void drawHomeScreen() {
  myGLCD.setBackColor(0, 0, 0); //set background to black
  myGLCD.setColor(255, 255, 255); //set color to white
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.print("theoryPedal", CENTER, 10); //prints string to screen
  myGLCD.setColor(255, 0, 0); //sets color to red
  myGLCD.drawLine(0, 32, 319, 32); //draws red line
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.setFont(SmallFont);//sets font to small
  myGLCD.print("LeuthnerLangnerLabs", CENTER, 41); //prints string
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.print("Select Pedal Mode", CENTER, 64); //prints string
  //draw Jam Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(35, 90, 285, 130); //draws filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(35, 90, 285, 130); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//set font to big
  myGLCD.setBackColor(16, 167, 103); //sets background color to green
  myGLCD.print("Jam Mode", CENTER, 102); //prints string
  //draw Game Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(35, 190, 285, 230); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(35, 190, 285, 230); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("Game Mode", CENTER, 152); //prints string
}
//--------------------------------------------------------------------------------
void drawFrame(int x1, int y1, int x2, int y2) {
  myGLCD.setColor(255, 0, 0); //sets color to red
  myGLCD.drawRoundRect(x1, y1, x2, y2); //draw line rect around clicked button
  while (myTouch.dataAvailable()) {
    myTouch.read();//read the rouch screen position
    myGLCD.setColor(255, 255, 255); //sets color to white
    myGLCD.drawRoundRect(x1, y1, x2, y2); //draw line rect around button
  }
}
//--------------------------------------------------------------------------------
void drawJamScreen() {
  if (two_playersGame) {
    myGLCD.setColor(255, 255, 255); //set color to white
    myGLCD.fillRect(0, 0, 320, 240); //fill background white
    myGLCD.setColor(0, 0, 0); //set color to black
    myGLCD.fillRect(155, 0, 165, 240); //split screen horizontally
    //draw Back button
    myGLCD.setColor(16, 167, 103); //set color to green
    myGLCD.fillRoundRect(10, 10, 60, 36); //draw filled round rectangle
    myGLCD.setColor(255, 255, 255); //sets color to white
    myGLCD.drawRoundRect(10, 10, 60, 36); //draws line rect over filled rect
    myGLCD.setFont(BigFont);//sets font to big
    myGLCD.setBackColor(16, 167, 103); //sets background to green
    myGLCD.print("<< Back", 5, 13); //prints string
  }
  else {
    myGLCD.setColor(255, 255, 255); //set color to white
    myGLCD.fillRect(0, 0, 320, 240); //fill background white
    //draw Back button
    myGLCD.setColor(16, 167, 103); //set color to green
    myGLCD.fillRoundRect(10, 10, 60, 36); //draw filled round rectangle
    myGLCD.setColor(255, 255, 255); //sets color to white
    myGLCD.drawRoundRect(10, 10, 60, 36); //draws line rect over filled rect
    myGLCD.setFont(BigFont);//sets font to big
    myGLCD.setBackColor(16, 167, 103); //sets background to green
    myGLCD.print("<< Back", 5, 13); //prints string
  }
}
//--------------------------------------------------------------------------------
void drawPlayScreen() {
  if (two_playersGame) {
    myGLCD.setColor(255, 255, 255); //set color to white
    myGLCD.fillRect(0, 0, 320, 240); //fill background white
    myGLCD.setColor(0, 0, 0); //set color to black
    myGLCD.fillRect(0, 115, 320, 125); //split screen vertically
    //draw Back button
    myGLCD.setColor(16, 167, 103); //set color to green
    myGLCD.fillRoundRect(10, 10, 60, 36); //draw filled round rectangle
    myGLCD.setColor(255, 255, 255); //sets color to white
    myGLCD.drawRoundRect(10, 10, 60, 36); //draws line rect over filled rect
    myGLCD.setFont(BigFont);//sets font to big
    myGLCD.setBackColor(16, 167, 103); //sets background to green
    myGLCD.print("<< Back", 5, 13); //prints string
  }
  else {
    myGLCD.setColor(255, 255, 255); //set color to white
    myGLCD.fillRect(0, 0, 320, 240); //fill background white
    //draw Back button
    myGLCD.setColor(16, 167, 103); //set color to green
    myGLCD.fillRoundRect(10, 10, 60, 36); //draw filled round rectangle
    myGLCD.setColor(255, 255, 255); //sets color to white
    myGLCD.drawRoundRect(10, 10, 60, 36); //draws line rect over filled rect
    myGLCD.setFont(BigFont);//sets font to big
    myGLCD.setBackColor(16, 167, 103); //sets background to green
    myGLCD.print("<< Back", 5, 13); //prints string
  }
}
//--------------------------------------------------------------------------------
boolean two_players() {
  two_playersGame = false;//initialize two_playersGame
  if (two_playersValue > 512) {
    two_playersGame = true;
    //if 2playerValue analog value is high, the two_playersGame is true
  }
  else {
    two_playersGame = false;
    //if 2playerValue analog value is low, the two_playersGame is false
  }
  return two_playersGame;
  //return boolean value to determine if its a 2 player game or not
}
//--------------------------------------------------------------------------------
void readPlayer1() {
  if (analogRead(player1) > 527) {
    player1Bool = HIGH;
  }
  else {
    player1Bool = LOW;
  }
}
//--------------------------------------------------------------------------------
void readPlayer2() {
  if (analogRead(player2) > 527) {
    player2Bool = HIGH;
  }
  else {
    player2Bool = LOW;
  }
}
//--------------------------------------------------------------------------------
void getJamNotes() {
  if (two_players()) { //2player jam session
    //get both player1 and player2 frequencies
    readPlayer1();// call/ run readPlayer1 function
    if (player1Bool == HIGH) { //player1Bool is high
      currentP1HighMicros = micros();
      p1Ltime = currentP1LowMicros - previousP1LowMicros;//
      previousP1LowMicros = currentP1LowMicros;
    }
    else if (player1Bool == LOW) { //player1Bool is low
      currentP1LowMicros = micros();
      p1Htime = currentP1HighMicros - previousP1HighMicros;
      previousP1HighMicros = currentP1HighMicros;
    }
    player1Freq = 1 / (p1Ltime + p1Htime);
    readPlayer2();//call/run readPlayer2 function
    
    if (player2Bool == HIGH) {
      currentP2HighMicros = micros();
      p2Ltime = currentP2LowMicros - previousP2LowMicros;
      previousP2LowMicros = currentP2LowMicros;
    }
    else if (player2Bool == LOW) {
      currentP2LowMicros = micros();
      p2Htime = currentP2HighMicros - previousP2HighMicros;
      previousP2HighMicros = currentP1HighMicros;
    }
    player2Freq = 1 / (p2Ltime + p2Htime);
    findFreq(player2Freq);
  }
  else { //singles player jam session
    //get only player1 frequency
    readPlayer1();//call/run readPlayer1 function
    if (player1Bool == HIGH) { //player1Bool is high
      currentP1HighMicros = micros();
      p1Ltime = currentP1LowMicros - previousP1LowMicros;//
      previousP1LowMicros = currentP1LowMicros;
    }
    else if (player1Bool == LOW) { //player1Bool is low
      currentP1LowMicros = micros();
      p1Htime = currentP1HighMicros - previousP1HighMicros;
      previousP1HighMicros = currentP1HighMicros;
    }
    player1Freq = 1 / (p1Ltime + p1Htime);
    findFreq(player1Freq);
  }
}
//--------------------------------------------------------------------------------
void drawNoteScreen() {
  //draw note screen
  myGLCD.setBackColor(0, 0, 0); //set background to black
  myGLCD.setColor(255, 255, 255); //set color to white
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.print("Select Note", LEFT, 10); //prints string to screen
  //draw A Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(160, 6, 239, 36); //draws filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(160, 6, 239, 36); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//set font to big
  myGLCD.setBackColor(16, 167, 103); //sets background color to green
  myGLCD.print("A", 200, 21); //prints string
  //draw A# Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(241, 6, 320, 36); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(241, 6, 320, 36); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("A#", 280, 21); //prints string
  //draw B Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(160, 38, 320, 69); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(160, 38, 320, 69); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("B", 240, 52); //prints string
  //draw C Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(160, 71, 239, 102); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(160, 71, 239, 102); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("C", 200, 86); //prints string
  //draw C# Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(241, 71, 320, 102); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(241, 71, 320, 102); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("C#", 280, 86); //prints string
  //draw D Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(160, 104, 239, 135); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(160, 104, 239, 135); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("D", 200, 119); //prints string
  //draw D# Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(241, 104, 320, 135); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(241, 104, 320, 135); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("D#", 280, 119); //prints string
  //draw E Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(160, 137, 320, 168); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(160, 137, 320, 168); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("E", 240, 152); //prints string
  //draw F Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(160, 170, 239, 201); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(160, 170, 239, 201); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("F", 200, 185); //prints string
  //draw F# Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(241, 170, 320, 201); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(241, 170, 320, 201); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("F#", 280, 185); //prints string
  //draw G Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(160, 203, 239, 234); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(160, 203, 239, 234); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("G", 200, 218); //prints string
  //draw G# Button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(241, 203, 320, 234); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(241, 203, 320, 234); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("G#", 280, 218); //prints string
  //draw Back button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(10, 10, 60, 36); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(10, 10, 60, 36); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("<< Back", 5, 13); //prints string
};
//--------------------------------------------------------------------------------
void drawScaleScreen() {
  //draw scale screen- major and minor scale buttons
  myGLCD.setBackColor(0, 0, 0); //set background to black
  myGLCD.setColor(255, 255, 255); //set color to white
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.print("Select Scale", LEFT, 10); //prints string to screen
  //draw Major button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(80, 40, 240, 119); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(80, 40, 240, 119); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("Major Scale", 280, 218); //prints string
  //draw Minor button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(80, 121, 240, 200); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(80, 121, 240, 200); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("Minor Scale", 280, 218); //prints string
  //draw Back button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(10, 10, 60, 36); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(10, 10, 60, 36); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("<< Back", 5, 13); //prints string
}
//--------------------------------------------------------------------------------
void drawModeScreen() {
  //draw mode screen
  myGLCD.setBackColor(0, 0, 0); //set background to black
  myGLCD.setColor(255, 255, 255); //set color to white
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.print("Select Scale Type", LEFT, 10); //prints string to screen
  //draw Major Natural button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(80, 40, 240, 119); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(80, 40, 240, 119); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("Natural Major", 280, 218); //prints string
  //draw Major Pentatonic button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(80, 121, 240, 200); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(80, 121, 240, 200); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("Major Pentatonic", 280, 218); //prints string
  //draw Back button
  myGLCD.setColor(16, 167, 103); //set color to green
  myGLCD.fillRoundRect(10, 10, 60, 36); //draw filled round rectangle
  myGLCD.setColor(255, 255, 255); //sets color to white
  myGLCD.drawRoundRect(10, 10, 60, 36); //draws line rect over filled rect
  myGLCD.setFont(BigFont);//sets font to big
  myGLCD.setBackColor(16, 167, 103); //sets background to green
  myGLCD.print("<< Back", 5, 13); //prints string
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void findFreq(int frequency) {
  freqCase;
  for (int i = 0; i < 45 ; i++) {
    if ((((frequency - freqVals[i]) / frequency) * 100) < 3.0) {
      freqCase = caseVal[i];
      freqVal = caseVal[i];
      break;
    }
  }
  switch (freqCase) {
    case 1:
      //note is Open E-82Hz
      myScreen.background(0, 255, 0);
      myScreen.text("E", midX, midY);
      delay(t);
      break;
    case 2:
      //note is F-87Hz
      myScreen.background(127, 0, 127);
      myScreen.text("F", midX, midY);
      delay(t);
      break;
    case 3:
      //note is F#-92Hz
      myScreen.background(255, 0, 255);
      myScreen.text("F#", midX, midY);
      delay(t);
      break;
    case 4:
      //note is G-98Hz
      myScreen.background(0, 0, 0);
      myScreen.text("G", midX, midY);
      delay(t);
      break;
    case 5:
      //note is G#-104Hz
      myScreen.background(5, 5, 5);
      myScreen.text("G#", midX, midY);
      delay(t);
      break;
    case 6:
      //note is A-110Hz
      myScreen.background(127, 0, 0);
      myScreen.text("A", midX, midY);
      delay(t);
      break;
    case 7:
      //note is A#-117Hz
      myScreen.background(255, 0, 0);
      myScreen.text("A#", midX, midY);
      delay(t);
      break;
    case 8:
      //note is B-123Hz
      myScreen.background(0, 0, 255);
      myScreen.text("B", midX, midY);
      delay(t);
      break;
    case 9:
      //note is C-131Hz
      myScreen.background(127, 127, 0);
      myScreen.text("C", midX, midY);
      delay(t);
      break;
    case 10:
      //note is C#-139Hz
      myScreen.background(255, 255, 0);
      myScreen.text("C#", midX, midY);
      delay(t);
      break;
    case 11:
      //note is D-147Hz
      myScreen.background(127, 127, 127);
      myScreen.text("D", midX, midY);
      delay(t);
      break;
    case 12:
      //note is D#-156Hz
      myScreen.background(255, 255, 255);
      myScreen.text("D#", midX, midY);
      delay(t);
      break;
    case 13:
      //note is E-165Hz
      myScreen.background(0, 255, 0);
      myScreen.text("E", midX, midY);
      delay(t);
      break;
    case 14:
      //note is F-175Hz
      myScreen.background(127, 0, 127);
      myScreen.text("F", midX, midY);
      delay(t);
      break;
    case 15:
      //note is F#-185Hz
      myScreen.background(255, 0, 255);
      myScreen.text("F#", midX, midY);
      delay(t);
      break;
    case 16:
      //note is G-196Hz
      myScreen.background(0, 0, 0);
      myScreen.text("G", midX, midY);
      delay(t);
      break;
    case 17:
      //note is G#-208Hz
      myScreen.background(5, 5, 5);
      myScreen.text("G#", midX, midY);
      delay(t);
      break;
    case 18:
      //note is A-220Hz
      myScreen.background(127, 0, 0);
      myScreen.text("A", midX, midY);
      delay(t);
      break;
    case 19:
      //note is A#-233Hz
      myScreen.background(255, 0, 0);
      myScreen.text("A#", midX, midY);
      delay(t);
      break;
    case 20:
      //note is B-247Hz
      myScreen.background(0, 0, 255);
      myScreen.text("B", midX, midY);
      delay(t);
      break;
    case 21:
      //note is C-262Hz
      myScreen.background(127, 127, 0);
      myScreen.text("C", midX, midY);
      delay(t);
      break;
    case 22:
      //note is C#-277Hz
      myScreen.background(255, 255, 0);
      myScreen.text("C#", midX, midY);
      delay(t);
      break;
    case 23:
      //note is D-294Hz
      myScreen.background(127, 127, 127);
      myScreen.text("D", midX, midY);
      delay(t);
      break;
    case 24:
      //note is D#-311
      myScreen.background(255, 255, 255);
      myScreen.text("D#", midX, midY);
      delay(t);
      break;
    case 25:
      //note is E-330Hz
      myScreen.background(0, 255, 0);
      myScreen.text("E", midX, midY);
      delay(t);
      break;
    case 26:
      //note is F-349Hz
      myScreen.background(127, 0, 127);
      myScreen.text("F", midX, midY);
      delay(t);
      break;
    case 27:
      //note is F#-370Hz
      myScreen.background(255, 0, 255);
      myScreen.text("F#", midX, midY);
      delay(t);
      break;
    case 28:
      //note is G-392Hz
      myScreen.background(0, 0, 0);
      myScreen.text("G", midX, midY);
      delay(t);
      break;
    case 29:
      //note is G#-415Hz
      myScreen.background(5, 5, 5);
      myScreen.text("G#", midX, midY);
      delay(t);
      break;
    case 30:
      //note is A-440Hz
      myScreen.background(127, 0, 0);
      myScreen.text("A", midX, midY);
      delay(t);
      break;
    case 31:
      //note is A#- 466Hz
      myScreen.background(255, 0, 0);
      myScreen.text("A#", midX, midY);
      delay(t);
      break;
    case 32:
      //note is B-494Hz
      myScreen.background(0, 0, 255);
      myScreen.text("B", midX, midY);
      delay(t);
      break;
    case 33:
      //note is C-523Hz
      myScreen.background(127, 127, 0);
      myScreen.text("C", midX, midY);
      delay(t);
      break;
    case 34:
      //note is C#-554Hz
      myScreen.background(255, 255, 0);
      myScreen.text("C#", midX, midY);
      delay(t);
      break;
    case 35:
      //note is D-587Hz
      myScreen.background(127, 127, 127);
      myScreen.text("D", midX, midY);
      delay(t);
      break;
    case 36:
      //note is D#-622Hz
      myScreen.background(255, 255, 255);
      myScreen.text("D#", midX, midY);
      delay(t);
      break;
    case 37:
      //note is E-659Hz
      myScreen.background(255, 0, 0);
      myScreen.text("E", midX, midY);
      delay(t);
      break;
    case 38:
      //note is F-698Hz
      myScreen.background(127, 0, 127);
      myScreen.text("F", midX, midY);
      delay(t);
      break;
    case 39:
      //note is F#-740Hz
      myScreen.background(255, 0, 255);
      myScreen.text("F#", midX, midY);
      delay(t);
      break;
    case 40:
      //note is G-784Hz
      myScreen.background(0, 0, 0);
      myScreen.text("G", midX, midY);
      delay(t);
      break;
    case 41:
      //note is G#-831Hz
      myScreen.background(5, 5, 5);
      myScreen.text("G#", midX, midY);
      delay(t);
      break;
    case 42:
      //note is A-880Hz
      myScreen.background(127, 0, 0);
      myScreen.text("A", midX, midY);
      delay(t);
      break;
    case 43:
      //note is A#-932Hz
      myScreen.background(255, 0, 0);
      myScreen.text("A#", midX, midY);
      delay(t);
      break;
    case 44:
      //note is B-988Hz
      myScreen.background(0, 0, 255);
      myScreen.text("B", midX, midY);
      delay(t);
      break;
    case 45:
      //note is C-1047Hz
      myScreen.background(127, 127, 0);
      myScreen.text("C", midX, midY);
      delay(t);
      break;
    default:
      myScreen.background(0, 0, 0);//set background to black
      break;
  }
}
//--------------------------------------------------------------------------------
void loop() {
  if (currentPage == '0') {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX();//get x-coord of touch
      y = myTouch.getY();//get y-coord of touch
      if (x >= 35 && x <= 285 && y >= 90 && y <= 130) { //jam button touched
        drawFrame(35, 90, 285, 130); //draw frame around button
        currentPage = '1';//set current page to 1
        myGLCD.clrScr();//clear lcd
        drawJamScreen();
      }
      if (x >= 35 && x <= 285 && y >= 190 && y <= 230) { //game button touched
        drawFrame(35, 190, 285, 230); //draw frame around button
        currentPage = '2';//set current page to 2
        myGLCD.clrScr();//clear screen
        myGLCD.setColor(114, 198, 206); //set color
        myGLCD.fillRect(0, 0, 319, 39); //set screen background
        drawGameModes();//draw game modes/scales
      }
    }
  }
  if (currentPage == '1') {
    getJamNotes();//call/run the getNotes method
    if (myTouch.dataAvailable()) {
      myTouch.read();//read touch screen
      x = myTouch.getX();//get x coord of touch
      y = myTouch.getY();//get y coord of touch
      if (x >= 10 && x <= 60 && y >= 10 && y <= 36) { //back button pressed
        drawFrame(10, 10, 60, 36); //highlight back button
        currentPage = '0';//set currentPage to homescreen
        myGLCD.clrScr();//clear screen
        drawHomeScreen();//draws home screen
      }
    }
  }
  if (currentPage == '2') { //draw mode/scale screen
    drawNoteScreen();//draw the scales and modes buttons/screen
    if (notePage == '0') {
      if (myTouch.dataAvailable()) {
        myTouch.read();//read touch screen
        x = myTouch.getX();//get x coord of touch
        y = myTouch.getY();//get y coord of touch
        //select note
        if (x <= 239 && x >= 160 && y <= 36 && y >= 6) {
          //selected A note
          notePage = '1';
          drawFrame(160, 6, 239, 36);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 320 && x >= 241 && y <= 36 && y >= 6) {
          //selected A# note
          notePage = '2';
          drawFrame(241, 6, 320, 36);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 320 && x >= 160 && y <= 69 && y >= 38) {
          //selected B note
          notePage = '3';
          drawFrame(160, 38, 320, 69);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 239 && x >= 160 && y <= 102 && y >= 71) {
          //selected C note
          notePage = '4';
          drawFrame(160, 71, 239, 102);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 320 && x >= 241 && y <= 102 && y >= 71) {
          //selected C# note
          notePage = '5';
          drawFrame(241, 71, 320, 102);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 239 && x >= 160 && y <= 135 && y >= 104) {
          //selected D note
          notePage = '6';
          drawFrame(160, 104, 239, 135);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 320 && x >= 241 && y <= 135 && y >= 104) {
          //selected D# note
          notePage = '7';
          drawFrame(241, 104, 320, 135);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 320 && x >= 160 && y <= 168 && y >= 137) {
          //selected E note
          notePage = '8';
          drawFrame(160, 137, 320, 168);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 239 && x >= 160 && y <= 201 && y >= 170) {
          //selected F note
          notePage = '8';
          drawFrame(160, 170, 239, 201);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 320 && x >= 241 && y <= 201 && y >= 170) {
          //selected F# note
          notePage = '9';
          drawFrame(241, 170, 320, 201);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 239 && x >= 160 && y <= 234 && y >= 203) {
          //selected G note
          notePage = '10';
          drawFrame(160, 203, 239, 234);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x <= 320 && x >= 241 && y <= 234 && y >= 203) {
          //selected G# note
          notePage = '11';
          drawFrame(241, 203, 320, 234);
          myGLCD.clrScr();
          drawScaleScreen();
        }
        if (x >= 10 && x <= 60 && y >= 10 && y <= 36) { //back button pressed
          drawFrame(10, 10, 60, 36); //highlight back button
          notePage = '0';//set currentPage to homescreen
          myGLCD.clrScr();//clear screen
          drawHomeScreen();//draws home screen
        }
        if (notePage != 0) {
          if (myTouch.dataAvailable()) {
            myTouch.read();
            x = myTouch.getX();
            y = myTouch.getY();
            if (scalePage == '0') {
              if (x <= 240 && x >= 80 && y <= 119 && y >= 40) {
                //major scale selected
                scalePage = '1';
                drawFrame(80, 40, 240, 119);
                myGLCD.clrScr();
                drawModeScreen()
              }
              if (x <= 240 && x >= 80 && y <= 200 && y >= 121) {
                //minor scale selected
                scalePage = '2';
                drawFrame(80, 121, 240, 200);
                myGLCD.clrScr();
                drawModeScreen();
              }
              if (x >= 10 && x <= 60 && y >= 10 && y <= 36) { //back button pressed
                drawFrame(10, 10, 60, 36); //highlight back button
                scalePage = '0';//set scalePage to homescreen
                myGLCD.clrScr();//clear screen
                drawNoteScreen();//draws note screen
              }
              if (scalePage == '1') { //major scale selected
                if (myTouch.dataAvailable()) {
                  myTouch.read();
                  x = myTouch.getX();
                  y = myTouch.getY();
                  if (x <= 240 && x >= 80 && y <= 119 && y >= 40) {
                    //Major Natural
                    majorModePage = '1';
                    myGLCD.clrScr();
                    drawPlayScreen();
                  }
                  if (x <= 240 && x >= 80 && y <= 200 && y >= 121) {
                    //Major Pentatonic
                    majorModePage = '2';
                    myGLCD.clrScr();
                    drawPlayScreen();
                  }
                  if (x >= 10 && x <= 60 && y >= 10 && y <= 36) { //back button pressed
                    drawFrame(10, 10, 60, 36); //highlight back button
                    majorModePage = '0';//set scalePage to homescreen
                    myGLCD.clrScr();//clear screen
                    drawScaleScreen();//draws note screen
                  }
                }
                if (scalePage == '2') { //minor scale selected
                  if (myTouch.dataAvailable()) {
                    myTouch.read();
                    x = myTouch.getX();
                    y = myTouch.getY();
                    if (x <= && x >= && y <= && y >= ) {
                      //Natural Minor
                      minorModePage = '1';
                      drawPlayScreen();
                    }
                    if (x <= && x >= && y <= && y >= ) {
                      //Harmonic Minor
                      minorModePage = '2';
                      drawPlayScreen();
                    }
                    if (x <= && x >= && y <= && y >= ) {
                      //Melodic Minor
                      minorModePage = '3';
                      drawPlayScreen();
                    }
                    if (x <= && x >= && y <= && y >= ) {
                      //Minor Pentatonic
                      minorModePage = '4';
                      drawPlayScreen();
                    }
                    if (x <= && x >= && y <= && y >= ) {
                      //Blues scale
                      minorModePage = '4';
                      drawPlayScreen();
                    }
                    if (x >= 10 && x <= 60 && y >= 10 && y <= 36) { //back button pressed
                      drawFrame(10, 10, 60, 36); //highlight back button
                      scalePage = '0';//set scalePage to homescreen
                      myGLCD.clrScr();//clear screen
                      drawScaleScreen();//draws note screen
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
