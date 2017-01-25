#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

/**
 * First, the NRF24L01 module
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 */

// Then we have a GPS example
// * MUST upload before pin connection *
// Vcc -> 5V
// GND -> GND
// TXD -> D3  (RX:D3, TX:D4)
// Use software serial to connect between GPS and Arduino

// 1602 Screen:
// RS,E,D5,D6,D7,D8 -> 10,9,6,5,4,2

char nmeaSentence[68];
String latitude;    //纬度
String longitude;   //经度
String lndSpeed;    //速度
String gpsTime;     //UTC时间，本初子午线经度0度的时间，和北京时间差8小时
String beiJingTime;   //北京时间

SoftwareSerial GPSSerial(3, 4); // RX, TX

LiquidCrystal lcd(10,9,6,5,4,2); // RS,E,D5,D6,D7,D8

void setup(){

	//===========================================
	//	The Setup of GPS
	//===========================================
	
	GPSSerial.begin(9600);
	
	//===========================================
	//	The Setup of NRF240L01
	//===========================================
	
	Serial.begin(9600);
	Mirf.spi = &MirfHardwareSpi;
	Mirf.init();
	Mirf.setRADDR((byte *)"clie1");
	Mirf.payload = sizeof(unsigned long);
	Mirf.config();

	Serial.println("Beginning ... "); 

	//===========================================
	//	The Setup of 1602 Screen
	//===========================================
	lcd.begin(16, 2);
	lcd.print("Starting");
	lcd.setCursor(0, 1);
	lcd.print(millis() / 1000);
}

void loop(){

	// Keep searching for 1 sec
	for (unsigned long start = millis(); millis() - start < 1000;)	//一秒钟内不停扫描GPS信息
	{
		lcd.clear();
		lcd.print("Searching");
		while (GPSSerial.available())	//串口获取到数据开始解析
	    {
		    char c = GPSSerial.read();	//读取一个字节获取的数据
		    switch(c)					//判断该字节的值
		    {
		    case '$':					//若是$，则说明是一帧数据的开始
		    	GPSSerial.readBytesUntil('*', nmeaSentence, 67);		//读取接下来的数据，存放在nmeaSentence字符数组中，最大存放67个字节
		    	Serial.println(nmeaSentence);
		    	lcd.clear();
		    	lcd.print(nmeaSentence);
		    	for(int i = 0 ; i < 68; i ++){
		    		Mirf.send(&nmeaSentence[i]);
					while(Mirf.isSending()){}
					delay(10);
		    	}
		    	// Send a stop node
		    	char stopSign = '|';
		    	Mirf.send(&stopSign);
		    	while(Mirf.isSending()){}
		    	delay(10);
		    	lcd.clear();
		    	lcd.print("Data Sent");
	      	}
	    }
	}

	// Sending Str Test;
	//char test[10] = "123456789";
	//for(int i = 0 ; i < 10 ; i ++){
	//	Mirf.send(&test[i]);
	//	Serial.print(test[i]);
	//	while(Mirf.isSending()){}
	//	delay(100);
	//}
} 

// sends a string via the nRF24L01
void transmit(const char *string)
{
  byte c; 
  
  for( int i=0 ; string[i]!=0x00 ; i++ )
  { 
    c = string[i];
    Mirf.send(&c);
    while( Mirf.isSending() ) ;
  }
}





