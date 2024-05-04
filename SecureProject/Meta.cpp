// Inkluudit
#include <openssl/err.h>
#include <iostream>
#include "Meta.h"


// RAKENTAJA JA PURKAJA //

// Rakentaja
Meta::Meta(unsigned char* message, unsigned char* password) {
	message_ = message;
	SHA256(password, strlen((char*)password), hashed_password_);
	setup(password);
}

Meta::Meta() {
	message_ = (unsigned char*)"aabbccdd";
	SHA256((unsigned char*)"1234", strlen((char*)"1234"), hashed_password_);
}

// Purkaja
Meta::~Meta() {
}



// Alustetaan tilanne
void Meta::setup(unsigned char* password) {
	// Muodostetaan avain
	PKCS5_PBKDF2_HMAC_SHA1((char*)password, (int)strlen((char*)password), NULL, 0, 1000, KEY_SIZE, key_);

	// Sitten salataan tuo viesti
	encrypt();
}



// SALAUS JA SALAUKSEN PURKU //

// Salausohjelma
int Meta::encrypt() {
	// Muuttujat
	EVP_CIPHER_CTX* ctx;
	int len;
	int ciphertext_len;
	int plaintext_len = strlen((char*)message_);

	// Konteksti
	if (!(ctx = EVP_CIPHER_CTX_new())) {
		std::cerr << "Error: Failed to create context" << std::endl;
		return -1;
	}

	// Alustetaan salausoperaatio
	if (EVP_EncryptInit_ex(ctx, AES_CIPHERS.at(ALGORITHM).fun(), nullptr, key_, nullptr) != 1) {
		std::cerr << "Error: Failed to initialize encryption" << std::endl;
		return -1;
	}

	// Tehdään salaus
	if (EVP_EncryptUpdate(ctx, ciphertext_, &len, message_, plaintext_len) != 1) {
		std::cerr << "Error: Failed to encrypt text" << std::endl;
		return -1;
	}
	ciphertext_len = len;

	// Viimeistellään salaus
	if (EVP_EncryptFinal_ex(ctx, ciphertext_ + len, &len) != 1) {
		std::cerr << "Error: Failed to finalize encryption" << std::endl;
		return -1;
	}
	ciphertext_len += len;

	// Siivotaan ja palataan
	EVP_CIPHER_CTX_free(ctx);
	return ciphertext_len;
}


// Puretaan salaus
int Meta::decrypt(unsigned char* plaintext) {
	// Muuttujat
	EVP_CIPHER_CTX* ctx;
	int len;
	int plaintext_len;

	// Konteksti
	if (!(ctx = EVP_CIPHER_CTX_new())) {
		std::cerr << "Error: Failed to create context" << std::endl;
		return -1;
	}

	// Alustetaan purkuoperaatio
	if (EVP_DecryptInit_ex(ctx, AES_CIPHERS.at(ALGORITHM).fun(), nullptr, key_, nullptr) != 1) {
		std::cerr << "Error: Failed to initialize decryption" << std::endl;
		return -1;
	}

	// Puretaan salaus
	if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext_, 
		BLOCK_SIZE * ceil((double)strlen((char*)message_) / BLOCK_SIZE)) != 1) {
		std::cerr << "Error: Failed to decrypt text" << std::endl;
		return -1;
	}
	plaintext_len = len;

	// Viimeistellään salaus
	if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) {
		long int erro = ERR_get_error();
		std::cout << ERR_reason_error_string(erro) << std::endl;
		return -1;
	}
	plaintext_len += len;

	// Siivotaan ja palautetaan
	EVP_CIPHER_CTX_free(ctx);
	return plaintext_len;
}



// JULKISET //

// Tarkistetaan menikö oikein
void Meta::check(char* password) {
	// Muuttujia
	int decryptedtext_len = 0;
	unsigned char decryptedtext[MESSAGE_SIZE];

	// Ensiksi haetaan avain
	PKCS5_PBKDF2_HMAC_SHA1(password, (int)strlen((char*)password), NULL, 0, 1000, KEY_SIZE, key_);

	// Ajetaan decryptauksen läpi
	decryptedtext_len = decrypt(decryptedtext);

	// Miltä näyttää
	if (decryptedtext_len > 0) {
		std::cout << std::endl << "Decypted text is: " << std::endl;
		BIO_dump_fp(stdout, (const char*)decryptedtext, decryptedtext_len);
	}
}


// GETTERS //
unsigned char* Meta::get_ciphertext() { return ciphertext_; };
unsigned char* Meta::get_hashed_password() { return hashed_password_; };