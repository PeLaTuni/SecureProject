#pragma once
// Inkluudit
#include "Meta.h"

class Meta256 : public Meta
{
public: 
	Meta256(unsigned char* message, unsigned char* password);
	~Meta256();

private:
	static const int KEY_SIZE = 32;
};

