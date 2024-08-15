#include <iostream> // Подключение библиотеки для ввода/вывода
#include <string>   // Подключение библиотеки для работы со строками
#include "Sleep.h"
#include "Robot.h"


using namespace std;

// перечисление состояние робота
enum StateRobot {
    TURN_ON,
    TURN_OFF,
    GOING,
    FIND_MUSHROOM,
    GO_HOME
};

const char botSymbol = '@'; // символ для обозначения робота на схеме 
const char emptySymbol = '.'; // пустая клетка леса
const int sleepDuration = 1; // время задержки в секундах
const int maxBagCapacity = 10; // максимальное количество грибов в отсеке 
const int maxMushrooms = 20; // максимальное количество грибов каждого типа
const int batteryLimit = 220; // заряд батареии 


StateRobot state; // текущее состояние робота.
Forest& forest; // ссылка на объект класса Forest.


// текущие координаты робота.
int x;
int y;

// количество съедобных и галлюциногенных грибов в отсеках.
int bagEdible;
int bagHallucinogenic;

// время работы робота.
int batteryTime;

Robot(Forest& f) : x(0), y(0), bagEdible(0), bagHallucinogenic(0), batteryTime(0), state(TURN_ON), forest(f) {
    forest.grid[x][y] = botSymbol;
}

// проверяет на наличие случайных ошибок.
void Robot::checkErrors() {
    if (rand() % 1000 == 1) {
        cout << "Непредвиденная ошибка" << endl;
        turnOff();
    }
}

// функция определения типа гриба и добавляет его в соответствующий отсек.
void Robot::pickMushroom(const string& type) {
    cout << "Это " << type << " гриб" << endl;
    slp(sleepDuration);
    if (type == "съедобный" && bagEdible < maxBagCapacity) {
        cout << "Помещаю в отсек для съедобных грибов" << endl;
        bagEdible++;
    }
    else if (type == "галлюциногенный" && bagHallucinogenic < maxBagCapacity) {
        cout << "Помещаю в отсек для галлюциногенных грибов" << endl;
        bagHallucinogenic++;
    }
    else if (type == "несъедобный") {
        cout << "Продолжаю поиск" << endl;
    }
    else {
        cout << "Отсек переполнен" << endl;
    }
    slp(sleepDuration);
}

// функция движения робота
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

// функция определения что находится в клетке леса куда наступил робот и вывод схемы в консоль
void Robot::move() {
    switch (forest.grid[x][y]) {
    case emptySymbol:
        forest.grid[x][y] = botSymbol;
        break;
    case 'e':
        pickMushroom("съедобный");
        forest.removeMushroom(x, y);
        break;
    case 'h':
        pickMushroom("галлюциногенный");
        forest.removeMushroom(x, y);
        break;
    case 'i':
        pickMushroom("несъедобный");
        forest.removeMushroom(x, y);
        break;
    }
    forest.grid[x][y] = botSymbol;
    forest.display(x, y, bagEdible, bagHallucinogenic);
    slp(sleepDuration);
}

// функция возвращения домой (начальная координата)
void Robot::goHome() {
    while (x > 0 || y > 0) {
        forest.grid[x][y] = emptySymbol;
        if (x > 0) --x;
        if (y > 0) --y;
        move();
    }
}

// функция выключения робота (выход из программы)
void Robot::turnOff() {
    cout << "Робот выключен" << endl;
    exit(0);
}


// Основная функция работы робота
void Robot::runStep() {
    checkErrors();
    switch (state) {
    case TURN_ON:
        cout << "Робот включен" << endl;
        state = GOING;
        break;

    case GOING:
        if (!forest.mushrooms.empty()) {
            pair<int, int> nearestMushroom = forest.getNearestMushroom(x, y);
            int mushroomX = nearestMushroom.first;
            int mushroomY = nearestMushroom.second;
            moveTo(mushroomX, mushroomY);
            if (bagEdible >= maxBagCapacity && bagHallucinogenic >= maxBagCapacity) {
                cout << "Оба отсека переполнены\nВозвращаюсь домой" << endl;
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

// Функция проверки зарядки батареи
bool Robot::isBatteryDepleted() const {
    return batteryTime >= batteryLimit;
}

// Функция обновления времени работы батареи
void Robot::updateBatteryTime(int seconds) {
    batteryTime += seconds;
}
