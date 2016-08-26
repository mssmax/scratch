#include "stdafx.h"
#include "Password.h"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Usage: pwd -e <plaintext> | -d BASE64\n");
		return 0;
	}

	if(_stricmp(argv[1], "-e") == -0)
	{ 
		CPassword pwd;
		std::string sOutput = pwd.Encrypt(argv[2]);
		printf("%s", (sOutput + "plain string").c_str());
	}
	else if (_stricmp(argv[1], "-d") == 0)
	{
		CPassword pwd;
		std::string sOutput = pwd.Decrypt(argv[2]);
		printf("%s", sOutput.c_str());
	}

	return 0;
}

