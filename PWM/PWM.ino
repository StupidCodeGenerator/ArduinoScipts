int potValue = 255;
int pwmPin = 3;

int loopState = 0; // 0 means up

void setup() {
  // put your setup code here, to run once:
  pinMode(pwmPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(potValue >= 255){
  	loopState = 1;
  } 
  if(potValue <= 0){
  	loopState = 0;
  }
  analogWrite(pwmPin, potValue);

  if(loopState == 0){
  	potValue++;
  } else {
  	potValue--;	
  }

  delay(5);
}

