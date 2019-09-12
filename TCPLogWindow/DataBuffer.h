#pragma once
#include "networking.h"
#include <memory>

struct DataBuffer
{
public:
	DataBuffer(size_t size);

	char* getData() { return data.get(); }
	const char* getData() const { return data.get(); }
	size_t getSize() const { return size; }

private:
	std::unique_ptr<char> data;
	size_t size;
};