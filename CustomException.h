#pragma once
#include <exception>

class CustomException : public std::exception
{
public:
	CustomException(int in_line, const char* file);
	const char* what() const override;
	unsigned int GetLine() const;
	virtual const char* GetFile() const; //std::string???
	virtual const char* GetType() const;
	const char* GetOriginString() const; // const char?

protected:
	mutable const char* whatBuffer = {};

private:
	const char* ex_file = {};
	unsigned int ex_line = {};
};

