#include <iostream> // ����������� ���������� ��� �����/������
#include <string>   // ����������� ���������� ��� ������ �� ��������
#include "Sleep.h"
#include "Robot.h"


using namespace std;

// ������������ ��������� ������
enum StateRobot {
    TURN_ON,
    TURN_OFF,
    GOING,
    FIND_MUSHROOM,
    GO_HOME
};

const char botSymbol = '@'; // ������ ��� ����������� ������ �� ����� 
const char emptySymbol = '.'; // ������ ������ ����
const int sleepDuration = 1; // ����� �������� � ��������
const int maxBagCapacity = 10; // ������������ ���������� ������ � ������ 
const int maxMushrooms = 20; // ������������ ���������� ������ ������� ����
const int batteryLimit = 220; // ����� �������� 


StateRobot state; // ������� ��������� ������.
Forest& forest; // ������ �� ������ ������ Forest.


// ������� ���������� ������.
int x;
int y;

// ���������� ��������� � ��������������� ������ � �������.
int bagEdible;
int bagHallucinogenic;

// ����� ������ ������.
int batteryTime;

Robot(Forest& f) : x(0), y(0), bagEdible(0), bagHallucinogenic(0), batteryTime(0), state(TURN_ON), forest(f) {
    forest.grid[x][y] = botSymbol;
}

// ��������� �� ������� ��������� ������.
void Robot::checkErrors() {
    if (rand() % 1000 == 1) {
        cout << "�������������� ������" << endl;
        turnOff();
    }
}

// ������� ����������� ���� ����� � ��������� ��� � ��������������� �����.
void Robot::pickMushroom(const string& type) {
    cout << "��� " << type << " ����" << endl;
    slp(sleepDuration);
    if (type == "���������" && bagEdible < maxBagCapacity) {
        cout << "������� � ����� ��� ��������� ������" << endl;
        bagEdible++;
    }
    else if (type == "���������������" && bagHallucinogenic < maxBagCapacity) {
        cout << "������� � ����� ��� ��������������� ������" << endl;
        bagHallucinogenic++;
    }
    else if (type == "�����������") {
        cout << "��������� �����" << endl;
    }
    else {
        cout << "����� ����������" << endl;
    }
    slp(sleepDuration);
}

// ������� �������� ������
void Robot::moveTo(int targetX, int targetY) {
    while (x != targetX || y != targetY) {
        forest.grid[x][y] = emptySymbol;
        if (x < targetX) ++x;
        else if (x > targetX) --x;
        if (y < targetY) ++y;
        else if (y > targetY) --y;
        move();
    }
}

// ������� ����������� ��� ��������� � ������ ���� ���� �������� ����� � ����� ����� � �������
void Robot::move() {
    switch (forest.grid[x][y]) {
    case emptySymbol:
        forest.grid[x][y] = botSymbol;
        break;
    case 'e':
        pickMushroom("���������");
        forest.removeMushroom(x, y);
        break;
    case 'h':
        pickMushroom("���������������");
        forest.removeMushroom(x, y);
        break;
    case 'i':
        pickMushroom("�����������");
        forest.removeMushroom(x, y);
        break;
    }
    forest.grid[x][y] = botSymbol;
    forest.display(x, y, bagEdible, bagHallucinogenic);
    slp(sleepDuration);
}

// ������� ����������� ����� (��������� ����������)
void Robot::goHome() {
    while (x > 0 || y > 0) {
        forest.grid[x][y] = emptySymbol;
        if (x > 0) --x;
        if (y > 0) --y;
        move();
    }
}

// ������� ���������� ������ (����� �� ���������)
void Robot::turnOff() {
    cout << "����� ��������" << endl;
    exit(0);
}


// �������� ������� ������ ������
void Robot::runStep() {
    checkErrors();
    switch (state) {
    case TURN_ON:
        cout << "����� �������" << endl;
        state = GOING;
        break;

    case GOING:
        if (!forest.mushrooms.empty()) {
            pair<int, int> nearestMushroom = forest.getNearestMushroom(x, y);
            int mushroomX = nearestMushroom.first;
            int mushroomY = nearestMushroom.second;
            moveTo(mushroomX, mushroomY);
            if (bagEdible >= maxBagCapacity && bagHallucinogenic >= maxBagCapacity) {
                cout << "��� ������ �����������\n����������� �����" << endl;
                state = GO_HOME;
            }
        }
        else {
            state = GO_HOME;
        }
        break;

    case GO_HOME:
        goHome();
        state = TURN_OFF;
        break;

    case TURN_OFF:
        turnOff();
        break;
    }
}

// ������� �������� ������� �������
bool Robot::isBatteryDepleted() const {
    return batteryTime >= batteryLimit;
}

// ������� ���������� ������� ������ �������
void Robot::updateBatteryTime(int seconds) {
    batteryTime += seconds;
}
