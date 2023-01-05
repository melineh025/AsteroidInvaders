#include <FastLED.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define NUM_LEDS 256
#define DATA_PIN 8
//Joystick + buttons

#define BUTTON_UP 2
#define BUTTON_RIGHT 3
#define BUTTON_DOWN 4
#define BUTTON_LEFT 5
#define BUTTON_E 6
#define BUTTON_F 7
#define PIN_ANALOG_X 0
#define PIN_ANALOG_Y 1

volatile int x;
volatile int y;


CRGB leds[NUM_LEDS];
//const int buttonPin = 2;
//const int ledPin =  13;   
 int buttonState = 0; 

unsigned char mat[16][16] = {
  {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240},
  {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
  {223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208},
  {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
  {191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176},
  {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175},
  {159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144},
  {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143},
  {127, 126, 125, 124, 123, 122,121, 120, 119,118, 117, 116,115, 114,113,112},
  {96,97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
  {95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80},
  {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
  {63, 62, 61, 60, 59, 58 ,57 ,56, 55, 54, 53, 52, 51, 50, 49, 48},
  {32,33,34,35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
  {31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
  };
///////////////////////////
class EnemyShip {

    public:
    EnemyShip(){
      col = 0;
      row = 0;
   
      laserArray[10];
      laserCol[10];
      laserRow[10];
      movementRow[8]; //{1, 1, 0, 0, -1, -1, 0, 0};
      movementCol[8];// {0, 0, -1, -1, 0, 0, 1, 1};
      counter = 0;
      collision = false;
      firstCollision = true;
    }

     bool checkCollision(bool laserArr[10], unsigned char laserR[10], unsigned char laserC[10]) {

           for (int i =0; i<10; i++){
               if (laserArr[i]) {
                  if ((laserR[i] == row) && (laserC[i] == col)){
                      collision = true;
                  }
                  else if ((laserR[i] == row -1 ) && (laserC[i] == col)){
                      collision = true;
                  }
                  else if ((laserR[i] == row -2 ) && (laserC[i] == col)){
                      collision = true;
                  }

                  if (collision){
                    if (firstCollision){
                      leds[mat[row][col - 1]] =  CRGB::Purple;
                      leds[mat[row][col + 1]] = CRGB::Purple;
                      leds[mat[row - 1][col - 1]] =  CRGB::Purple;
                      leds[mat[row - 1][col]] =  CRGB::Purple;
                      leds[mat[row - 1][col + 1]] = CRGB::Purple;
                      leds[mat[row - 2][col]] =  CRGB::Purple;
                      FastLED.show();
                      firstCollision = false;
                    }

                  
                    leds[mat[row][col - 1]] =  CRGB::Black;
                    leds[mat[row][col + 1]] = CRGB::Black;
          
                    leds[mat[row - 1][col - 1]] =  CRGB::Black;
                    leds[mat[row - 1][col]] =  CRGB::Black;
                    leds[mat[row - 1][col + 1]] = CRGB::Black;
                    leds[mat[row - 2][col]] =  CRGB::Black;
               }
               }}

               
               return collision;

      
     }
      void place2() {
                leds[mat[row][col - 1]] =  CRGB::Red;
                leds[mat[row][col + 1]] = CRGB::Red;
        
                leds[mat[row - 1][col - 1]] =  CRGB::Red;
                leds[mat[row - 1][col]] =  CRGB::Purple;
                leds[mat[row - 1][col + 1]] = CRGB::Red;
                leds[mat[row - 2][col]] =  CRGB::Red;
                FastLED.show();
          }
         void moveObj2() {
                  
                leds[mat[row][col - 1]] =  CRGB::Black;
                leds[mat[row][col + 1]] =  CRGB::Black;
        
                leds[mat[row - 1][col - 1]] =  CRGB::Black;
                leds[mat[row - 1][col]] =  CRGB::Black;
                leds[mat[row - 1][col + 1]] =  CRGB::Black;
                leds[mat[row - 2][col]] =  CRGB::Black;
                FastLED.show();
                if (counter == 8){
                  counter = 0;
                }
      
            
                  col = col + movementCol[counter]; 
                  row = row + movementRow[counter];
                  counter++;
                  place2();
           
         }

          void shoot() {

        //shoot from current col, turning one row on and then off
        for (int i =0; i<10; i++){
          if (laserArray[i] == false) {
           laserArray[i] = true;
           laserCol[i] = col;
           laserRow[i] = row + 1;
           leds[mat[laserRow[i]][laserCol[i]]] = CRGB::Brown;
           break;
          }
        }
        //place();
        
         
        FastLED.show();
         //leds[mat[row + 2][col]] = CRGB::Pink;
         
    }
    void laserUpdate() {
        for (int i =0; i<10; i++){
          if (laserArray[i]) {
             if(laserRow[i] == 15) {
                laserArray[i] = false;
                leds[mat[laserRow[i]][laserCol[i]]] = CRGB::Black;
             }
             if (laserArray[i]){
               leds[mat[laserRow[i]][laserCol[i]]] = CRGB::Black;
               laserRow[i]++;
               leds[mat[laserRow[i]][laserCol[i]]] = CRGB::Brown;
             }
           }
         }
     FastLED.show();
          

      
    
    }
     void setVal(unsigned char r, unsigned char c){
        col = c;
        row = r;
    }

   bool collision;
   bool firstCollision;
              unsigned char laserCol[10];
              unsigned char laserRow[10];
              bool laserArray[10];
       private:
             unsigned char col;
              unsigned char row;
             
            char movementRow[8] = {1, 1, 0, 0, -1, -1, 0, 0};
            char movementCol[8] = {0, 0, -1, -1, 0, 0, 1, 1};
            unsigned char counter;
  
};

class MyShip { 
  public:
    MyShip(){
      col = 0;
      row = 0;
   
      laserArray[10];
      laserCol[10];
      laserRow[10];
      collision = false;
      firstCollision = true;
    }
    void setVal(unsigned char r, unsigned char c){
        col = c;
        row = r;
    }
    void shoot() {

        //shoot from current col, turning one row on and then off
        for (int i =0; i<10; i++){
          if (laserArray[i] == false) {
           laserArray[i] = true;
           laserCol[i] = col;
           laserRow[i] = row - 3;
           leds[mat[laserRow[i]][laserCol[i]]] = CRGB::Pink;
           break;
          }
        }
        //place();
        
         
        FastLED.show();
         //leds[mat[row + 2][col]] = CRGB::Pink;
         
    }
    void laserUpdate() {
        for (int i =0; i<10; i++){
          if (laserArray[i]) {
             if(laserRow[i] == 0) {
                laserArray[i] = false;
                leds[mat[laserRow[i]][laserCol[i]]] = CRGB::Black;
             }
             if (laserArray[i]){
               leds[mat[laserRow[i]][laserCol[i]]] = CRGB::Black;
               laserRow[i]--;
               leds[mat[laserRow[i]][laserCol[i]]] = CRGB::Pink;
             }
           }
         }
     FastLED.show();
          

      
    
    }
    void place() {
          leds[mat[row][col]] =  CRGB::Blue;
          leds[mat[row][col - 1]] =  CRGB::Blue;
          leds[mat[row][col + 1]] =  CRGB::Blue;
  
          leds[mat[row - 1][col - 1]] =  CRGB::Blue;
          leds[mat[row - 1][col]] =  CRGB::Blue;
          leds[mat[row - 1][col + 1]] =  CRGB::Blue;
          leds[mat[row - 2][col]] =  CRGB::Blue;

          
          FastLED.show();
    }
   void moveObj( char r1, char c1) {
          if ((c1 > 0) && (col == 14)){
             c1 = 0;}
          else if ((c1 < 0) && (col == 1)){
             c1 = 0;}
             
          if ((r1 > 0) && (row == 15)){
             r1 = 0;}
          else if ((r1 < 0) && (row == 9)){
             r1 = 0;}
          leds[mat[row][col]] =  CRGB::Black;
          leds[mat[row][col - 1]] =  CRGB::Black;
          leds[mat[row][col + 1]] =  CRGB::Black;
  
          leds[mat[row - 1][col - 1]] =  CRGB::Black;
          leds[mat[row - 1][col]] =  CRGB::Black;
          leds[mat[row - 1][col + 1]] =  CRGB::Black;
          leds[mat[row - 2][col]] =  CRGB::Black;
          FastLED.show();


            col = col + c1; 
            row = row + r1;
            place();
     
   }
   bool checkCollision(bool laserArr[10], unsigned char laserR[10], unsigned char laserC[10]) {

           for (int i =0; i<10; i++){
               if (laserArr[i]) {
                  if ((laserR[i] == row) && (laserC[i] == col)){
                      collision = true;
                  }
                  else if ((laserR[i] == row -1 ) && (laserC[i] == col)){
                      collision = true;
                  }
                  else if ((laserR[i] == row -2 ) && (laserC[i] == col)){
                      collision = true;
                  }

                  if (collision){
                    if (firstCollision){
                      leds[mat[row][col - 1]] =  CRGB::Purple;
                      leds[mat[row][col + 1]] = CRGB::Purple;
                      leds[mat[row - 1][col - 1]] =  CRGB::Purple;
                      leds[mat[row - 1][col]] =  CRGB::Purple;
                      leds[mat[row - 1][col + 1]] = CRGB::Purple;
                      leds[mat[row - 2][col]] =  CRGB::Purple;
                      FastLED.show();
                      firstCollision = false;
                    }

                  
                    leds[mat[row][col - 1]] =  CRGB::Black;
                    leds[mat[row][col + 1]] = CRGB::Black;
          
                    leds[mat[row - 1][col - 1]] =  CRGB::Black;
                    leds[mat[row - 1][col]] =  CRGB::Black;
                    leds[mat[row - 1][col + 1]] = CRGB::Black;
                    leds[mat[row - 2][col]] =  CRGB::Black;
               }
               }}

               
               return collision;

      
     }
   
    unsigned char laserCol[10];
    unsigned char laserRow[10];
   bool laserArray[10];
   
   bool collision;
   bool firstCollision;
   
  private:
    unsigned char col;
    unsigned char row;
   // unsigned char laserCol[10];
   // unsigned char laserRow[10];
  //  bool laserArray[10];
  
};



//////////////////////////
typedef struct _task {

        signed char state;
        unsigned long int period;
        unsigned long int elapsedTime;
        int (*TickFct)(int);

} task;
//tasks
static task  task1, task2, task3, task4, task5;
task *tasks[] = { &task1, &task2, &task3, &task4, &task5};
enum SM_States {SM_On, SM_Off};
enum FM_Ship {FM_Show, FM_S1,FM_Init};
enum LM_Enemy{E_Init, E_Show, E_Fall};
enum SM_Bullet {SM2_On, SM2_Off};
enum SM_EnemyBullet {SM3_Shoot, SM3_Update};

//const int LED_pin = 13; 
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerSet(unsigned long M) {
  _avr_timer_M = M;
  _avr_timer_cntcurr = _avr_timer_M;

}

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(8);
  //pinMode(buttonPin, INPUT);
  //pinMode(ledPin, OUTPUT);
 Serial.begin(9600);
 pinMode(BUTTON_UP, INPUT);
 digitalWrite(BUTTON_UP, HIGH);
 pinMode(BUTTON_RIGHT, INPUT);
 digitalWrite(BUTTON_RIGHT, HIGH);
 pinMode(BUTTON_DOWN, INPUT);
 digitalWrite(BUTTON_DOWN, HIGH);
 pinMode(BUTTON_LEFT, INPUT);
 digitalWrite(BUTTON_LEFT, HIGH);
 pinMode(BUTTON_E, INPUT);
 digitalWrite(BUTTON_E, HIGH);
 pinMode(BUTTON_F, INPUT);
 digitalWrite(BUTTON_F, HIGH);

  
  //digitalWrite(LED_pin, LOW);
  TCCR1B |= (1 << WGM02) | (1 << CS01) | (1 << CS00);
  OCR1A = 0xF9;
  TCNT1 = 0;
  TIMSK1 |= (1 << OCIE0A);
  _avr_timer_cntcurr = _avr_timer_M;
  SREG |= 0x80;
  TimerSet(1) ; //set interval to 1 ms
  //

 
  task1.state = SM_On;
  task1.period = 1;
  task1.elapsedTime = task1.period;
  task1.TickFct = &flash;


  task2.state = FM_Init;
  task2.period = 65;
  task2.elapsedTime = task2.period;
  task2.TickFct = &Ship;


  task3.state = E_Init;
  task3.period = 100;
  task3.elapsedTime = task3.period;
  task3.TickFct = &Enemy;

  task4.state = SM2_On;
  task4.period = 75;
  task4.elapsedTime = task4.period;
  task4.TickFct = &LaserShoot;
  
  task5.state = SM3_Shoot;
  task5.period = 100;
  task5.elapsedTime = task5.period;
  task5.TickFct = &EnemyLaserShoot;

}

void TimerISR() {
  TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
  _avr_timer_cntcurr--;
  if(_avr_timer_cntcurr == 0)
  {
    TimerISR();
    _avr_timer_cntcurr = _avr_timer_M;
  }

}


MyShip obj1;
EnemyShip enemy1, enemy2, enemy3, enemy4;

int Ship(int state) {
  
  
  switch(state) {
      case FM_Init:
            obj1.setVal(14,7);
            obj1.place();
            state = FM_Show;
            break;
      case FM_Show:
          //if (buttonState == HIGH) {
            state = FM_S1;
          //}   
          break; 
      case FM_S1:
         bool e1Collisi = obj1.checkCollision(enemy1.laserArray, enemy1.laserRow, enemy1.laserCol);
         bool e2Collisi = obj1.checkCollision(enemy2.laserArray, enemy2.laserRow, enemy2.laserCol);
         bool e3Collisi = obj1.checkCollision(enemy3.laserArray, enemy3.laserRow, enemy3.laserCol);
         if((!e1Collisi) && (!e2Collisi) && (!e3Collisi)) {
       
              //check to see if we should shoot
              if( digitalRead(BUTTON_UP) == LOW) {
                    obj1.shoot();
                
              }
              char moveRightLeft = 0;
              char moveUpDown = 0;
      
               if ( x > 600 ){
                  moveRightLeft = 1;}
               else if ( x < 400 ){
                  moveRightLeft = -1;}
               if ( y > 600 ){
                  moveUpDown = -1;}
               else if ( y < 400 ){
                  moveUpDown = 1;}
      
               obj1.moveObj(moveUpDown, moveRightLeft);
         }
           
   
         
           //state = FM_Show;
            break;
  }
    switch(state) {
      case FM_Init:
          break;
      case FM_Show:  
        break;
       case FM_S1:
          break;
        
    
  }
  return state;
}



int flash(int state)
{
  switch(state){
    case SM_On:
      state = SM_On;
      break;
    case SM_Off:
      state = SM_On;
      break;
  }
  switch(state){
    case SM_On:
        if(digitalRead(BUTTON_UP) == LOW) {
                 // Serial.println("UP");
        }
        else if(digitalRead(BUTTON_RIGHT) == LOW) {

            // Serial.println("RIGHT");
        }
       else if(digitalRead(BUTTON_DOWN) == LOW) {

          // Serial.println("DOWN");
      }
      else if(digitalRead(BUTTON_LEFT) == LOW) {

          //  Serial.println("LEFT");   
      }
      //Serial.print("x: ");
     // Serial.println(analogRead(PIN_ANALOG_X));
      x = analogRead(PIN_ANALOG_X);
       // Print y axis values
      // Serial.print("y: ");
      // Serial.println(analogRead(PIN_ANALOG_Y));
      y = analogRead(PIN_ANALOG_Y);

      
      break;
    case SM_Off:
      break;
  }

return state;
}

///enemy generation
int Enemy( int state) {
  switch(state) {
      case E_Init:
          enemy1.setVal(3,4);
          enemy2.setVal(4,8);
          enemy3.setVal(3,12);
          enemy1.place2();
          enemy2.place2();
          enemy3.place2();
          state = E_Show;
        break;
      case E_Show:
        //unsigned char element = obj1.laserRow[0];
       bool e1Collision = enemy1.checkCollision(obj1.laserArray, obj1.laserRow, obj1.laserCol);
        bool e2Collision = enemy2.checkCollision(obj1.laserArray, obj1.laserRow, obj1.laserCol);
       bool  e3Collision = enemy3.checkCollision(obj1.laserArray, obj1.laserRow, obj1.laserCol);
       if(!e1Collision) {
        enemy1.moveObj2();
       
       }
       //else play sound
       if(!e2Collision) {
        enemy2.moveObj2();
       
       }
       if(!e3Collision) {
        enemy3.moveObj2();
       
       }
        break;
      case E_Fall:
         break;

        
  }
    switch(state) {
      case E_Init:
        break;
      case E_Show:
        break;
      case E_Fall:
         break;

        
  }
  
  return state;
}



//enum SM_Bullet {SM2_On, SM2_Off};

int LaserShoot(int state) {

    switch(state) {
          case SM2_On:
              obj1.laserUpdate();
             // state = SM2_Off;
              break;
          case SM2_Off:
               //state = SM2_On;
               break; 
    }
    
    switch(state) {
          case SM2_On:
              break;
          case SM2_Off:
               break;

      
    }

  return state;
}


bool e1Collis, e2Collis, e3Collis;
int EnemyLaserShoot(int state) {

    switch(state) {
      
              
               
          case SM3_Shoot: 
              if ((random(1,12) < 3) && (!e1Collis)){
                enemy1.shoot();
              }
              if ((random(1,12) < 3) && (!e3Collis)){
                enemy2.shoot();
              }
              if ((random(1,12) < 3) && (!e3Collis)){
                enemy3.shoot();
              }
              state = SM3_Update;
              break;
              
          case SM3_Update:
               e1Collis = enemy1.checkCollision(obj1.laserArray, obj1.laserRow, obj1.laserCol);
               e2Collis = enemy2.checkCollision(obj1.laserArray, obj1.laserRow, obj1.laserCol);
               e3Collis = enemy3.checkCollision(obj1.laserArray, obj1.laserRow, obj1.laserCol);
                
               enemy1.laserUpdate();
               enemy2.laserUpdate();
               enemy3.laserUpdate();
               state = SM3_Shoot;
               break; 
    }
    
    switch(state) {
          case SM3_Shoot:
              break;
          case SM3_Update:
               break;

      
    }

  return state;
}






const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
void loop() {
   
     //buttonState = digitalRead(buttonPin);
    //task scheduler code
  for(int i = 0; i < numTasks; i++) {
    if(tasks[i]->elapsedTime == tasks[i]->period) {
      tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
      tasks[i]->elapsedTime = 0;
    }
    tasks[i]->elapsedTime += 1; //GCD

  }   
  while(!TimerFlag);
  TimerFlag = 0;

  
}

