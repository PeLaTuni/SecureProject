/*
  Decrypter
  Creator: Peetu Lahtinen
*/


#include "Decrypter.h"

// Constructor and destructor
Decrypter::Decrypter() {};
Decrypter::~Decrypter() {};



// PUBLIC //
// Prints info
void Decrypter::print_options(const std::vector<std::string>& params) {
	// Variables used
	std::cout << "VARIABLES" << std::endl;
	std::cout << "Wordslist: " << wordslist_ << std::endl;
	std::cout << "Rainbow table: " << rainbow_table_ << std::endl;
	std::cout << "Brute depth: " << brute_depth_ << std::endl;
	std::cout << "Starting quess: " << start_quess_ << std::endl;
	std::cout << "Salt ";
	if (salt_.size() == 0) {
		std::cout << "not set" << std::endl << std::endl;
	} else {
		std::cout << "set" << std::endl << std::endl;
	}
	
	// Tests toggled
	std::cout << "TESTS" << std::endl;
	for (const auto& test : tests_) {
		std::cout << test.first;
		if (test.second.toggled) {
			std::cout << " enabled" << std::endl;
		} else {
			std::cout << " disabled" << std::endl;
		}
	}
	std::cout << std::endl;
}


// Tests a user-inserted password
void Decrypter::test_password(const std::vector<std::string>& params) {
	// Does not test zero-length passwords
	if (params.at(0).size() == 0) {
		std::cout << "Insert a password" << std::endl << std::endl;
		return;
	}

	// Constants and variables
	std::string salted_password = params.at(0);
	add_salt(salted_password);
	unsigned char hashed_password[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char*)salted_password.c_str(), salted_password.size(), hashed_password);

	// Goes through all tests
	run_tests(hashed_password, SHA256_DIGEST_LENGTH);
	std::cout << std::endl;
}



// Solves hashed passwords from a user-inserted file
void Decrypter::solve_hash_file(const std::vector<std::string>& params) {
	// The legnth of the filename must not be zero
	if (params.at(0).size() == 0) {
		std::cout << "Insert a hashfile" << std::endl << std::endl;
		return;
	}

	// Tests the hash file
	if (not test_hash_file(params.at(0))) { std::cout << std::endl; return; }

	// Open the hash file
	std::ifstream hashfile(params.at(0), std::ios::binary);

	// Reads the amount
	uint32_t amount = 0;
	hashfile.read(reinterpret_cast<char*>(&amount), sizeof(amount));

	// Starts to read the file
	int i = 1;
	while (true) {
		// Reads the digest length
		uint8_t length = 0;
		if (not hashfile.read(reinterpret_cast<char*>(&length), sizeof(length))) { break; }

		// Opening message
		std::cout << "PASSWORD: " << i++ << std::endl;

		// Reads the hash
		unsigned char* hash = new unsigned char[length];
		hashfile.read(reinterpret_cast<char*>(hash), length);

		// Do we support this digest length
		bool support = false;
		for (auto &sha : sha_) {
			if (sha.first == length) { support = true; break; }
		}
		if (not support) {
			std::cout << "Hash digest length '" << static_cast<int>(length) << "' not supported" << std::endl << std::endl;
			continue;
		}

		// Perform tests on the hash
		run_tests(hash, length);
		delete[] hash;
		hash = nullptr;
		std::cout << std::endl;
	}

	// The file was read
	hashfile.close();
}



// SETTERS //
// Sets the wordlist
void Decrypter::set_wordlist(const std::vector<std::string>& params) {
	// Checks user input
	if (params.at(0).size() == 0) {
		std::cout << "Insert wordlist" << std::endl << std::endl;
		return;
	}

	// Tries to open the file
	std::string wordlist = params.at(0);
	std::ifstream wordfile(wordlist);
	if (not wordfile) {
		std::cout << "Error: Wordlist '" << wordlist << "' could not be opened" << std::endl << std::endl;
	}
	else {
		wordslist_ = wordlist;
		std::cout << "Wordlist set as: " << wordlist << std::endl << std::endl;
		wordfile.close();
	}
}


