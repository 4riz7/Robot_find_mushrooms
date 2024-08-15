#include <iostream> // ����������� ���������� ��� �����/������
#include <string>   // ����������� ���������� ��� ������ �� ��������
#include "Robot.h"
#include "Forest.h"
#include "Sleep.h"


using namespace std;

// ������������ ��������� ������
enum StateRobot {
    TURN_ON,
    TURN_OFF,
    GOING,
    FIND_MUSHROOM,
    GO_HOME
};

// ��������� ��� ���� � ������
const int forestWidth = 20; // ������ ���� 
const int forestLength = 50; // ����� ����

const char botSymbol = '@'; // ������ ��� ����������� ������ �� ����� 
const char emptySymbol = '.'; // ������ ������ ����
const int sleepDuration = 1; // ����� �������� � ��������
const int maxBagCapacity = 10; // ������������ ���������� ������ � ������ 
const int maxMushrooms = 20; // ������������ ���������� ������ ������� ����
const int batteryLimit = 220; // ����� �������� 

const int sleepDuration = 1; // ����� �������� � ��������


// �������� �������
int main() {
    setlocale(LC_ALL, "RU"); // ������ ������� ������� ����
    srand(time(0)); // ��� ������ ����� ������� ��������� ������� rand ������ ����� ��������

    Forest forest(forestWidth, forestLength);
    forest.placeMushrooms('e', rand() % maxMushrooms); // ���������� ��������� ������ � ���
    forest.placeMushrooms('h', rand() % maxMushrooms); // ���������� ��������������� ������ � ���
    forest.placeMushrooms('i', rand() % maxMushrooms); // ���������� ����������� ������ � ���
    forest.display(0, 0, 0, 0);

    Robot robot(forest);

    // �������� ���� ������ ������
    while (true) {  
        robot.runStep();
        if (robot.isBatteryDepleted()) {
            cout << "������� ����" << endl;
            break;
        }
        slp(sleepDuration);
        robot.updateBatteryTime(sleepDuration);
    }

    return 0;
}
