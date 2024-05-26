#include "Block.h"

USING_NS_CC;

Block* Block::createBlock(BlockType type, GridPosition gridPosition)
{
	Block* block = new Block();

	if (block && block->init())
	{
		block->autorelease();
		block->type = type;
		block->gridPosition = gridPosition;

		return block;
	}
	else
		CC_SAFE_DELETE(block);

	return nullptr;
}

void Block::onEnter()
{
	Node::onEnter();

	setContentSize(_BlockSize);
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	blockSprite = Sprite::createWithSpriteFrameName(BlockTypeToFrameName[static_cast<int>(type)]);
	blockSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	blockSprite->setPosition(_BlockSize * 0.5f);
	addChild(blockSprite);
}

void Block::setType(BlockType type)
{
	this->type = type;

	if (blockSprite != nullptr)
		blockSprite->setSpriteFrame(BlockTypeToFrameName[static_cast<int>(type)]);
}
