#include "StdAfx.h"
#include "CharTransform.h"
#include <vector>

CharTransform::CharTransform(void)
{
}


CharTransform::~CharTransform(void)
{
}


std::wstring CharTransform::Utf82Unicode(const std::string& utf8string)
{
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0); 
	if (widesize == ERROR_NO_UNICODE_TRANSLATION) 
	{ 
		throw std::exception("Invalid UTF-8 sequence."); 
	} 
	if (widesize == 0) 
	{ 
		throw std::exception("Error in conversion."); 
	} 
	std::vector<wchar_t> resultstring(widesize); 
	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize); 
	if (convresult != widesize) 
	{ 
		throw std::exception("La falla!"); 
	} 
	return std::wstring(&resultstring[0]); 
}

std::string CharTransform::WideByte2Acsi(const std::wstring& wstrcode)
{
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL); 
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION) 
	{ 
		throw std::exception("Invalid UTF-8 sequence."); 
	} 
	if (asciisize == 0) 
	{ 
		throw std::exception("Error in conversion."); 
	} 
	std::vector<char> resultstring(asciisize); 
	int convresult =::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL); 
	if (convresult != asciisize) 
	{ 
		throw std::exception("La falla!"); 
	} 
	return std::string(&resultstring[0]); 
}

std::wstring CharTransform::Acsi2WideByte(std::string& strascii) 
{ 
	int widesize = MultiByteToWideChar (CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0); 
	if (widesize == ERROR_NO_UNICODE_TRANSLATION) 
	{ 
		throw std::exception("Invalid UTF-8 sequence."); 
	} 
	if (widesize == 0) 
	{ 
		throw std::exception("Error in conversion."); 
	} 
	std::vector<wchar_t> resultstring(widesize); 
	int convresult = MultiByteToWideChar (CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize) 
	{ 
		throw std::exception("La falla!"); 
	} 
	return std::wstring(&resultstring[0]); 
} 
//Unicode ת Utf8 
std::string CharTransform::Unicode2Utf8(const std::wstring& widestring) 
{ 
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL); 
	if (utf8size == 0) 
	{ 
		throw std::exception("Error in conversion."); 
	} 
	std::vector<char> resultstring(utf8size); 
	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL); 
	if (convresult != utf8size) 
	{ 
		throw std::exception("La falla!"); 
	} 
	return std::string(&resultstring[0]); 
} 

