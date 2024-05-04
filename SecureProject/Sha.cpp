/*
  Sha
  Creator: Peetu Lahtinen
*/



#include "Sha.h"

Sha::Sha()
{
}

Sha::~Sha()
{
}

void Sha::Sha224(const unsigned char* password, size_t length, unsigned char* buffer)
{
	SHA224(password, length, buffer);
}

void Sha::Sha256(const unsigned char* password, size_t length, unsigned char* buffer)
{
	SHA256(password, length, buffer);
}

void Sha::Sha384(const unsigned char* password, size_t length, unsigned char* buffer)
{
	SHA384(password, length, buffer);
}

void Sha::Sha512(const unsigned char* password, size_t length, unsigned char* buffer)
{
	SHA512(password, length, buffer);
}
