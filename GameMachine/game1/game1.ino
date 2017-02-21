
#include "U8glib.h"

#define MAX 20

U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

int KEY_UP = 7;
int KEY_DOWN = 6;
int KEY_LEFT = 8;
int KEY_RIGHT = 5;
int KEY_A = 4;
int KEY_B = 3;

int spaceShipX = 64;
int spaceShipY = 32;


struct PointStruct;
typedef struct PointStruct Point;
struct PointStruct{
  int x;
  int y;
  bool isDead;
  int index;
};

void AddPoint(Point* pointArray[], int* ptrCurrentIndex, Point* ptrNewPoint){
  if(*ptrCurrentIndex < MAX){
    ptrNewPoint -> isDead = false;
    ptrNewPoint -> index = (*ptrCurrentIndex);
    pointArray[*ptrCurrentIndex] = ptrNewPoint;
    (*ptrCurrentIndex) ++;
  } else {
    // array is full, clear the dead
    Point* temp[MAX];
    int tempIndex = 0;
    for(int i = 0 ; i < MAX; i ++){
      temp[i] = NULL;
    }
    for(int i = 0 ; i < MAX ;i ++){
      if(pointArray[i] -> isDead == false){
        temp[tempIndex++] = pointArray[i];
      }
    }
    (*ptrCurrentIndex) = tempIndex;
    pointArray = temp;
  }
}

void DeletePoint(Point* pointArray[], int deleteIndex){
  if(deleteIndex < MAX && pointArray[deleteIndex] != NULL){
    pointArray[deleteIndex] -> isDead = true;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Bullets, Spaceships, etc.

Point* bullets[MAX];
int currentBulletCount = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
  drawStars();
  drawSpaceShip();
  drawBullets();
}

void drawStars(void){
  // Each star will generated from right side of the screen, then move to right with random speed
}

void drawSpaceShip(void){
  // Hero
  u8g.drawLine(spaceShipX, spaceShipY - 2, spaceShipX, spaceShipY + 2);
  u8g.drawLine(spaceShipX, spaceShipY - 2, spaceShipX + 5, spaceShipY);
  u8g.drawLine(spaceShipX, spaceShipY + 2, spaceShipX + 5, spaceShipY);
}

void drawBullets(void){
  if(bullets != NULL){
    for(int i = 0 ;i <= currentBulletCount ;i ++){
      if(! bullets[i] -> isDead){
        int x = bullets[i] -> x;
        int y = bullets[i] -> y;
        u8g.drawLine(x - 2, y, x + 2, y );
        //(bullets[i] -> x) ++;
        //(bullets[i] -> y) ++;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(void) {

  // flip screen, if required
  // u8g.setRot180();

 // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }

  // KEYS
  pinMode(KEY_UP, INPUT_PULLUP);
  pinMode(KEY_DOWN, INPUT_PULLUP);
  pinMode(KEY_LEFT, INPUT_PULLUP);
  pinMode(KEY_RIGHT, INPUT_PULLUP);
  pinMode(KEY_A, INPUT_PULLUP);
  pinMode(KEY_B, INPUT_PULLUP);

  // Debug
  Serial.begin(9600);

  // init;
  // Make all point arraies as NULL
  for(int i = 0 ; i < MAX ; i ++){
    bullets[i] = NULL;
  }
}

int lastAState = HIGH;

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
    int upKeyState = digitalRead(KEY_UP);
    int downKeyState = digitalRead(KEY_DOWN);
    int leftKeyState = digitalRead(KEY_LEFT);
    int rightKeyState = digitalRead(KEY_RIGHT);
    int aKeyState = digitalRead(KEY_A);
    int bKeyState = digitalRead(KEY_B);
    if(upKeyState == LOW){
      if(spaceShipY > 5){
        spaceShipY --;
      }
    }
    if(downKeyState == LOW){
      if(spaceShipY < 59){
        spaceShipY ++;
      }
    }
    if(leftKeyState == LOW){
      if(spaceShipX > 5){
        spaceShipX --;
      }
    }
    if(rightKeyState == LOW){
      if(spaceShipX < 123){
        spaceShipX ++;
      }
    }
    if(aKeyState == LOW && lastAState == HIGH){
      Point newBullet;
      newBullet.x = spaceShipX;
      newBullet.y = spaceShipY;
      AddPoint(bullets, &currentBulletCount, &newBullet);
      lastAState = LOW;

      // Test

      Serial.println(" -- new : -- ");

      Serial.println(newBullet.index);
      Serial.println(newBullet.x);
      Serial.println(newBullet.y);
      Serial.println(newBullet.isDead);

      Serial.println("***");
      for(int i = 0 ; i < currentBulletCount; i ++){
        Serial.println(bullets[i] -> index);
        Serial.println(bullets[i] -> x);
        Serial.println(bullets[i] -> y);
        Serial.println(bullets[i] -> isDead);
      }

    } else if (aKeyState == HIGH){
    	lastAState = HIGH;
    }


    //if(bKeyState == LOW){
    //  u8g.drawBox(90, 50, 10, 10);
    //}
  } while( u8g.nextPage() );

  // rebuild the picture after some delay
  delay(50);
}


