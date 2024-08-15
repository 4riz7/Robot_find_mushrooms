#include <iostream> // ����������� ���������� ��� �����/������
#include <string>   // ����������� ���������� ��� ������ �� ��������
#include <cstdlib>  // ����������� ���������� ��� ��������� ��������� �����
#include <vector>   // ����������� ���������� ��� ������ � ���������
#include <cmath>    // ����������� ���������� ��� �������������� ��������
#include <limits>   // ����������� ���������� ��� ������ � ��������� �����
#include "Forest.h"

using namespace std;

const int forestWidth = 20; // ������ ���� 
const int forestLength = 50; // ����� ����
const char botSymbol = '@'; // ������ ��� ����������� ������ �� ����� 
const char emptySymbol = '.'; // ������ ������ ����
const int maxBagCapacity = 10; // ������������ ���������� ������ � ������

// ��������� ��� ������������� �����
struct Mushroom {
    char type; // ��� ����� (���������, ���������������, �����������)
    int x;
    int y;
};


vector<vector<char>> grid;  // ��������� ������, �������������� ���
vector<Mushroom> mushrooms; //������, ���������� ���������� � ���� ������.


// ������� ��c�������� ������ � ��������� ������ � ����
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

// ���������� ������� ��������� ���� � ������ � �������.
void Forest::display(int robotX, int robotY, int bagEdible, int bagHallucinogenic) {
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
pair<int, int> Forest::getNearestMushroom(int robotX, int robotY) {
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
void Forest::removeMushroom(int x, int y) {
    mushrooms.erase(remove_if(mushrooms.begin(), mushrooms.end(),
        [x, y](const Mushroom& mushroom) {
            return mushroom.x == x && mushroom.y == y;
        }), mushrooms.end());
}
