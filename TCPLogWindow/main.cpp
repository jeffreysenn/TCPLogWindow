#include "networking.h"

#include "Application.h"

#include <string.h>
#include <stdio.h>
#include <windows.h>

static int usage()
{
	puts(".exe [server|client]");
	return 0;
}

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		return usage();
	}

	if (strcmp(argv[1], "server") == 0)
	{
		Application app(Application::Type::Server);
		app.run();
	}
	else if (strcmp(argv[1], "client") == 0)
	{
		Application	app(Application::Type::Client);
		app.run();
	}
	else
	{
		puts("error: invalid argument. ");
	}
}