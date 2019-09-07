#include "Application.h"
#include "networking.h"

Application::Application(Type type)
	: mType(type)
{
	network::init();
}

void Application::run()
{
	switch (mType)
	{
	case Application::Type::Server:

		break;
	case Application::Type::Client:
		break;
	default:
		break;
	}
}
