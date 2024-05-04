/*
  Decrypter
  Creator: Peetu Lahtinen

  Function:
  - The Decrypter class is used to solve hashed passwords
  - It currently supports four different methods of solving a password
    - Dictionary attack, Rainbow tables, 1337-attack and Brute force
	  - See documentation for more detail on these methods
  - The methods can be toggled on and off
    - Some of these methods require prerequisite information
	  - See documentation for more details
	  - E.g. A dictionary attack requires a wordlist
  - Certain variables can be set
    - Dictionary, Rainbow table, brute depth and brute quess
	  - See documentation for more details
  - Can also be used to test if the software can solve a user
    inserted password
	- This feature is mainly for development testing
*/



#pragma once
#ifndef DECRYPTER_HH
#define DECRYPTER_HH


// Includes
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "Sha.h"


// Needs to be pre-defined for function pointers to work
class Decrypter;


// Using condenced names for function pointers for legibility
using sha_func_ptr = void (Sha::*)(const unsigned char* password, size_t length, unsigned char* buffer);
using dec_func_ptr = bool (Decrypter::*)(unsigned char* hashed_password, int len);


class Decrypter
{
public:
	// Constructor and destructor
	Decrypter();
	~Decrypter();

	// All of the public methods take a vector of parameters
	// This helps with the implementation of the Cli-class

	// Gets a user submitted password as a parameter
	// Hashes the password with SHA256 and runs all toggled tests
	// on the hash
	// Runs tests until the password is found or the tests run out
	// Prints out the results
	void test_password(const std::vector<std::string>& params);

	// Gets the filename of a file of hashed passwords (format in documentation)
	// Runs all toggled tests on the hashes
	// Runs tests on a password, until it is found or the tests run out
	// Prints out the results for each hashed password in the file
	void solve_hash_file(const std::vector<std::string>& params);

	// Prints out toggled tests and set internal variables
	void print_options(const std::vector<std::string>& params);

	// SETTERS //
	void set_wordlist(const std::vector<std::string>& params);
	void set_rainbow_table(const std::vector<std::string>& params);
	void set_brute_depth(const std::vector<std::string>& params);
	void set_brute_quess(const std::vector<std::string>& params);
	void set_tests(const std::vector<std::string>& params);


private:
	// Takes a hashed password and its length in bytes
	// Performs all toggled tests on the hash
	void run_tests(unsigned char* hashed_password, int len);

	// Takes a hashed password and its length in bytes
	// Performs a "Rainbow table" -test against the hash
	// If the password was found, then returns true. Otherwise returns false
	// Also returns false, if the digest length of the hashed password is
	// different from the rainbow table
	bool rainbow_attack(unsigned char* hashed_password, int len);

	// Takes a hashed password and its length in bytes
	// Performs a Dictionary attack against the hash
	// If the password was found, then returns true. Otherwise returns false
	bool dictionary_attack(unsigned char* hashed_password, int len);

	// Takes a hashed password and its length in bytes
	// Performs a "1337" -test against the hash (see documentation)
	// If the password was found, then returns true. Otherwise returns false
	bool leet_attack(unsigned char* hashed_password, int len);

	// A recursive help-function for the "1337" -test
	// Every loop, the first character is taken away from "start" and leet_recursive is called again
	// with all possible variations of this character being added to "end" (see leet_)
	// Runs until it finds the correct password or goes through all possible combinations of replacement
	// characters
	// Returns true if password is found, false if not
	bool leet_recursive(std::string& start, std::string& end, unsigned char* hashed_password, int len);

	// Takes a hashed password and its length in bytes
	// Performs a Brute test against the hash
	// If the password was found, then returns true. Otherwise returns false
	bool brute_attack(unsigned char* hashed_password, int len);

	// A recursive help-function for the Brute test
	// Every loop, adds a character from chars_ to the end of "brute", and then calls brute_recursive again
	// Runs until it finds the correct password or goes through all possible combinations of brute_depth_
	// -amount of characters
	// Returns true if password is found, false if not
	bool brute_recursive(std::string& brute, int depth, unsigned char* hashed_password, int len);

