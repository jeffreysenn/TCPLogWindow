#pragma once
class Application
{
public:
	enum class Type
	{
		Server,
		Client,
	};

public:
	Application(Type type);

	void run();

private:
	Type mType;
};

