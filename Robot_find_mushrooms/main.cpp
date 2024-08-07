#include <iostream> // ����������� ���������� ��� �����/������
#include <chrono>   // ����������� ���������� ��� ������ � ��������
#include <thread>   // ����������� ���������� ��� ������ � ��������
#include <string>   // ����������� ���������� ��� ������ �� ��������
#include <cstdlib>  // ����������� ���������� ��� ��������� ��������� �����
#include <vector>   // ����������� ���������� ��� ������ � ���������
#include <cmath>    // ����������� ���������� ��� �������������� ��������
#include <limits>   // ����������� ���������� ��� ������ � ��������� �����

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

// ��������� ��� ������������� �����
struct Mushroom {
    char type; // ��� ����� (���������, ���������������, �����������)
    int x;
    int y;
};

// ����� ��� ������������� ����
class Forest {
public:
    vector<vector<char>> grid;  // ��������� ������, �������������� ���
    vector<Mushroom> mushrooms; //������, ���������� ���������� � ���� ������.

    Forest(int width, int length) {
        grid.resize(width, vector<char>(length, emptySymbol));
    }

    // ������� ����������� ������ � ��������� ������ � ����
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

    // ���������� ������� ��������� ���� � ������ � �������.
    void display(int robotX, int robotY, int bagEdible, int bagHallucinogenic) {
        system("cls"); // ������� �������
        for (const auto& row : grid) {
            for (char cell : row) {
                cout << cell << ' ';
            }
            cout << endl;
        }
        // ����� ���������� ��������� ������ � ������� ������ � ����� ������ 
        cout << "\n���������� ������: " << robotY << ":" << robotX << endl;
        cout << "� ������ ��� ��������� ������: " << min(bagEdible, maxBagCapacity) << endl;
        cout << "� ������ ��� ��������������� ������: " << min(bagHallucinogenic, maxBagCapacity) << "\n\n" << endl;
    }

    // ������� ��������� ���� � ������� ������� ������.
    pair<int, int> getNearestMushroom(int robotX, int robotY) {
        double minDistance = forestLength * forestWidth; // �������� ��������� ����� ����� ������.
        pair<int, int> nearestMushroom = { -1, -1 }; // nearestMushroom ���������������� ��������� (-1, -1), ��� ������ �����������, ����� ��������� ���� ��� �� ������.

        for (const auto& mushroom : mushrooms) {
            double distance = sqrt(pow(mushroom.x - robotX, 2) + pow(mushroom.y - robotY, 2));  // ���������� ���������� �� ������� ����������� ����������
            if (distance < minDistance) {
                minDistance = distance;
                nearestMushroom = { mushroom.x, mushroom.y };
            }
        }
        return nearestMushroom;
    }

    // ������� ���� �� ������ ������ � �� �����.
    void removeMushroom(int x, int y) {
        mushrooms.erase(remove_if(mushrooms.begin(), mushrooms.end(),
            [x, y](const Mushroom& mushroom) {
                return mushroom.x == x && mushroom.y == y;
            }), mushrooms.end());
    }
};

class Robot {
private:
    // ������� ���������� ������.
    int x;
    int y;

    // ���������� ��������� � ��������������� ������ � �������.
    int bagEdible;
    int bagHallucinogenic;

    // ����� ������ ������.
    int batteryTime;

    StateRobot state; // ������� ��������� ������.
    Forest& forest; // ������ �� ������ ������ Forest.

    // ��������� �� ������� ��������� ������.
    void checkErrors() {
        if (rand() % 1000 == 1) {
            cout << "�������������� ������" << endl;
            turnOff();
        }
    }

    // ������� ����������� ���� ����� � ��������� ��� � ��������������� �����.
    void pickMushroom(const string& type) {
        cout << "��� " << type << " ����" << endl;
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
    }

    // ������� �������� ������
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

    // ������� ����������� ��� ��������� � ������ ���� ���� �������� ����� � ����� ����� � �������
    void move() {
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
    }

    // ������� ����������� ����� (��������� ����������)
    void goHome() {
        while (x > 0 || y > 0) {
            forest.grid[x][y] = emptySymbol;
            if (x > 0) --x;
            if (y > 0) --y;
            move();
        }
    }

    // ������� ���������� ������ (����� �� ���������)
    void turnOff() {
        cout << "����� ��������" << endl;
        exit(0);
    }

public:
    Robot(Forest& f) : x(0), y(0), bagEdible(0), bagHallucinogenic(0), batteryTime(0), state(TURN_ON), forest(f) {
        forest.grid[x][y] = botSymbol;
    }

    // �������� ������� ������ ������
    void runStep() {
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
    bool isBatteryDepleted() const {
        return batteryTime >= batteryLimit;
    }

    // ������� ���������� ������� ������ �������
    void updateBatteryTime(int seconds) {
        batteryTime += seconds;
    }
};

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
        this_thread::sleep_for(chrono::seconds(sleepDuration));
        robot.updateBatteryTime(sleepDuration);
    }

    return 0;
}
