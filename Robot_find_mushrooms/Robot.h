#pragma once

#ifndef Robot_H
#define Robot_h

class Forest;

class Robot {
private:
	void checkErrors();
	void pickMushroom(const string& type);
	void moveTo(int targetX, int targetY);
	void move();
	void goHome();
	void turnOff();
public:
	void runStep();
	bool isBatteryDepleted() const;
	void updateBatteryTime(int seconds);
};
#endif