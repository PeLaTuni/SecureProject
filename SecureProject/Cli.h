/*
  Cli
  Creator: Peetu Lahtinen

  Function:
  - Command line interface
  - Let's the user operate the Decrypter-class via cli
  - Also comes with help and quit -methods
*/



#pragma once
#ifndef CLI_HH
#define CLI_HH


// Includes
#include <string>
#include <vector>
#include "decrypter.h"


// Using condenced names for function pointers for legibility
using FuncType = void (Decrypter::*)(const std::vector<std::string>& params);


// Command structure
struct Cmd {
	std::string command;
	std::string command_exp;
	std::vector<std::string> params;
	FuncType func_ptr;
};


class Cli
{
public:
	Cli();
	~Cli();

	// Runs the program
	// If the user inserts q, returns false. Otherwise true
	bool run();

private:
	// Checks user input
	// Returns the cmd -struct corresponding to the command
	Cmd* check_command(std::vector<std::string>& command);

	// Prints all commands and their explanations
	void print_help();

	// Takes a string as input
	// Splits the string on "split"s
	// Returns the split string as a vector
	std::vector<std::string> split_string(std::string& str, char split);

	// Which commands do we have
	std::vector<Cmd> cmds_ = {
		{ "h",   "Help",                                              {},           nullptr},
		{ "q",   "Quit",                                              {},           nullptr},
		{ "po",  "Prints saved options",                              {},           &Decrypter::print_options},
		{ "tp",  "Tests the system with user inputed password",       {"password"}, &Decrypter::test_password},
		{ "tt",  "Toggle tests",                                      {},           &Decrypter::set_tests},
		{ "shf", "Attempts to solve all hashed passwords in a file",  {"filename"}, &Decrypter::solve_hash_file},
		{ "swl", "Sets a wordlist",                                   {"filename"}, &Decrypter::set_wordlist},
		{ "srt", "Sets a rainbow table",                              {"filename"}, &Decrypter::set_rainbow_table},
		{ "sbd", "Sets how many characters the brute force attempts", {"depth"},    &Decrypter::set_brute_depth},
		{ "siq", "Sets an initial quess (space for reset)",           {"quess"},    &Decrypter::set_brute_quess},
		{ "ss",  "Sets the salt (space for reset)",					  {"salt"},     &Decrypter::set_salt},
	};

	// Internal variables
	Decrypter* dec_;
};


#endif // CLI_HH