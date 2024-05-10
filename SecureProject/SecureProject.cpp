/* Secure Project
- Made by Peetu Lahtinen
- Testing password strength and solving hashed passwords
- I got the wordlist from here: https://github.com/danielmiessler/SecLists/tree/master/Passwords
*/



// Includes
#include "Cli.h"



// Main
int main() {
	Cli cli;

	// Loop
	while ( cli.run() ) {};

	// End
	return 0;
}


/*******************************************************************/
/* eof */