// Sets rainbow table
void Decrypter::set_rainbow_table(const std::vector<std::string>& params) {
	// Checks user input
	if (params.at(0).size() == 0) {
		std::cout << "Insert rainbow table" << std::endl << std::endl;
		return;
	}

	// Tests the table
	if (not test_rainbow_table(params.at(0))) { std::cout << std::endl; return; }

	// Sets the table
	rainbow_table_ = params.at(0);
	std::cout << "Rainbow table set as: " << rainbow_table_ << std::endl << std::endl;
}


// Sets brute depth
void Decrypter::set_brute_depth(const std::vector<std::string>& params) {
	// Tries to convert user input into a natural number
	try {
		int length = std::stoi(params.at(0));
		if (length > 0) {
			brute_depth_ = length;
			std::cout << "Brute depth set as: " << length << std::endl << std::endl;
		}
		else {
			std::cout << "Must be a positive integer" << std::endl << std::endl;
		}
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Must be a positive integer" << e.what() << std::endl << std::endl;
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Must be a positive integer" << e.what() << std::endl << std::endl;
	}
}


// Sets brute quess
void Decrypter::set_brute_quess(const std::vector<std::string>& params) {
	if (params.at(0).size() == 0) {
		std::cout << "Initial quess reset" << std::endl << std::endl;
		start_quess_ = "";
		return;
	}
	start_quess_ = params.at(0);
	std::cout << "Initial quess set as: " << params.at(0) << std::endl << std::endl;
}


// What tests do we want to run
void Decrypter::set_tests(const std::vector<std::string>& params) {
	// Constants and variables
	std::string choice = "";

	// Instructions
	std::cout << "1 to enable, 0 to disable" << std::endl;

	// Goes through the tests
	std::cout << "Dictionary";
	if (wordslist_.size() < 1) {
		std::cout << " (no wordlist set)";
	}
	std::cout << ": ";
	std::getline(std::cin, choice);
	if (choice == "1") {
		tests_.at("Dictionary").toggled = true;
	}
	else if (choice == "0") {
		tests_.at("Dictionary").toggled = false;
	}

	std::cout << "Rainbow table";
	if (rainbow_table_.size() < 1) {
		std::cout << " (no rainbow table set)";
	}
	std::cout << ": ";
	std::getline(std::cin, choice);
	if (choice == "1") {
		tests_.at("Rainbow table").toggled = true;
	}
	else if (choice == "0") {
		tests_.at("Rainbow table").toggled = false;
	}

	std::cout << "Brute force";
	std::cout << ": ";
	std::getline(std::cin, choice);
	if (choice == "1") {
		tests_.at("Brute").toggled = true;
	}
	else if (choice == "0") {
		tests_.at("Brute").toggled = false;
	}

	std::cout << "1337";
	if (wordslist_.size() < 1) {
		std::cout << " (no starting quess set)";
	}
	std::cout << ": ";
	std::getline(std::cin, choice);
	if (choice == "1") {
		tests_.at("1337").toggled = true;
	}
	else if (choice == "0") {
		tests_.at("1337").toggled = false;
	}
	std::cout << std::endl;
}


// Sets the salt
void Decrypter::set_salt(const std::vector<std::string>& params) {
	// Did we get a salt
	if (params.at(0).size() == 0) {
		std::cout << "Salt reset" << std::endl << std::endl;
		salt_.clear();
		return;
	}

	// Test conversion to binary
	std::vector<uint8_t> bin;
	std::string hex = params.at(0);
	if (not hex_to_bin(hex, bin)) { return; }

	// It worked, let's set it
	salt_ = bin;
	std::cout << "Salt set as: " << params.at(0) << std::endl << std::endl;
}




// PRIVATE //
// Runs all toggled tests
void Decrypter::run_tests(unsigned char* hashed_password, int len) {
	// Goes through all tests
	for (const auto& test : tests_) {
		// Is the test toggled
		if (not test.second.toggled) { continue; };

		// Performs toggled tests. Stops if the password is found
		if ((this->*(test.second.ptr))(hashed_password, len)) {
			return;
		}
	}
}



