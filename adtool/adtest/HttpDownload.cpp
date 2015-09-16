#include "HttpDownload.h"
#include "stdafx.h"
#include <string> 
#include <wininet.h>   
#include <iostream>
#include <tchar.h>
#include <process.h>
#pragma comment(lib, "wininet.lib")  
#include <assert.h>
using namespace std;


class MemoryMap
{
public:
	MemoryMap()
	{
		m_pData = NULL;
		m_uSize = m_uAllocated = 0;
	}
	~MemoryMap()
	{
		if (m_pData != NULL)
		{
			free(m_pData);
			m_pData = NULL;
		}
		m_uSize = m_uAllocated = 0;
	}

	unsigned int WriteMemoryMap(void* pdata, unsigned int nDataCount, unsigned int nDataLen)
	{
		if (nDataLen <= 0)
			return m_uSize;

		if (m_pData == NULL)
		{
			m_pData = (char *)calloc(1024, sizeof(char));
			m_uAllocated = 1024;
		}

		if (m_uSize + nDataLen >= m_uAllocated)
		{
			unsigned int allocSize = m_uSize + nDataLen + 1024;
			char* temp = (char *)calloc(allocSize, sizeof(char));
			if (m_uSize > 0)
			{
				memcpy(temp, m_pData, m_uSize);
			}
			free(m_pData);
			m_pData = NULL;

			m_pData = temp;
			m_uAllocated = allocSize;
		}

		memcpy(m_pData + m_uSize, pdata, nDataLen);
		m_uSize += nDataLen;

		return m_uSize;
	}


	char*			m_pData;

	unsigned int    m_uSize;
	//calloc
	unsigned int    m_uAllocated;
};



HttpDownload::HttpDownload(DownType dtype) : m_pMemoeyMap(NULL), m_downType(dtype)
{
	if (m_downType == Down_Memory)
	{
		m_pMemoeyMap = new MemoryMap();
	}

}

HttpDownload::~HttpDownload()
{
	if (m_downType == Down_Memory)
	{
		delete m_pMemoeyMap;
		m_pMemoeyMap = NULL;
	}
}

int HttpDownload::HttpDownloading(char *url)
{
	URL_COMPONENTSA crackedURL;
	char  szBuffer[102400];//这里是下载缓冲区大小 1KB大小缓冲写入一次   
	char szHostName[128];
	char szUrlPath[512];
	ZeroMemory(&crackedURL, sizeof(URL_COMPONENTS));
	crackedURL.dwStructSize = sizeof(URL_COMPONENTS);
	crackedURL.lpszHostName = szHostName;
	crackedURL.dwHostNameLength = sizeof(szHostName);
	crackedURL.lpszUrlPath = szUrlPath;
	crackedURL.dwUrlPathLength = sizeof(szUrlPath);
	if (!InternetCrackUrlA(url, (DWORD)strlen(url), 0, &crackedURL))
		return -1;
	HINTERNET hInt = InternetOpenA("", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInt == NULL)
		return -2;
	HINTERNET hConn = InternetConnectA(hInt, crackedURL.lpszHostName, crackedURL.nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (hConn == NULL)
	{
		InternetCloseHandle(hInt);
		return -3;
	}
	DWORD dwTimeout = 10000;
	InternetSetOptionA(NULL, INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT, &dwTimeout, sizeof(DWORD));
	HINTERNET hReq = HttpOpenRequestA(hConn, "GET", crackedURL.lpszUrlPath, NULL, "", NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (hReq == NULL)
	{
		InternetCloseHandle(hInt);
		InternetCloseHandle(hConn);
		return -4;
	}
	string strHeader;
	strHeader += "Connection:Keep-Alive\r\n";
	string strRange = "Range: bytes=";
	strRange += "0";//IntToString(dwFileLen);
	strRange += "-";
	strRange + "\r\n";
	strHeader += strRange;
	DWORD dwRetCode = 0;
	DWORD dwSizeOfRq = sizeof(DWORD);
	if (HttpSendRequestA(hReq, strHeader.c_str(), strHeader.length(), NULL, 0)
		&& HttpQueryInfoA(hReq, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL)
		&& (dwRetCode >= 200 && dwRetCode < 400))
	{
		;
	}
	//else if (dwRetCode == 416)
	//{
	//	if (percent)
	//		*percent = 100;
	//	InternetCloseHandle(hInt);
	//	InternetCloseHandle(hConn);
	//	InternetCloseHandle(hReq);
	//	return TRUE;
	//}
	else
	{
		InternetCloseHandle(hInt);
		InternetCloseHandle(hConn);
		return -5;
	}

	char buff[64];
	DWORD dwContLen = 0;
	//查询文件大小   
	if (HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, &buff, &dwSizeOfRq, NULL))
		dwContLen = atol(buff);

	//开始读取文件   
	DWORD  dwLen;
	while (true)
	{
		if (InternetReadFile(hReq, szBuffer, sizeof(szBuffer), &dwLen))
		{
			if (dwLen == 0)
				break;
			m_pMemoeyMap->WriteMemoryMap(szBuffer, 1, dwLen);
			//fwrite(szBuffer, 1, dwLen, file);
		}
	}
	InternetCloseHandle(hInt);
	InternetCloseHandle(hConn);
	InternetCloseHandle(hReq);

	return 1;
}

char* HttpDownload::GetDownMemory(unsigned int& downMemLen)
{
	downMemLen = m_pMemoeyMap->m_uSize;
	return m_pMemoeyMap->m_pData;
}

