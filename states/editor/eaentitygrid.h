#pragma once

class EAEntityGrid	// like tiles, stuff matched to the grid
{
	int max;
	int width;
	char** array;

public:
	EAEntityGrid();
	~EAEntityGrid();
private:
	void free();
public:
	void reset();	// empty the array

	void init(const int& width);	// allocate
	bool isCellEmpty(const int &mouseX, const int &mouseY);
	char get(const int &x, const int &y);
	bool add(int &mouseX, int &mouseY, const int &chosen);
	int remove(int &mouseX, int &mouseY);
	const int& getMax();
};