// "Rainbow table" -test
bool Decrypter::rainbow_attack(unsigned char* hashed_password, int len) {
	// Prints out which test we are running
	std::cout << "Rainbow table: ";

	// If there is a salt, then a rainbow table is useless
	if (salt_.size() != 0) {
		std::cout << "Salt has been set" << std::endl;
		return false;
	}

	// The rainbow table must be set beforehand
	if (rainbow_table_.size() < 1) {
		std::cout << "No rainbowlist" << std::endl;
		return false;
	}

	// Tests the rainbow table, in case of modifications
	if (not test_rainbow_table(rainbow_table_)) { std::cout << std::endl; return false; }

	// Tries to open the rainbow table
	std::ifstream input(rainbow_table_, std::ios::binary);

	// If the digest length of the password is different from the rainbow table,
	// then the test returns false
	uint8_t read_length = 0;
	input.read(reinterpret_cast<char*>(&read_length), sizeof(read_length));
	if (static_cast<int>(read_length) != len) {
		std::cout << "Password hash different from table (" << sha_.at(static_cast<int>(len)).name;
		std::cout << " not " << sha_.at(static_cast<int>(read_length)).name << ")" << std::endl;
		return false;
	}

	// Reads the amount of passwords (testing)
	uint32_t password_amount = 0;
	input.read(reinterpret_cast<char*>(&password_amount), sizeof(password_amount));

	// Goes through the rainbow table
	unsigned char* hash = new unsigned char[read_length];
	int i = 0;
	while (true) {
		// Stops when there is no more hash to read
		if (not input.read(reinterpret_cast<char*>(hash), read_length)) { break; }
		i++;

		// The length of the password
		uint8_t length = 0;
		input.read(reinterpret_cast<char*>(&length), sizeof(length));

		// The password
		std::string password = "";
		password.resize(length);
		input.read(&password[0], length);

		// Compare the hashes
		if (std::memcmp(hash, hashed_password, len) == 0) {
			input.close();

			// Test if the password was hashed correctly
			bool correct = true;
			unsigned char* hash_check = new unsigned char[read_length];
			(Sha().*(sha_.at(len).ptr))((unsigned char*)password.c_str(), strlen(password.c_str()), hash_check);
			if (std::memcmp(hash, hash_check, len) == 0) {
				std::cout << password << " (" << i << " tries)" << std::endl;
				correct = true;
			} else {
				std::cout << "Error: " << password << " hashed incorrectly" << std::endl;
				correct = false;
			}
			delete[] hash;
			delete[] hash_check;
			hash = nullptr;
			hash_check = nullptr;
			return correct;
		}
	}

	// The password was not found
	delete[] hash;
	hash = nullptr;
	input.close();
	std::cout << "No matches found" << " (" << i << " tries)" << std::endl;
	return false;
}



// Dictionary test
bool Decrypter::dictionary_attack(unsigned char* hashed_password, int len) {
	// Prints out which test we are running
	std::cout << "Dictionary attack: ";

	// The wordlist must be set beforehand
	if (wordslist_.size() < 1) {
		std::cout << "No wordlist" << std::endl;
		return false;
	}

	// Tries to open the wordlist
	std::ifstream passwordfile(wordslist_);
	if (not passwordfile) {
		std::cout << "Could not open file" << std::endl;
		return false;
	}
	
	// Hash passwords from the dictionary, until one matches or no passwords
	std::string row;
	int i = 0;
	while (getline(passwordfile, row)) {
		// First the salt
		std::string password = row;
		add_salt(password);

		// Using the correct sha-function, based on length of hashed password
		i++;
		(Sha().*(sha_.at(len).ptr))((unsigned char*)password.c_str(), strlen(password.c_str()), sha_.at(len).buffer);
		if (std::memcmp(sha_.at(len).buffer, hashed_password, len) == 0) {
			passwordfile.close();
			std::cout << row << " (" << i << " tries)" << std::endl;
			return true;
		};
	}

	// The password was not found
	passwordfile.close();
	std::cout << "No matches found" << " (" << i << " tries)" << std::endl;
	return false;
}



// "1337" -test
bool Decrypter::leet_attack(unsigned char* hashed_password, int len) {
	// Prints out which test we are running
	std::cout << "1337-test: ";

	// The brute quess must be set beforehand
	if (start_quess_.size() < 1) {
		std::cout << "No starting string set" << std::endl;
		return false;
	}

	// Calls recursive help-function
	std::string sacrificial = start_quess_;
	std::string password = "";
	if (leet_recursive(sacrificial, password, hashed_password, len)) {
		return true;
	} else {
		std::cout << "No matches found" << std::endl;
		return false;
	}
	
}


