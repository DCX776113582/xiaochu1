#ifndef JEWELSJewelsGrid_H
#define JEWELSJewelsGrid_H

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

#define GRID_WIDTH 40
#define MOVE_SPEED 0.2
#define FIRST_JEWEL_ID 1
#define LAST_JEWEL_ID 7 

class Jewel;


class JewelsGrid : public Node
{
public:
	static JewelsGrid* create(int row, int col);
	bool init(int row, int col);

	void updateMap();
	bool isDeadMap();

private:
	Jewel* createAJewel(int x, int y);
	bool isJewelLegal(Jewel* jewel, int x, int y);
	void setJewelPixPos(Jewel* jewel, float x, float y);

	void swapJewels(Jewel *jewelA, Jewel *jewelB);
	void swapJewelToNewPos(Jewel* jewel);

	void refreshJewelsGrid();
	void refreshJewelsToNewPos(int col);

private:
	bool onTouchBegan(Touch*, Event*);
	void onTouchMoved(Touch*, Event*);

private:
	bool canCrush();
	void goCrush();

	void onJewelsSwaping(float dt);
	void onJewelsSwapingBack(float dt);
	void onJewelsCrushing(float dt);
	void onJewelsRefreshing(float dt);

public:
	int getRow() { return m_row; }
	int getCol() { return m_col; }

private:
	int m_row;
	int m_col;

	Jewel* m_jewelSelected;
	Jewel* m_jewelSwapped;

	vector<vector<Jewel*>> m_JewelsBox;
	Vector<Jewel*> m_crushJewelBox;
	Vector<Jewel*> m_newJewelBox;
};

#endif

