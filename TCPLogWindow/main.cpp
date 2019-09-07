#include "networking.h"

#include "Application.h"

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <windows.h>

static int usage()
{
	puts(".exe [server|client]");
	return 0;
}


static int run_server(const ip_address& address)
{
	tcp_listener listener;
	if (!listener.open(address))
	{
		auto errcode = network::error::get_error();
		auto errmsg = network::error::as_string(errcode);
		printf("could not open listener - %s (%d)", errmsg, errcode);
		return 0;
	}

	while (true)
	{
		auto result = listener.accept();
		if (result.is_success())
		{
			result.socket_;
			result.address_;
		}
		else if (!network::error::is_non_critical(result.code_))
		{
			// ...
		}
	}
	return 0;
}

static int run_client(ip_address& address)
{
	udp_socket socket;
	if (!socket.open(address))
		return 0;

	for (int i = 0; i < 255; ++i)
	{
		ip_address revieverAddr(192, 168, 1, i, 15000);

		const std::string text = "Meow";
		socket.send_to(revieverAddr, text.length(), (uint8*)text.c_str());
		Sleep(1);
	}

	socket.close();

	return 0;
}

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		return usage();
	}

	network::init();
	ip_address address(0, 0, 0, 0, 15000);

	int result = 0;
	if (strcmp(argv[1], "server") == 0)
	{
		result = run_server(address);
	}
	else if (strcmp(argv[1], "client") == 0)
	{
		result = run_client(address);
	}
	else
	{
		puts("error: invalid argument. ");
	}

	network::shut();
}