// Recursive help-funtion
bool Decrypter::leet_recursive(std::string& start, std::string& end, unsigned char* hashed_password, int len) {
	// If "start" is empty, then hashes "end" and compares it to the hashed password
	if (start.size() < 1) {
		std::string end_salted = end;
		add_salt(end_salted);
		(Sha().*(sha_.at(len).ptr))((unsigned char*)end_salted.c_str(), strlen(end_salted.c_str()), sha_.at(len).buffer);
		if (std::memcmp(sha_.at(len).buffer, hashed_password, len) == 0) {
			std::cout << end << std::endl;
			return true;
		};
		return false;
	}

	// Converts the character to its variants. If the character is not found, just uses the character
	char first = start.at(0);
	start.erase(0,1);
	if (leet_.find(tolower(first)) != leet_.end()) {
		for (char repla : leet_.at(tolower(first))) {
			end += repla;
			if (leet_recursive(start, end, hashed_password, len)) {
				return true;
			};
			end.pop_back();
		}
	} else {
		end += first;
		if (leet_recursive(start, end, hashed_password, len)) {
			return true;
		};
		end.pop_back();
	}
	start = first + start;
	return false;
}



// Brute test
bool Decrypter::brute_attack(unsigned char* hashed_password, int len) {
	// Prints out which test we are running
	std::cout << "Brute: ";

	// Is the starting quess alrealy correct
	std::string brute = start_quess_;
	if (brute.size() > 0) {
		(Sha().*(sha_.at(len).ptr))((unsigned char*)brute.c_str(), strlen(brute.c_str()), sha_.at(len).buffer);
		if (std::memcmp(sha_.at(len).buffer, hashed_password, len) == 0) {
			std::cout << brute << std::endl;
			return true;
		}
	}

	// Calls recursive help-function
	if (brute_recursive(brute, 0, hashed_password, len)) {
		return true;
	} else {
		std::cout << "No matches found" << std::endl;
		return false;
	}
}


// Recursive help-function
bool Decrypter::brute_recursive(std::string& brute, int depth, unsigned char* hashed_password, int len) {
	// If we have gone past the depth limit, returns to previous depth
	if (depth >= brute_depth_) {
		return false;
	}

	// If we are still within the depth limit
	for (char cha : chars_) {
		brute += cha;
		std::string brute_salted = brute;
		add_salt(brute_salted);
		(Sha().*(sha_.at(len).ptr))((unsigned char*)brute_salted.c_str(), strlen(brute_salted.c_str()), sha_.at(len).buffer);
		if (std::memcmp(sha_.at(len).buffer, hashed_password, len) == 0) {
			std::cout << brute << std::endl;
			return true;
		}
		if (brute_recursive(brute, depth + 1, hashed_password, len)) {
			return true;
		}
		brute.pop_back();
	}
	
	// Nothing was found
	return false;
}



