#pragma once
 
#ifndef Forest_h
#define Forest_h

class Robot;



class Forest {
public:
	Forest(int width, int length) {
		grid.resize(width, vector<char>(length, emptySymbol));
	}
	void placeMushrooms(char mushroomType, int count);
	void display(int robotX, int robotY, int bagEdible, int bagHallucinogenic);
	pair<int, int> getNearestMushroom(int robotX, int robotY);
	void removeMushroom(int x, int y);


};

#endif //Forest
