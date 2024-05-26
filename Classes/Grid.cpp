#include "Grid.h"

USING_NS_CC;

Grid* Grid::createGrid(int rows, int cols)
{
	Grid* grid = new Grid();

	if (grid && grid->init())
	{
		grid->autorelease();

		grid->setContentSize(Size(rows * _BlockSize.width, cols * _BlockSize.height));
		grid->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		return grid;
	}
	else
		CC_SAFE_DELETE(grid);

	return nullptr;
}

void Grid::onEnter()
{
	Layer::onEnter();
}

void Grid::generateRandomBlocks()
{
	for (int col = 0; col < MAX_COLS; col++)
	{
		for (int row = 0; row < MAX_ROWS; row++)
		{
			Block* block = Block::createBlock((BlockType)random((int)BlockType::RED, (int)BlockType::GREEN), { row, col });

			block->setType((BlockType)random((int)BlockType::RED, (int)BlockType::GREEN));
			block->setPosition(row * _BlockSize.width + _BlockSize.width * 0.5f, 
				col * _BlockSize.height + _BlockSize.height * 0.5f);

			addChild(block);
			blocks[row][col] = block;
		}
	}
}

void Grid::removeAllBlocks()
{
	stopAllActions();
	busy = false;

	for (int col = 0; col < MAX_COLS; col++)
	{
		for (int row = 0; row < MAX_ROWS; row++)
		{
			if (blocks[row][col] != nullptr)
			{
				removeChild(blocks[row][col]);
				blocks[row][col] = nullptr;
			}
		}
	}
}

Block* Grid::getBlockForPosition(cocos2d::Vec2 position)
{
	unsigned int row = (unsigned int) ceil((position.x - getPosition().x) / _BlockSize.width) - 1;
	unsigned int col = (unsigned int) ceil((position.y - getPosition().y) / _BlockSize.height) - 1;

	CCASSERT((col < MAX_COLS && row < MAX_ROWS), "Incorrect row/col. Index out of range");

	if (blocks[row][col])
		return blocks[row][col];

	return nullptr;
}

int Grid::findMatches(Block* block, std::vector<Block*>& matches, bool isRootRecursion)
{
	if (isRootRecursion)
		matches.clear();

	if (!isRootRecursion)
		matches.push_back(block);

	GridPosition pos = block->gridPosition;

	Block* leftBlock = (pos.row > 0) ? blocks[pos.row - 1][pos.col] : nullptr;
	Block* rightBlock = (pos.row < MAX_ROWS - 1) ? blocks[pos.row + 1][pos.col] : nullptr;
	Block* topBlock = (pos.col < MAX_COLS - 1) ? blocks[pos.row][pos.col + 1] : nullptr;
	Block* bottomBlock = (pos.col > 0) ? blocks[pos.row][pos.col - 1] : nullptr;

	// Поиск одинаковых блоков с левой стороны
	if (leftBlock && leftBlock->getType() == block->getType())
		if (std::find(matches.begin(), matches.end(), leftBlock) == matches.end())
			findMatches(leftBlock, matches, false);

	// Поиск блоков с правой стороны
	if (rightBlock && rightBlock->getType() == block->getType())
		if (std::find(matches.begin(), matches.end(), rightBlock) == matches.end())
			findMatches(rightBlock, matches, false);

	// Сверху
	if (topBlock && topBlock->getType() == block->getType())
		if (std::find(matches.begin(), matches.end(), topBlock) == matches.end())
			findMatches(topBlock, matches, false);

	// Снизу
	if (bottomBlock && bottomBlock->getType() == block->getType())
		if (std::find(matches.begin(), matches.end(), bottomBlock) == matches.end())
			findMatches(bottomBlock, matches, false);

	return matches.size();
}

void Grid::removeBlockAt(GridPosition pos)
{
	Block* block = blocks[pos.row][pos.col];

	blocks[pos.row][pos.col] = nullptr;
	removeChild(block, true);
}

void Grid::fallingBlocks()
{
	busy = true;

	for (int row = 0; row < MAX_ROWS; row++)
	{
		// Пустые места в grid
		int blanks = 0;

		//Поиск пустых мест
		for (int col = 0; col < MAX_COLS; col++)
		{
			if (blocks[row][col] == nullptr)
				blanks++;
			// Перемещение блоков на пустые места
			else if (blanks > 0)
			{
				int new_col = col - blanks;
				// Получает расположение по координатам куда падать и анимирует падения блока
				blocks[row][col]->runAction(EaseBounceOut::create(
					MoveTo::create(0.7f,
						Vec2(blocks[row][col]->getPositionX(),
							new_col * _BlockSize.height + _BlockSize.height * 0.5f))
				));

				blocks[row][new_col] = blocks[row][col];
				blocks[row][col] = nullptr;
				blocks[row][new_col]->gridPosition = { row, new_col };
			}
		}
	}

	busy = false;
}

bool Grid::findAvalailableMoves()
{
	std::vector<Block*> matches;

	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLS; col++)
		{
			Block* block = blocks[row][col];
			if (block == nullptr)
				continue;

			if (findMatches(block, matches) >= MATCHES_REQUIRED)
				return true;
		}
	}

	return false;
}