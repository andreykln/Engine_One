#include "CustomException.h"
#include <sstream>

CustomException::CustomException(int in_line, const char* in_file)
	: ex_line(in_line), ex_file(in_file)
{}

const char* CustomException::what() const
{
	std::stringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

unsigned int CustomException::GetLine() const
{
	return ex_line;
}

std::string CustomException::GetFile() const
{
	return ex_file;
}

const char* CustomException::GetType() const
{
	return "Custom Win32 Exception";
}

std::string CustomException::GetOriginString() const
{
	std::ostringstream oss;
	oss << "[File]" << GetFile() << std::endl
		<< ", [Line]" << GetLine();
	return oss.str();
}
