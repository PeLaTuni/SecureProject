/* Secure Project
- Testing password strength and solving hashed passwords
- https://github.com/danielmiessler/SecLists/tree/master/Passwords
*/



// Includes
#include "Cli.h"
#include "Meta.h"



// Main
int main() {
	// Muutokset (poista)
	Meta meta;
	//meta.create_rainbowtable();
	//meta.create_hashfile();
	meta.create_big_hashfile();

	Cli cli;

	// Loop
	while ( cli.run() ) {};

	// End
	return 0;
}


/*******************************************************************/
/* eof */
