// SFLog.cpp: implementation of the CSFLog class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "SFLog.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CSFLog::CSFLog(const char* pszFilePath)
{
	strcpy(this->pszFilePath, pszFilePath);
	fp = NULL;
	if (strlen( pszFilePath) > 0)
	{
		fp = fopen(this->pszFilePath, "a");	
	}
	
	criLog = new pthread_mutex_t;
	pthread_mutex_init(criLog, NULL);
	
}

CSFLog::~CSFLog()
{
	if (fp )
	{
		fclose(fp);
	}
	pthread_mutex_destroy(criLog);
	delete criLog;
}

void CSFLog::LogError(char* pszSrcFile, int nLine, char* pszMsg)
{
	if (fp == NULL)
	{
		return;
	}
	char pszTmp[1024+1] = {0};
	char pszErr[1024+1] = {0};

	strerror_r(errno, pszErr, sizeof(pszErr));	
#ifndef WIN32
	snprintf(pszTmp, 1024, "[%s][%d][%d:%s]", pszSrcFile, nLine, 	errno, pszErr);
#else
	_snprintf(pszTmp, 1024, "[%s][%d][%d:%s]", pszSrcFile, nLine, 	errno, pszErr);
#endif

	if(pszMsg)
	{
		strcat(pszTmp, pszMsg);
	}

	WriteLog(pszTmp);
}

void CSFLog::LogMsg(char* pszSrcFile, int nLine, char* pszMsg)
{
	if (fp == NULL)
	{
		return ;
	}
	char pszTmp[1024+1] = {0};
	if (pszMsg)
	{
#ifndef WIN32
	snprintf(pszTmp, 1024, "[%s][%d]%s", pszSrcFile, nLine, pszMsg);
#else
	_snprintf(pszTmp, 1024, "[%s][%d]%s", pszSrcFile, nLine, pszMsg);
#endif
		
	}
	else
	{
#ifndef WIN32
		snprintf(pszTmp, 1024, "[%s][%d]", pszSrcFile, nLine);
#else
		_snprintf(pszTmp, 1024, "[%s][%d]", pszSrcFile, nLine);
#endif
		
	}
	
	WriteLog(pszTmp);
}


void CSFLog::LogMsg(char* pszSrcFile, int nLine, int rtn)
{
	if (fp == NULL)
	{
		return;
	}
	char pszTmp[1024+1] = {0};
	
#ifndef WIN32
	snprintf(pszTmp, 1024, "[%s][%d]%d", pszSrcFile, nLine, rtn);
#else
	_snprintf(pszTmp, 1024, "[%s][%d]%d", pszSrcFile, nLine, rtn);
#endif
	
	WriteLog(pszTmp);
}

void CSFLog::LogVal(char* pszSrcFile, int nLine, char* pszName, long nValue)
{
	if (fp == NULL)
	{
		return;
	}
	char pszTmp[1024+1] = {0};
	if (pszName)
	{
	
#ifndef WIN32
		snprintf(pszTmp, 1024, "[%s][%d]%s:%d", pszSrcFile, nLine, pszName, nValue);
#else
		_snprintf(pszTmp, 1024, "[%s][%d]%s:%d", pszSrcFile, nLine, pszName, nValue);
#endif
	WriteLog(pszTmp);
	}
}

void CSFLog::LogVal(char* pszSrcFile, int nLine, char* pszName, const char* pszValue)
{
	if (fp == NULL)
	{
		return ;
	}
	if (pszName && pszValue)
	{
	
	char pszTmp[1024+1] = {0};
#ifndef WIN32
		snprintf(pszTmp, 1024, "[%s][%d]%s:%s", pszSrcFile, nLine, pszName, pszValue);
#else
		_snprintf(pszTmp, 1024, "[%s][%d]%s:%s", pszSrcFile, nLine, pszName, pszValue);
#endif
	WriteLog(pszTmp);
	}
}



void CSFLog::LogPkg(char* pszSrcFile, int nLine, char* pPkg, int nLen)
{
	if (fp == NULL)
	{
		return ;
	}
	char pszTmp[1024+1] = {0};		
	if (pPkg)
	{
	
#ifndef WIN32
	snprintf(pszTmp, 1024, "[File:%s][Line:%d]\n", pszSrcFile, nLine);
#else
	_snprintf(pszTmp, 1024, "[File:%s][Line:%d]\n", pszSrcFile, nLine);
#endif
	WriteLog(pszTmp);
	WriteHex(pPkg, nLen);
	}
	
}

void CSFLog::WriteHex(char *str, int len)
{
    unsigned char c;
    char buf1[20],buf2[101];
    int i,j,count,loopcount;

		pthread_mutex_lock(criLog);

    if(str == NULL || fp == NULL)
    {
		pthread_mutex_unlock(criLog);

        return ;
    }   

		fprintf(fp,"OFFSET    -0--1--2--3--4--5--6--7-HEX-1--2--3--4--5--6--7  ---ASCII Value---\n");
    
    
    count = 0;
    if(len % 16)
        loopcount = len /16 + 1;
    else
        loopcount = len / 16;

    for(i = 0; i < loopcount;i++)
    {
        sprintf(buf1,"%07d   ",i*16);
        fputs(buf1,fp);

        memset(buf2,' ',100);
        memset(buf2+100,'\0',1);
        for(j = 0;j < 16;j++)
        {
           c = *(str+i*16+j);
 
           sprintf(&buf2[j*3],"%02x ",c);
           if(!isprint(c))
              c = '.';
        
           buf2[16*3+2+j] = c;
         
           count++;

           if(count == len)
              break;
        }

        if(count == len)
        {
            int k,len;
            len = strlen(buf2);
            for(k = 0;k < 48 + 2 - len ;k++)
                 buf2[len+k] = ' ';
        }
     
        buf2[16*3] = ' ';
        buf2[16*3+2+j+1] = '\n';
        buf2[16*3+2+j+2] = '\0';
        fputs(buf2,fp);
    }
    
	pthread_mutex_unlock(criLog);

    return ;
}

void CSFLog::WriteLog(char *pszMsg)
{
//	return;
	if (fp == NULL || pszMsg == NULL)
	{
		return;
	}
		
	pthread_mutex_lock(criLog);
#ifndef WIN32	
	fprintf(fp, "[TID:%u] %s\n", pthread_self(), pszMsg);
#else
	fprintf(fp, "%s\n", pszMsg);
#endif	
	fflush(fp);

	int n = ftell(fp);
	if( n > 10240000)
	{
		fclose(fp);
		char tmpfile[512];
		sprintf(tmpfile,"%s%d.txt",pszFilePath,time(NULL));
		rename(pszFilePath,tmpfile);
		fp = fopen(pszFilePath, "a");		
	}
	
	pthread_mutex_unlock(criLog);

	return;
}


void CSFLog::Log(long lLevel, char *pszFileName, long lLineNo, char *pFormat, ...)
{
    pthread_mutex_lock(criLog);
    if (fp && pszFileName && pFormat)
    {	    
	va_list ap;
	va_start(ap, pFormat);
	fprintf(fp, "\n[%s][%d] ", pszFileName, lLineNo);
	vfprintf(fp, pFormat, ap);	
	va_end(ap);	    
	int n = ftell(fp);
	if( n > 10240000)
	{
	    fclose(fp);
	    char tmpfile[512];
	    sprintf(tmpfile,"%s%d.txt",pszFilePath,time(NULL));
	    rename(pszFilePath,tmpfile);
	    fp = fopen(pszFilePath, "a");
	}
    }	
    pthread_mutex_unlock(criLog);
}



