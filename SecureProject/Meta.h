#pragma once
#ifndef META_HH
#define META_HH

// Inkluudit
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <vector>
#include <string>


// Luokka
class Meta
{
public:
	Meta(unsigned char* message, unsigned char* password);
	Meta();
	~Meta();
	void check(char* password);
	unsigned char* get_hashed_password();
	unsigned char* get_ciphertext();

protected:
	// Tietotyypit
	struct AES_Cipher {
		std::string name;
		const EVP_CIPHER* (*fun)() = EVP_aes_256_ecb;
	};
	const std::vector<AES_Cipher> AES_CIPHERS = {
		{"256_ecb",EVP_aes_256_ecb},
		{"192_ecb",EVP_aes_192_ecb},
		{"128_ecb",EVP_aes_128_ecb},
	};

	// Vakiot
	static const int MESSAGE_SIZE = 128;
	static const int KEY_SIZE = 32;
	static const int ALGORITHM = 0;
	static const int BLOCK_SIZE = 16;

	// Funktiot
	int encrypt();
	int decrypt(unsigned char* plaintext);
	void setup(unsigned char* password);

	// Muuttujat
	unsigned char ciphertext_[MESSAGE_SIZE];
	unsigned char* message_;
	unsigned char key_[KEY_SIZE];
	unsigned char hashed_password_[SHA256_DIGEST_LENGTH];
};


#endif // META_HH