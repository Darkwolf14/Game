#ifndef BLOCK_H
#define BLOCK_H

#include "Object.h"

namespace gEng {

	enum BlockType {GRASS};

	class Block : public Object
	{
	private:
		BlockType type;

	public:
		Block(sf::Vector2f position, BlockType type);

	};

}

#endif