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
	//std::vector<std::string> param = { "3A5E8D2B71F956C4B90A2D38FEB4C7E9" };
	//meta.set_salt(param);
	//meta.create_rainbowtable();
	//meta.create_hashfile();
	//meta.create_big_hashfile();

	Cli cli;

	// Loop
	while ( cli.run() ) {};

	// End
	return 0;
}


/*******************************************************************/
/* eof */
