#ifndef Motors_h
#define Motors_h
#include "Arduino.h"

class Motors {
public:
	Motors(int PWMA, int IN1, int IN2, int IN3, int IN4, int PWMB);
	void move(int velA, int velB);
private:
	int _pin;
};
#endif