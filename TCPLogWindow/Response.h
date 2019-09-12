#pragma once
#include <string>
struct Response
{
	enum Status
	{
		Success,
		BadRequest,
		InternalServerError,
		Count,
	};

	uint8_t statusToCode(Status status);
	std::string statusToString(Status status);

	Response(Status status, float timestamp);

	std::string formResponse();


	Status status;
	float timestamp;
};

