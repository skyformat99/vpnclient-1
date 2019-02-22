// ConsoleApplication1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "curl/curl.h"


#define POSTURL "http://www.hao123.com"
#define FILEPATH "D:\\temp.txt"


//����libcurl�������ݵĻص��������൱��recv����ѭ��
//����stream�����Զ����������ͣ������Ҵ�������ļ�����·��
static size_t write_callback( void *ptr, size_t size, size_t nmemb, void *stream )
{
	int len = size * nmemb;
	int written = len;
	FILE *fp = NULL;

	if ( _access( (char*)stream, 0 ) == -1 )
	{
		fp = fopen( (char*) stream, "wb" );
	}
	else
	{
		fp = fopen( (char*) stream, "ab" );
	}
	if (fp)
	{
		fwrite( ptr, size, nmemb, fp );
	}
	return written;
}


int GetUrl( const char *url, char *savepath )
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *chunk = NULL;

	curl = curl_easy_init();
	if ( curl ) {
		curl_easy_setopt( curl, CURLOPT_VERBOSE, 0L );
		curl_easy_setopt( curl, CURLOPT_URL, url );
		//ָ���ص�����
		curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_callback);
		//�����������Ϊ���ջ򴫵����ݵ�����
		curl_easy_setopt( curl, CURLOPT_WRITEDATA, savepath );
		res = curl_easy_perform( curl );
		if (res == CURLE_OK)
		{
			return 1;
		}
		return 0;
	}

	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
 	if ( GetUrl(POSTURL, FILEPATH) )
 	{
 		printf( "OK\n" );
 	}

	return 0;
}
