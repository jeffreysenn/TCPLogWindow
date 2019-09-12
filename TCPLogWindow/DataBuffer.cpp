#include "DataBuffer.h"

DataBuffer::DataBuffer(size_t size)
	: size(size)
	, data(new char[size]())
{
}
