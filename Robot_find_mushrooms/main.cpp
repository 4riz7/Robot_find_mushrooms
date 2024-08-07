#include <iostream> // Подключение библиотеки для ввода/вывода
#include <chrono>   // Подключение библиотеки для работы с временем
#include <thread>   // Подключение библиотеки для работы с потоками
#include <string>   // Подключение библиотеки для работы со строками
#include <cstdlib>  // Подключение библиотеки для генерации случайных чисел
#include <vector>   // Подключение библиотеки для работы с векторами
#include <cmath>    // Подключение библиотеки для математических операций
#include <limits>   // Подключение библиотеки для работы с пределами чисел

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

// Структура для представления гриба
struct Mushroom {
    char type; // Тип гриба (съедобный, галлюциногенный, несъедобный)
    int x;
    int y;
};

// Класс для представления леса
class Forest {
public:
    vector<vector<char>> grid;  // двумерный вектор, представляющий лес
    vector<Mushroom> mushrooms; //вектор, содержащий информацию о всех грибах.

    Forest(int width, int length) {
        grid.resize(width, vector<char>(length, emptySymbol));
    }

    // функция раставления грибов в рандомных местах в лесу
    void placeMushrooms(char mushroomType, int count) {
        while (count--) {
            int x = rand() % grid.size();
            int y = rand() % grid[0].size();
            if (x != 0 || y != 0) {
                if (grid[x][y] == emptySymbol) {
                    grid[x][y] = mushroomType;
                    mushrooms.push_back({ mushroomType, x, y });
                }
            }
        }
    }

    // отображает текущее состояние леса и робота в консоли.
    void display(int robotX, int robotY, int bagEdible, int bagHallucinogenic) {
        system("cls"); // очищаем консоль
        for (const auto& row : grid) {
            for (char cell : row) {
                cout << cell << ' ';
            }
            cout << endl;
        }
        // вывод информации координат робота и сколько грибов в каком отсеке 
        cout << "\nКоординаты робота: " << robotY << ":" << robotX << endl;
        cout << "В отсеке для съедобных грибов: " << min(bagEdible, maxBagCapacity) << endl;
        cout << "В отсеке для галлюциногенных грибов: " << min(bagHallucinogenic, maxBagCapacity) << "\n\n" << endl;
    }

    // находит ближайший гриб к текущей позиции робота.
    pair<int, int> getNearestMushroom(int robotX, int robotY) {
        double minDistance = forestLength * forestWidth; // реальная дистанция точно будет меньше.
        pair<int, int> nearestMushroom = { -1, -1 }; // nearestMushroom инициализируется значением (-1, -1), что служит индикатором, когда ближайший гриб ещё не найден.

        for (const auto& mushroom : mushrooms) {
            double distance = sqrt(pow(mushroom.x - robotX, 2) + pow(mushroom.y - robotY, 2));  // вычисление расстояния по формуле евклидового расстояния
            if (distance < minDistance) {
                minDistance = distance;
                nearestMushroom = { mushroom.x, mushroom.y };
            }
        }
        return nearestMushroom;
    }

    // удаляет гриб из списка грибов и из грида.
    void removeMushroom(int x, int y) {
        mushrooms.erase(remove_if(mushrooms.begin(), mushrooms.end(),
            [x, y](const Mushroom& mushroom) {
                return mushroom.x == x && mushroom.y == y;
            }), mushrooms.end());
    }
};

class Robot {
private:
    // текущие координаты робота.
    int x;
    int y;

    // количество съедобных и галлюциногенных грибов в отсеках.
    int bagEdible;
    int bagHallucinogenic;

    // время работы робота.
    int batteryTime;

    StateRobot state; // текущее состояние робота.
    Forest& forest; // ссылка на объект класса Forest.

    // проверяет на наличие случайных ошибок.
    void checkErrors() {
        if (rand() % 1000 == 1) {
            cout << "Непредвиденная ошибка" << endl;
            turnOff();
        }
    }

    // функция определения типа гриба и добавляет его в соответствующий отсек.
    void pickMushroom(const string& type) {
        cout << "Это " << type << " гриб" << endl;
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
    }

    // функция движения робота
    void moveTo(int targetX, int targetY) {
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
    void move() {
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
    }

    // функция возвращения домой (начальная координата)
    void goHome() {
        while (x > 0 || y > 0) {
            forest.grid[x][y] = emptySymbol;
            if (x > 0) --x;
            if (y > 0) --y;
            move();
        }
    }

    // функция выключения робота (выход из программы)
    void turnOff() {
        cout << "Робот выключен" << endl;
        exit(0);
    }

public:
    Robot(Forest& f) : x(0), y(0), bagEdible(0), bagHallucinogenic(0), batteryTime(0), state(TURN_ON), forest(f) {
        forest.grid[x][y] = botSymbol;
    }

    // Основная функция работы робота
    void runStep() {
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
    bool isBatteryDepleted() const {
        return batteryTime >= batteryLimit;
    }

    // Функция обновления времени работы батареи
    void updateBatteryTime(int seconds) {
        batteryTime += seconds;
    }
};

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
        this_thread::sleep_for(chrono::seconds(sleepDuration));
        robot.updateBatteryTime(sleepDuration);
    }

    return 0;
}
