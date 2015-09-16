#ifndef SHELL_BASE_HTTPDOWNLOAD_H_
#define SHELL_BASE_HTTPDOWNLOAD_H_

#include <Windows.h>



class MemoryMap;



enum DownType
{
	Down_Memory = 1
};


class HttpDownload
{
public:
	HttpDownload(DownType dtype = Down_Memory);
	~HttpDownload();

	int HttpDownloading(char *url);

	char* GetDownMemory(unsigned int& downMemLen);


private:
	DownType	m_downType; //обть
	char*		m_szUrl;

	MemoryMap*	m_pMemoeyMap;
};


#endif