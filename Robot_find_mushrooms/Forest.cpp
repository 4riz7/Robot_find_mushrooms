#include <iostream> // Подключение библиотеки для ввода/вывода
#include <string>   // Подключение библиотеки для работы со строками
#include <cstdlib>  // Подключение библиотеки для генерации случайных чисел
#include <vector>   // Подключение библиотеки для работы с векторами
#include <cmath>    // Подключение библиотеки для математических операций
#include <limits>   // Подключение библиотеки для работы с пределами чисел
#include "Forest.h"

using namespace std;

const int forestWidth = 20; // высота леса 
const int forestLength = 50; // длина леса
const char botSymbol = '@'; // символ для обозначения робота на схеме 
const char emptySymbol = '.'; // пустая клетка леса
const int maxBagCapacity = 10; // максимальное количество грибов в отсеке

// Структура для представления гриба
struct Mushroom {
    char type; // Тип гриба (съедобный, галлюциногенный, несъедобный)
    int x;
    int y;
};


vector<vector<char>> grid;  // двумерный вектор, представляющий лес
vector<Mushroom> mushrooms; //вектор, содержащий информацию о всех грибах.


// функция раcтавления грибов в рандомных местах в лесу
void Forest::placeMushrooms(char mushroomType, int count) {
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
void Forest::display(int robotX, int robotY, int bagEdible, int bagHallucinogenic) {
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
pair<int, int> Forest::getNearestMushroom(int robotX, int robotY) {
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
void Forest::removeMushroom(int x, int y) {
    mushrooms.erase(remove_if(mushrooms.begin(), mushrooms.end(),
        [x, y](const Mushroom& mushroom) {
            return mushroom.x == x && mushroom.y == y;
        }), mushrooms.end());
}