// Tests if a rainbow table is faulty
bool Decrypter::test_rainbow_table(const std::string& filename) {
	// Tries to open the file
	std::ifstream rainbowfile(filename, std::ios::binary);
	if (not rainbowfile) {
		std::cout << "Error: Rainbow table '" << filename << "' could not be opened" << std::endl;
		return false;
	}

	// How long is the file
	rainbowfile.seekg(0, std::ios::end);
	std::streampos end = rainbowfile.tellg();
	rainbowfile.seekg(0, std::ios::beg);

	// Checks the digest length of the table
	bool supported = false;
	uint8_t read_length = 0;
	if (not rainbowfile.read(reinterpret_cast<char*>(&read_length), sizeof(read_length))) {
		rainbowfile.close();
		std::cout << "Error: Could not read digest length" << std::endl;
		return false;
	}
	for (auto &sha : sha_) {
		if (sha.first == read_length) { supported = true; break; }
	}
	if (not supported) {
		rainbowfile.close();
		std::cout << "Error: Digest length '" << static_cast<int>(read_length) << "' not supported" << std::endl;
		return false;
	}

	// How many passwords do we have
	uint32_t size = 0;
	if (not rainbowfile.read(reinterpret_cast<char*>(&size), sizeof(size))) {
		rainbowfile.close();
		std::cout << "Error: Could not read amount of hashes" << std::endl;
		return false;
	} else if (size == 0) {
		rainbowfile.close();
		std::cout << "Error: Amount of passwords cannot be zero" << std::endl;
		return false;
	}

	// Goes through the file and tests if it works
	unsigned char* hash = new unsigned char[read_length];
	int pos = 0;
	std::streampos loc = rainbowfile.tellg();
	while (true) {
		// Reads the hash
		if (not rainbowfile.read(reinterpret_cast<char*>(hash), read_length)) { break; }

		// The length of the password
		uint8_t length = 0;
		if (not rainbowfile.read(reinterpret_cast<char*>(&length), sizeof(length))) { break; }

		// The password
		std::string password = "";
		password.resize(length);
		if (not rainbowfile.read(&password[0], length)) { break; }

		// The end of the read operations
		pos++;
		loc = rainbowfile.tellg();
	}
	delete[] hash;
	hash = nullptr;
	rainbowfile.close();

	// Was the end of the file left unread
	if (not (size == pos and end == loc)) {
		std::cout << "Error: Formatting issue" << std::endl;
		return false;
	}

	// Everything was fine
	return true;
}


// Tests the hash file
bool Decrypter::test_hash_file(const std::string& filename) {
	// Tries to open the file
	std::ifstream hashfile(filename, std::ios::binary);
	if (not hashfile) {
		std::cout << "Error: Hash file '" << filename << "' could not be opened" << std::endl;
		return false;
	}

	// How long is the file
	hashfile.seekg(0, std::ios::end);
	std::streampos end = hashfile.tellg();
	hashfile.seekg(0, std::ios::beg);

	// The amount of hashes
	uint32_t size = 0;
	if (not hashfile.read(reinterpret_cast<char*>(&size), sizeof(size))) {
		hashfile.close();
		std::cout << "Error: Could not read amount of hashes" << std::endl;
		return false;
	} else if (size == 0) {
		hashfile.close();
		std::cout << "Error: Amount of hashes cannot be zero" << std::endl;
		return false;
	}

	// Goes through the file and tests if it works
	int pos = 0;
	std::streampos loc = hashfile.tellg();
	while (true) {
		// Reads the length of the hash
		uint8_t length = 0;
		if (not hashfile.read(reinterpret_cast<char*>(&length), sizeof(length))) { break; }

		// Did we read a zero
		if (length == 0) { break; }

		// Reads the hash
		unsigned char* hash = new unsigned char[length];
		if (not hashfile.read(reinterpret_cast<char*>(hash), length)) { delete[] hash; break; }

		// Succesfull read
		delete[] hash;
		pos++;
		loc = hashfile.tellg();
	}
	hashfile.close();

	// Was the end of the file left unread
	if (not (size == pos and end == loc)) {
		std::cout << "Error: Formatting issue" << std::endl;
		return false;
	}

	// Went well
	return true;
}


// Converts hexadecimal into binary
bool Decrypter::hex_to_bin(const std::string& hex, std::vector<uint8_t>& bin)
{
	// Constants and variables
	std::vector<uint8_t> placeholder_bin;

	// Is the string divisible by two
	if (hex.size() % 2 != 0) {
		std::cout << "Error: Hexadecimal must be divisible by two" << std::endl;
		return false;
	}

	// Conversion
	for (size_t pos = 0; pos < hex.size(); pos+=2) {
		// The conversion
		std::stringstream ss;
		uint8_t byte;
		ss << std::hex << hex.substr(pos, 2);
		ss >> byte;

		// Check if conversion was successful
		if (ss.fail()) {
			std::cout << "Error: Failed to convert hexadecimal string" << std::endl;
			return false;
		}

		// If was, add byte
		placeholder_bin.push_back(byte);
	}

	// Check if the conversion went through
	if (placeholder_bin.size() != hex.size() / 2) {
		std::cout << "Error: Failed to convert hexadecimal string" << std::endl;
		return false;
	}

	// Everything worked
	bin = placeholder_bin;
	return true;
}


// Add salt to string
void Decrypter::add_salt(std::string& password) {
	for (uint8_t byte : salt_) { password.push_back(byte); }
}