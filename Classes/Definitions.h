#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <cocos2d.h>

const cocos2d::Size _BlockSize = cocos2d::Size(60, 60);

constexpr int MAX_ROWS = { 16 };
constexpr int MAX_COLS = { 10 };
constexpr int MATCHES_REQUIRED = { 3 };
const std::string FONT = "Verdana";

enum class BlockType : int
{
	RED = 0,
	BLUE,
	GREEN,
	YELLOW,
	PURPLE
};

const char* const BlockTypeToFrameName[]
{
	"red.png",		// RED
	"blue.png",		// BLUE
	"green.png",	// GREEN
	"yellow.png",	// YELLOW
	"purple.png"	// PURPLE
};

struct GridPosition {
	int row;
	int col;
};

#endif // __DEFINITIONS_H__