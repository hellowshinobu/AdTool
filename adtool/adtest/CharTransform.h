#pragma once
#include <string>

class CharTransform
{
public:
	CharTransform(void);
	~CharTransform(void);

	//utf8 to unicode
	static std::wstring Utf82Unicode(const std::string& utf8string) ;
	//unicode to ascii 
	static std::string WideByte2Acsi(const std::wstring& wstrcode);

	//Unicode to Utf8 
	static std::string Unicode2Utf8(const std::wstring& widestring);

	//ascii to Unicode 
	static std::wstring Acsi2WideByte(std::string& strascii);

};

