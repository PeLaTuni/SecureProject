#include "Meta256.h"

// RAKENTAJA JA PURKAJA //

// Rakentaja
Meta256::Meta256(unsigned char* message, unsigned char* password) {
	message_ = message;
	SHA256(password, strlen((char*)password), hashed_password_);
	setup(password);
}

// Purkaja
Meta256::~Meta256() {
}