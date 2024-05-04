/* Program name
- Tällä ohjelmalla testataan salasanojen vahvuutta
- Oletuksena on (vielä tässä vaiheessa), että olemme onnistuneet
  pääsemään käsiksi salasanan hash-arvoon ja suolaan. Lisäksi
  tiedämme hash-algoritmin
- https://github.com/danielmiessler/SecLists/tree/master/Passwords
*/



// Inkluudit
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <ctime>
#include "Cli.h"


struct Rainbow {
	unsigned char hash[SHA256_DIGEST_LENGTH] = {};
	std::string password;
};



// Pääohjelma
int main() {
	Cli cli;

	// Looppi
	while ( cli.run() ) {};

	/*
	// Leikitään hash-tiedostoilla
	std::ofstream hashfile1("hash_file.bin", std::ios::binary);
	if (not hashfile1) {
		std::cout << "No work :(" << std::endl;
		return 0;
	}

	/*
	std::vector<std::string> passwords = { "matias" , "m@71as", "matias384", "matias???" };
	unsigned char hash256[SHA256_DIGEST_LENGTH];
	uint8_t length = SHA256_DIGEST_LENGTH;
	for (int i = 0; i <= 3; i++) {
		SHA256((unsigned char*)passwords.at(i).c_str(), passwords.at(i).size(), hash256);
		hashfile1.write(reinterpret_cast<const char*>(&length), sizeof(length));
		hashfile1.write(reinterpret_cast<const char*>(hash256), SHA256_DIGEST_LENGTH);
		std::cout << passwords.at(i) << std::endl;
	}*/

	/*
	// Muuttujia
	std::string password224 = "matias???";
	unsigned char hash224[SHA224_DIGEST_LENGTH];
	std::string password256 = "m@71as";
	unsigned char hash256[SHA256_DIGEST_LENGTH];
	std::string password384 = "matias384";
	unsigned char hash384[SHA384_DIGEST_LENGTH];
	std::string password512 = "matias";
	unsigned char hash512[SHA512_DIGEST_LENGTH];

	// Hashataan
	SHA224((unsigned char*)password224.c_str(), password224.size(), hash224);
	SHA256((unsigned char*)password256.c_str(), password256.size(), hash256);
	SHA384((unsigned char*)password384.c_str(), password384.size(), hash384);
	SHA512((unsigned char*)password512.c_str(), password512.size(), hash512);

	// Kirjoitetaan
	uint8_t length224 = SHA224_DIGEST_LENGTH;
	hashfile1.write(reinterpret_cast<const char*>(&length224), sizeof(length224));
	hashfile1.write(reinterpret_cast<const char*>(hash224), SHA224_DIGEST_LENGTH);
	uint8_t length256 = SHA256_DIGEST_LENGTH;
	hashfile1.write(reinterpret_cast<const char*>(&length256), sizeof(length256));
	hashfile1.write(reinterpret_cast<const char*>(hash256), SHA256_DIGEST_LENGTH);
	uint8_t length384 = SHA384_DIGEST_LENGTH;
	hashfile1.write(reinterpret_cast<const char*>(&length384), sizeof(length384));
	hashfile1.write(reinterpret_cast<const char*>(hash384), SHA384_DIGEST_LENGTH);
	uint8_t length512 = SHA512_DIGEST_LENGTH;
	hashfile1.write(reinterpret_cast<const char*>(&length512), sizeof(length512));
	hashfile1.write(reinterpret_cast<const char*>(hash512), SHA512_DIGEST_LENGTH);

	// Suljetaan
	hashfile1.close();

	/*
	// Yritetään lukea nuo numerot tuolta
	std::ifstream hashfile2("hash_file256.bin", std::ios::binary);
	if (not hashfile2) {
		std::cout << "No work :(" << std::endl;
		return 0;
	}

	// Lukemaan
	while (true) {
		uint8_t length = 0;
		hashfile2.read(reinterpret_cast<char*>(&length), sizeof(length));
		if (hashfile2.eof()) { break; };
		int len = static_cast<int>(length);
		std::cout << len << std::endl;
		unsigned char* hash = new unsigned char(len);
		hashfile2.read(reinterpret_cast<char*>(hash), len);
		delete hash;
	}

	// Suljetaan tiedosto
	hashfile2.close();*/


	/*
	// Testataan sateenkaaripöytiä
	// Ensiksi avataan mitä halutaan muuttaa
	std::ifstream input1("pl100k.txt");
	if (not input1) {
		std::cout << "Shit got fucked" << std::endl;
		return 0;
	}

	// Heitetään vektoriin
	std::vector<std::string> passwords;
	std::string row = "";
	while (std::getline(input1, row)) {
		passwords.push_back(row);
	}

	// Suljetaan
	input1.close();


	
	// Tehdään sateenkaaripöytä
	std::vector<Rainbow> rainbow_vec;
	for (std::string password : passwords) {
		Rainbow entry;
		SHA256((unsigned char*)password.c_str(), password.size(), entry.hash);
		entry.password = password;
		rainbow_vec.push_back(entry);
	}
	
	// Avataan tiedosto
	std::ofstream output("rb100k_SHA256.bin", std::ios::binary);
	if (not output) {
		std::cout << "No work :(" << std::endl;
		return 0;
	}

	// Kirjoitetaan
	uint8_t digest_length = SHA256_DIGEST_LENGTH;
	output.write(reinterpret_cast<const char*>(&digest_length), sizeof(digest_length));
	for (const auto& entry : rainbow_vec) {
		output.write(reinterpret_cast<const char*>(entry.hash), SHA256_DIGEST_LENGTH);
		uint8_t length = entry.password.size();
		output.write(reinterpret_cast<const char*>(&length), sizeof(length));
		output.write(entry.password.data(), length);
	}

	// Laitetaan kiinni
	std::cout << "Kirjoitettiin tiedosto" << std::endl;
	output.close();


	// Sitten yritetään lukea
	std::vector<Rainbow> rainbow_table;

	// Avataan tiedosto
	std::ifstream input("rb100k_SHA256.bin", std::ios::binary);
	if (not input) {
		std::cout << "No work again :(" << std::endl;
		return 0;
	}

	// Luetaan
	uint8_t read_length;
	input.read(reinterpret_cast<char*>(&read_length), sizeof(read_length));
	while (true) {
		// Ensiksi tallennus
		Rainbow entry;

		// Luetaan hash
		if (not input.read(reinterpret_cast<char*>(entry.hash), SHA256_DIGEST_LENGTH)) {
			break;
		}

		// Luetaan salasanan pituus
		uint8_t length;
		if (!input.read(reinterpret_cast<char*>(&length), sizeof(length))) {
			std::cerr << "Error reading password length." << std::endl;
			return 1;
		}

		// Luetaan salasana
		entry.password.resize(length);
		if (!input.read(&entry.password[0], length)) {
			std::cerr << "Error reading password." << std::endl;
			return 1;
		}

		// Heitetään perään
		rainbow_table.push_back(entry);
	}
	
	// Suljetaan ja tulostetaan
	input.close();
	int i = 0;
	for (const auto& entry : rainbow_table) {
		std::cout << "Hash: ";
		for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
			std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(entry.hash[i]);
		}
		std::cout << " Password: " << entry.password << std::endl;
		if (i++ > 100) { break; };
	}
	*/
	// Lopetetaan
	return 0;
}


/*******************************************************************/
/* eof */
