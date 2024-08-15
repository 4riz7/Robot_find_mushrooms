#include <iostream> // Подключение библиотеки для ввода/вывода
#include <string>   // Подключение библиотеки для работы со строками
#include "Robot.h"
#include "Forest.h"
#include "Sleep.h"


using namespace std;

// перечисление состояние робота
enum StateRobot {
    TURN_ON,
    TURN_OFF,
    GOING,
    FIND_MUSHROOM,
    GO_HOME
};

// Константы для леса и робота
const int forestWidth = 20; // высота леса 
const int forestLength = 50; // длина леса

const char botSymbol = '@'; // символ для обозначения робота на схеме 
const char emptySymbol = '.'; // пустая клетка леса
const int sleepDuration = 1; // время задержки в секундах
const int maxBagCapacity = 10; // максимальное количество грибов в отсеке 
const int maxMushrooms = 20; // максимальное количество грибов каждого типа
const int batteryLimit = 220; // заряд батареии 

const int sleepDuration = 1; // время задержки в секундах


// Основная функция
int main() {
    setlocale(LC_ALL, "RU"); // теперь выводит русский язык
    srand(time(0)); // при каждом новом запуске программы функция rand выдает новое значение

    Forest forest(forestWidth, forestLength);
    forest.placeMushrooms('e', rand() % maxMushrooms); // добавление съедобных грибов в лес
    forest.placeMushrooms('h', rand() % maxMushrooms); // добавление галлюциногенных грибов в лес
    forest.placeMushrooms('i', rand() % maxMushrooms); // добавление несъедобных грибов в лес
    forest.display(0, 0, 0, 0);

    Robot robot(forest);

    // Основной цикл работы робота
    while (true) {  
        robot.runStep();
        if (robot.isBatteryDepleted()) {
            cout << "Батарея села" << endl;
            break;
        }
        slp(sleepDuration);
        robot.updateBatteryTime(sleepDuration);
    }

    return 0;
}