	// Tests if the rainbow table has any errors
	// If the file cannot be opened, the digest length is not supported, or the reading process contains
	// any errors, returns false. Otherwise true
	bool test_rainbow_table(const std::string& filename);

	// Tests if the hash file has any errors
	// If the file cannot be opened or the reading process contains any errors, returns false. Otherwise true
	bool test_hash_file(const std::string& filename);


	// MISC //
	// Internal variables
	std::string wordslist_ = "";
	std::string rainbow_table_ = "";
	int brute_depth_ = 3;
	std::string start_quess_ = "";
	std::string salt_ = "";

	// Characters used for the Brute test
	const std::string chars_ = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*_-+=?";

	// Buffers for the sha_ -map
	unsigned char hash224_[SHA224_DIGEST_LENGTH] = {};
	unsigned char hash256_[SHA256_DIGEST_LENGTH] = {};
	unsigned char hash384_[SHA384_DIGEST_LENGTH] = {};
	unsigned char hash512_[SHA512_DIGEST_LENGTH] = {};


	// STRUCTS //	
	// Name of the algorithm, predefined buffer and a pointer to
	// the algorithm in the Sha-class
	struct Sha_struct {
		std::string name = "";
		unsigned char* buffer = nullptr;
		sha_func_ptr ptr = nullptr;
	};

	// A pointer to a test-function and if it is toggled on
	struct Fcn_struct {
		dec_func_ptr ptr = nullptr;
		bool toggled = true;
	};


	// MAPS //
	// "1337" -table
	const std::unordered_map<char, std::vector<char>> leet_ = {
		{'a', {'a', 'A', '4', '@'}},
		{'b', {'b', 'B', '8'}},
		{'c', {'c', 'C'}},
		{'d', {'d', 'D'}},
		{'e', {'e', 'E', '3'}},
		{'f', {'f', 'F'}},
		{'g', {'g', 'G', '6', '9'}},
		{'h', {'h', 'H'}},
		{'i', {'i', 'I', '1'}},
		{'j', {'j', 'J'}},
		{'k', {'k', 'K'}},
		{'l', {'l', 'L', '1', '7', '2'}},
		{'m', {'m', 'M'}},
		{'n', {'n', 'N'}},
		{'o', {'o', 'O', '0'}},
		{'p', {'p', 'P', '9'}},
		{'q', {'q', 'Q', '2', '9', '&'}},
		{'r', {'r', 'R', '9', '7', '2', '3', '4'}},
		{'s', {'s', 'S', '5', '2'}},
		{'t', {'t', 'T', '7'}},
		{'u', {'u', 'U'}},
		{'v', {'v', 'V'}},
		{'w', {'w', 'W'}},
		{'x', {'x', 'X'}},
		{'y', {'y', 'Y'}},
		{'z', {'z', 'Z', '2'}},
	};

	// Map for the hashing algorithms used
	std::unordered_map<int, Sha_struct> sha_ = {
		{SHA224_DIGEST_LENGTH, {"SHA-224", hash224_, &Sha::Sha224}},
		{SHA256_DIGEST_LENGTH, {"SHA-256", hash256_, &Sha::Sha256}},
		{SHA384_DIGEST_LENGTH, {"SHA-384", hash384_, &Sha::Sha384}},
		{SHA512_DIGEST_LENGTH, {"SHA-512", hash512_, &Sha::Sha512}},
	};

	// Map for tests
	std::unordered_map<std::string, Fcn_struct> tests_ = {
		{"Rainbow table", {&Decrypter::rainbow_attack,    true}},
		{"Dictionary",    {&Decrypter::dictionary_attack, true}},
		{"1337",          {&Decrypter::leet_attack,       true}},
		{"Brute",         {&Decrypter::brute_attack,      true}},
	};
};


#endif // DECRYTPER_HH