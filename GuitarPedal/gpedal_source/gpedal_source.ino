/* frequency it dispays a unique color, corresponding to that frequency,
 * on a 1.8" LCD screen using the TFT libraries. The detectable
 * frequency range for this code is about 1-38,500 Hz. The highest note
 * on a guitar string is about 2,000 Hz. The TFT libraries are
 * modified- only containing pertinent methods and variables that will be
 * used in the code below. The code is setup to detect a note +-2.5Hz from
 * its fundamental frequency to allow for a small amount of error.
 *
 * Designed by Ari Leuthner & Jordan Langner
 */


//Include libraries and define variables
#include <TFT.h> //include TFT library
#define cs   10 //set lcd cs pin to arduino pin 10
#define dc   9 //set lcd dc pin to arduino pin 9
#define rst  8 //set lcd reset pin to arduino pin 8
TFT myScreen = TFT(cs, dc, rst);//declare name of TFT screen
//data storage variables
byte newData = 0;
byte prevData = 0;
unsigned int time = 0;//keeps time and stores vals in timer[]
int timer[10];//storage for timing events
int slope[10];//storage for slope events
unsigned int totalTimer;//used to calculate period
unsigned int period;//storage of wave period
byte index = 0;//current storage index
float frequency;//storage for frequency calculations
int maxSlope = 0;//used to calculate max slope as trigger point
int newSlope;//storage for incoming slope data
//variable for matching
byte noMatch = 0;//counts how many nonmatches you've received
byte slopeTol = 3;//slope tolerance, adjust if needed
int timerTol = 10;//timer tolerance, adjust if needed
unsigned int ampTimer = 0;
byte maxAmp = 0;
byte checkMaxAmp;
byte ampThreshold = 30;//raise value if signal is noisy


void setup(){
  Serial.begin(9600);
  myScreen.begin();//initialize TFT screen
  myScreen.background(0,0,0);//set background to black
  pinMode(12,OUTPUT);
  myScreen.stroke(255,255,255);
  myScreen.setTextSize(2);
  myScreen.text("**Pedal Name**",15,25);
  myScreen.setTextSize(1);
  myScreen.text("By Ari Leuthner",15,75);
  myScreen.text("and Jordan Langner",15,80);
  delay(3000);
  cli();//disable interrupts
  //set up continuous sampling of A0 at 38.5kHz
  ADCSRA = 0;//clear ADCSRA register
  ADCSRB = 0;//clear ADCSRB register
  ADMUX |= (1 << REFS0);//set reference voltage
  ADMUX |= (1 << ADLAR);//left align ADC value, to read highest 8 bits
  ADCSRA |= (1 << ADPS2)|(1 << ADPS0);//set ADC clock with 32 prescaler
  ADCSRA |= (1 << ADATE);//enable auto trigger
  ADCSRA |= (1 << ADIE);//enable interrups when measurement complete
  ADCSRA |= (1 << ADEN);//enable ADC
  ADCSRA |= (1 << ADSC);//start ADC measurements
  sei();//enable interrupts
}


ISR(ADC_vect){
  //when new ADC value is ready
  PORTB &= B11101111;//set pin 12 low
  prevData = newData;//store previous value
  newData = ADCH;//get value from A0
  if (prevData < 511 && newData >= 511){
    //if increasing and crossing midpoint
    newSlope = newData - prevData;//calculate slope
    if(abs(newSlope - maxSlope) < slopeTol){
      //if slopes are equal
      slope[index] = newSlope;//record new slope
      timer[index] = time;
      time = 0;//reset time
      if(index == 0){
        //newmax slope just reset
        PORTB |= B00010000;//set pin 12 high
        noMatch = 0;
        index++;//increment index
      }
      else if(abs(timer[0] - timer[index]) < timerTol && abs(slope[0] - newSlope) < slopeTol){
        //if timer duration and slopes match
        //sum timer values
        totalTimer = 0;
        for(byte i = 0; i < index; i++){
          totalTimer += timer[i];
        }
        period = totalTimer;//set period
        //reset new zero index values to compare with
        timer[0] = timer[index];
        slope[0] = slope[index];
        index = 1;//set index to 1
        PORTB |= B00010000;//set pin 12 high
        noMatch = 0;
      }
      else{
        //crossing midpoint but not match
        index++;//increment index
        if(index > 9){
          reset();
        }
      }
    }
    else if(newSlope > maxSlope){
      //if new slope is much larger than max slope
      maxSlope = newSlope;
      time = 0;//reset clock
      noMatch = 0;
      index = 0;//reset index
    }
    else{
      //slope not steep enough
      noMatch++;//increment noMatch counter
      if(noMatch > 9){
        reset();
      }
    }
  }
  time++;//increment timer at rate of 38.5kHz
  ampTimer++;//increment amplitude timer
  if(abs(511 - ADCH) > maxAmp){
    maxAmp = abs(511 - ADCH);
  }
  if(ampTimer == 1000){
    ampTimer = 0;
    checkMaxAmp = maxAmp;
    maxAmp = 0;
  }
}
void reset(){
  //clear out some variables
  index = 0;//reset index
  noMatch = 0;//reset match counter
  maxSlope = 0;//reset slope
}


