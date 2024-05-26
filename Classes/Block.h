#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <cocos2d.h>
#include "Definitions.h"

class Block : public cocos2d::Node
{

public:
	GridPosition gridPosition;

	static Block* createBlock(BlockType type, GridPosition gridPosition);
	void setType(BlockType type);
	BlockType getType() const noexcept
	{
		return type;
	}
	virtual void onEnter();

private:
	BlockType type;
	cocos2d::Sprite* blockSprite = nullptr;

	Block() = default;
	~Block() = default;
	Block(const Block&) = default;
};

#endif // __BLOCK_H__