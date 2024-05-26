#ifndef __GRID_H__
#define __GRID_H__

#include <cocos2d.h>
#include "Definitions.h"
#include "Block.h"

class Grid: public cocos2d::Layer
{
public:
	Block* blocks[MAX_ROWS][MAX_COLS];

	static Grid* createGrid(int rows, int cols);
	void removeAllBlocks();
	void generateRandomBlocks();
	void onEnter();
	Block* getBlockForPosition(cocos2d::Vec2 position);
	int findMatches(Block* block, std::vector<Block*>& matches, bool isRootRecursion = true);
	void removeBlockAt(GridPosition pos);
	void fallingBlocks();
	bool findAvalailableMoves();


	bool isBusy() const { return busy; }

private:
	bool busy;

	Grid() = default;
	~Grid() = default;
	Grid(const Grid&) = default;

};

#endif // __GRID_H__