#include "DataBuffer.h"

DataBuffer::DataBuffer(size_t size)
	: size(size)
	, data(new char[size]())
{
}

void DataBuffer::clear()
{
	memset(data.get(), '\0', size);
}
