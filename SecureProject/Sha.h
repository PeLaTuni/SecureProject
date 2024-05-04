/*
  Sha
  Creator: Peetu Lahtinen

  Function:
  - Makes it easier to use SHA-functions with function pointers
*/



#pragma once
#ifndef SHA_HH
#define SHA_HH

#include <openssl/sha.h>

class Sha
{
public:
	// Constructor and destructor
	Sha();
	~Sha();

	// The SHA-functions we use
	void Sha224(const unsigned char* password, size_t length, unsigned char* buffer);
	void Sha256(const unsigned char* password, size_t length, unsigned char* buffer);
	void Sha384(const unsigned char* password, size_t length, unsigned char* buffer);
	void Sha512(const unsigned char* password, size_t length, unsigned char* buffer);
};


#endif