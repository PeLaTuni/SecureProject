/*
  Cli
  Creator: Peetu Lahtinen
*/


#include "Cli.h"


// Constructor and destructor
Cli::Cli() { 
	dec_ = new Decrypter();
};
Cli::~Cli() { 
	delete dec_;
	dec_ = nullptr;
};



// PUBLIC //
// Runs the program
bool Cli::run() {
	// Constants and variables
	std::string command = "";
	std::vector<std::string> inputs;
	Cmd* cmd = nullptr;

	// First the command
	std::cout << "> ";
	std::getline(std::cin, command);
	if (command.size() < 1) {
		std::cout << "Insert h for help" << std::endl << std::endl;
		return true;
	}

	// Was quit called
	if (command == "q") {
		return false;
	}

	// Was help called
	if (command == "h") {
		print_help();
		return true;
	}

	// Check the command
	inputs = split_string(command, ' ');
	cmd = check_command(inputs);
	if (not cmd) {
		std::cout << "No such command" << std::endl << std::endl;
		return true;
	}

	// Call the function
	inputs.erase(inputs.begin());
	if (inputs.size() == cmd->params.size()) {
		(dec_->*(cmd->func_ptr))(inputs);
		return true;
	} else {
		std::cout << "Wrong amount of input parameters" << std::endl << std::endl;
		return true;
	}
}



// PRIVATE //
// Checks the command
Cmd* Cli::check_command(std::vector<std::string>& inputs) {
	// Goes through available commands
	for (auto command = cmds_.begin(); command != cmds_.end(); command++) {
		if (command->command == inputs.at(0)) {
			return &(*command);
		}
	}
	return nullptr;
}


// Prints explanations
void Cli::print_help() {
	// Goes through available commands
	for (auto &command : cmds_) {
		std::cout << command.command << ": " << command.command_exp << std::endl;
	}
	std::cout << std::endl;
}


// Splits the string
std::vector<std::string> Cli::split_string(std::string& str, char split) {
	// Constants and variables
	std::vector<std::string> splitted;
	std::string substring = "";

	// Goes through the string
	for (char cha : str) {
		if (cha != split) {
			substring += cha;
		} else {
			splitted.push_back(substring);
			substring = "";
		}
	}

	// Add the last string
	splitted.push_back(substring);
	return splitted;
}