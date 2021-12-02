#include <wiringPi.h>
#include <iostream>

int main(int, char**)
{
wiringPiSetupGpio();
pinMode(4,OUTPUT);
pinMode(17,OUTPUT);
pinMode(27,OUTPUT);
pinMode(22,OUTPUT);
pinMode(12,OUTPUT);
pinMode(16,OUTPUT);
digitalWrite(4,HIGH);
digitalWrite(17,HIGH);
digitalWrite(27,HIGH);
digitalWrite(22,HIGH);
digitalWrite(12,LOW);
digitalWrite(16,LOW);
	while(1)
	{
	//digitalWrite(4,LOW);
	//digitalWrite(17,LOW);	
	//digitalWrite(22,LOW);
	//digitalWrite(27,LOW);
	//delay(500);
	//digitalWrite(27,HIGH);
	//digitalWrite(4,HIGH);
	//digitalWrite(17,HIGH);	
	//digitalWrite(22,HIGH);
	//digitalWrite(12,HIGH);
	//digitalWrite(16,HIGH);
	//delay(500);

	}

return 0;
}
