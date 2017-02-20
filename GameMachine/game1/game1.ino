
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

int KEY_UP = 7;
int KEY_DOWN = 6;
int KEY_LEFT = 8;
int KEY_RIGHT = 5;
int KEY_A = 4;
int KEY_B = 3;

int spaceShipX = 64;
int spaceShipY = 32;


// The Position List
// Each Element on it is a point node.
// You can create a list of points representing bullets, enemies, etc.
struct PointNodeStruct;
typedef PointNodeStruct PointNode;
struct PointNodeStruct{
  int x;
  int y;
  PointNode* next; 
};

// Append, Delete

// Append to first, it's easy
void AddNode(PointNode** head, PointNode* newNode){
	if (*head == NULL){
		newNode->next = NULL;
		*head = newNode;
		Serial.println("Add first node");
	} else {
		newNode->next = *head;
		*head = newNode;
		Serial.println("Add more node");
	}
}

// Delete the given one
void DeletePoint(PointNode** head, PointNode* theNode){
	PointNode* current = *head;
	PointNode* pre = NULL;
	while (current->next != NULL){
		if (current == theNode){
			if (pre == NULL){
				*head = current->next;
				current->next = NULL;
				free(current);
			}
			else {
				pre->next = current->next;
				current->next = NULL;
				free(current);
			}
		}
		else{
			pre = current;
			current = current->next;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Bullets, Spaceships, etc.

PointNode* bullets = NULL;

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
    PointNode* currentBullet = bullets;
    while(currentBullet != NULL){
      u8g.drawLine(currentBullet->x - 2, currentBullet->y, currentBullet->x + 2, currentBullet->y);
      // update after drawit
      currentBullet->x++;
      currentBullet = currentBullet->next;
      if(currentBullet->next != NULL){
      	//Serial.println("not last");
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
      PointNode newBullet;
      newBullet.x = spaceShipX;
      newBullet.y = spaceShipY;
      AddNode(&bullets, &newBullet);
      lastAState = LOW;
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