void loop(){
  if(checkMaxAmp > ampThreshold){
    frequency = 38462/float(period);//calculate frequency = timer rate / period
    if(frequency > 79.5 && frequency < 84.5){
      //note is Open E-82Hz
      myScreen.background(0,255,0);
    }
    else if(frequency > 84.5 && frequency < 89.5){
      //note is F-87Hz
      myScreen.background(127,0,127);
      delay(ampTimer);
    }
    else if(frequency > 89.5 && frequency < 94.5){
      //note is F#-92Hz
      myScreen.background(255,0,255);
      delay(ampTimer);
    }
    else if(frequency > 94.5 && frequency < 100.5){
      //note is G-98Hz
      myScreen.background(0,0,0);
      delay(ampTimer);
    }
    else if(frequency > 101.5 && frequency < 106.5){
      //note is G#-104Hz
      myScreen.background(5,5,5);
      delay(ampTimer);
    }
    else if(frequency > 107.5 && frequency < 112.5){
      //note is A-110Hz
      myScreen.background(127,0,0);
      delay(ampTimer);
    }
    else if(frequency > 114.5 && frequency < 119.5){
      //note is A#-117Hz
      myScreen.background(255,0,0);
      delay(ampTimer);
    }
    else if(frequency > 120.5 && frequency < 125.5){
      //note is B-123Hz
      myScreen.background(0,0,255);
      delay(ampTimer);
    }
    else if(frequency > 128.5 && frequency < 133.5){
      //note is C-131Hz
      myScreen.background(127,127,0);
      delay(ampTimer);
    }
    else if(frequency > 136.5 && frequency < 141.5){
      //note is C#-139Hz
      myScreen.background(255,255,0);
      delay(ampTimer);
    }
    else if(frequency > 144.5 && frequency < 149.5){
      //note is D-147Hz
      myScreen.background(127,127,127);
      delay(ampTimer);
    }
    else if(frequency > 153.5 && frequency < 158.5){
      //note is D#-156Hz
      myScreen.background(255,255,255);
      delay(ampTimer);
    }
    else if(frequency > 162.5 && frequency < 167.5){
      //note is E-165Hz
      myScreen.background(0,255,0);
      delay(ampTimer);
    }
    else if(frequency > 172.5 && frequency < 177.5){
      //note is F-175Hz
      myScreen.background(127,0,127);
      delay(ampTimer);
    }
    else if(frequency > 182.5 && frequency < 187.5){
      //note is F#-185Hz
      myScreen.background(255,0,255);
      delay(ampTimer);
    }
    else if(frequency > 193.5 && frequency < 198.5){
      //note is G-196Hz
      myScreen.background(0,0,0);
      delay(ampTimer);
    }
    else if(frequency > 205.5 && frequency < 210.5){
      //note is G#-208Hz
      myScreen.background(5,5,5);
      delay(ampTimer);
    }
    else if(frequency > 217.5 && frequency < 220.5){
      //note is A-220Hz
      myScreen.background(127,0,0);
      delay(ampTimer);
    }
    else if(frequency > 230.5 && frequency < 235.5){
      //note is A#-233Hz
      myScreen.background(255,0,0);
      delay(ampTimer);
    }
    else if(frequency > 244.5 && frequency < 249.5){
      //note is B-247Hz
      myScreen.background(0,0,255);
      delay(ampTimer);
    }
    else if(frequency > 259.5 && frequency < 264.5){
      //note is C-262Hz
      myScreen.background(127,127,0);
      delay(ampTimer);
    }
    else if(frequency > 274.5 && frequency < 279.5){
      //note is C#-277Hz
      myScreen.background(255,255,0);
      delay(ampTimer);
    }
    else if(frequency > 291.5 && frequency < 296.5){
      //note is D-294Hz
      myScreen.background(127,127,127);
      delay(ampTimer);
    }
    else if(frequency > 308.5 && frequency < 313.5){
      //note is D#-311
      myScreen.background(255,255,255);
      delay(ampTimer);
    }
    else if(frequency > 327.5 && frequency < 332.5){
      //note is E-330Hz
      myScreen.background(0,255,0);
      delay(ampTimer);
    }
    else if(frequency > 346.5 && frequency < 351.5){
      //note is F-349Hz
      myScreen.background(127,0,127);
      delay(ampTimer);
    }
    else if(frequency > 366.5 && frequency < 372.5){
      //note is F#-370Hz
      myScreen.background(255,0,255);
      delay(ampTimer);
    }
    else if(frequency > 389.5 && frequency < 394.5){
      //note is G-392Hz
      myScreen.background(0,0,0);
      delay(ampTimer);
    }
    else if(frequency > 412.5 && frequency < 417.5){
      //note is G#-415Hz
      myScreen.background(5,5,5);
      delay(ampTimer);
    }
    else if(frequency > 437.5 && frequency < 442.5){
      //note is A-440Hz
      myScreen.background(127,0,0);
      delay(ampTimer);
    }
    else if(frequency > 463.5 && frequency < 468.5){
      //note is A#- 466Hz
      myScreen.background(255,0,0);
      delay(ampTimer);
    }
    else if(frequency > 491.5 && frequency < 496.5){
      //note is B-494Hz
      myScreen.background(0,0,255);
      delay(ampTimer);
    }
    else if(frequency > 500.5 && frequency < 525.5){
      //note is C-523Hz
      myScreen.background(127,127,0);
      delay(ampTimer);
    }
    else if(frequency > 551.5 && frequency < 556.5){
      //note is C#-554Hz
      myScreen.background(255,255,0);
      delay(ampTimer);
    }
    else if(frequency > 584.5 && frequency < 589.5){
      //note is D-587Hz
      myScreen.background(127,127,127);
      delay(ampTimer);
    }
    else if(frequency > 619.5 && frequency < 624.5){
      //note is D#-622Hz
      myScreen.background(255,255,255);
      delay(ampTimer);
    }
    else if(frequency > 656.5 && frequency < 661.5){
      //note is E-659Hz
      myScreen.background(255,0,0);
      delay(ampTimer);
    }
    else if(frequency > 695.5 && frequency < 700.5){
      //note is F-698Hz
      myScreen.background(127,0,127);
      delay(ampTimer);
    }
    else if(frequency > 737.5 && frequency < 742.5){
      //note is F#-740Hz
      myScreen.background(255,0,255);
      delay(ampTimer);
    }
    else if(frequency > 781.5 && frequency < 786.5){
      //note is G-784Hz
      myScreen.background(0,0,0);
      delay(ampTimer);
    }
    else if(frequency > 828.5 && frequency < 833.5){
      //note is G#-831Hz
      myScreen.background(5,5,5);
      delay(ampTimer);
    }
    else if(frequency > 877.5 && frequency < 882.5){
      //note is A-880Hz
      myScreen.background(127,0,0);
      delay(ampTimer);
    }
    else if(frequency > 929.5 && frequency < 934.5){
      //note is A#-932Hz
      myScreen.background(255,0,0);
      delay(ampTimer);
    }
    else if(frequency > 985.5 && frequency < 990.5){
      //note is B-988Hz
      myScreen.background(0,0,255);
      delay(ampTimer);
    }
    else if(frequency > 1044.5 && frequency < 1049.5){
      //note is C-1047Hz
      myScreen.background(127,127,0);
      delay(ampTimer);
    }
  }
}
