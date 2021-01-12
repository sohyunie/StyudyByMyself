#pragma once
#include "StaticObject.h"

class Block : public StaticObject
{
private:

public:
	Block() {
		this->type = ObjectType::BLOCK;
	}

};
