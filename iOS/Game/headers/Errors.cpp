#include <iostream>
#include "Errors.h"

// suggested error naming scheme:
// error xy, where x is category, and y is error number
// analogous to errors xyy in HTML (for example, 404, where x = 4 and yy == 04). See https://www.w3.org/Protocols/HTTP/HTRESP.html

// more errors to be added as needed

// usage in program: to throw an error, write
// complain(ErrNo::<errortype>);
// this makes the window invisible
// the error will be caught on next game_loop, and execution halted

int error::trace_error(int code) {
	switch (code)
	{
	case 10:
		error::profile_error();
	case 42:
		error::file_access();
	default:
		break;
	}
	std::cin.get(); // get user's attention
	return code;
}

void error::profile_error()
{
	std::cerr << "Erro de perfil de utilizador!";
}

void error::file_access()
{
	std::cerr << "Erro de acesso de ficheiro!";
}