#include "stdafx.h"
#include "POP3Proxy.h"

int main(int argc, char* argv[])
{
	printf("Test proxy started...\n");
	printf("Press Esc to exit\n");

	CPOP3Proxy proxy;

	if(argc < 2)
	{
		printf("Using default directory '.\\MailRoot'\n");
		printf("Add more directories to track specifying them on the command line\n");
		proxy.AddDirectory(".\\MailRoot\\");
	}
	else
	{
		printf("Using directories:\n");
		for(int i = 1; i < argc; i++)
		{
			printf("%s\n", argv[i]);
			proxy.AddDirectory(argv[i]);
		}
	}

	int iErr = 0;

	if((iErr = proxy.Start(110, "MyServer", 110)))
	{
		printf("Error running proxy on port 110, err = %d, exiting...\n", iErr);
		return 0;
	}


	while(getch() != 27);

	printf("Shutting down proxy...\n");

	proxy.Stop();

	printf("Proxy shut down\n");

	return 0;
}
