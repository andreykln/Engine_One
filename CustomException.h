#pragma once
#include <exception>
#include <string>
#include "WindowsEdited.h"
class CustomException : public std::exception
{
public:
	CustomException(int in_line, const char* file);
	const char* what() const override;
	unsigned int GetLine() const;
	virtual std::string GetFile() const; //std::string???
	virtual const char* GetType() const;
	std::string GetOriginString() const; // const char?

protected:
	mutable std::string whatBuffer;

private:
	std::string ex_file;
	unsigned int ex_line;
};

