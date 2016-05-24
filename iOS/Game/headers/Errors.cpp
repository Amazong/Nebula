#include <iostream>
#include "Errors.h"

int error::file_access()
{
	std::cerr << "Erro de acesso de ficheiro!";
	std::cin.get();
	return 42;
}
