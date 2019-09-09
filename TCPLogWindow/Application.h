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
	~Application();

	void run();

private:
	Type mType;
};

