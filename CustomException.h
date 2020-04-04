#pragma once
#include <exception>
#include <string>
#include "WindowsEdited.h"

namespace DX
{
	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		virtual const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X",
				static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}

}
class CustomException : public std::exception
{
public:
	CustomException(int in_line, const char* file);
	const char* what() const override;
	unsigned int GetLine() const;
	virtual std::string GetFile() const; 
	virtual const char* GetType() const;
	std::string GetOriginString() const; 

protected:
	mutable std::string whatBuffer;

private:
	std::string ex_file;
	unsigned int ex_line;
};

