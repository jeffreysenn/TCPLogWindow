#include "Application.h"
#include "Server.h"
#include "Client.h"

Application::Application(Type type)
	: mType(type)
{
	network::init();
}

Application::~Application()
{
	network::shut();
}

void Application::run()
{
	switch (mType)
	{
	case Application::Type::Server:
	{
		Server server;
		server.run();
		break;
	}
	case Application::Type::Client:
	{
		Client client;
		client.run();
		break;
	}
	}
}
