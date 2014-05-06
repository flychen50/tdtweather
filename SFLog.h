// SFLog.h: interface for the CSFLog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FLOG_H_
#define _FLOG_H_
#include <stdio.h>
#include <time.h>
//#include <pthread.h>
#include "GlobalAPI.h"
#include "stdarg.h"


class SFDLL_API CSFLog  
{
private:
	char pszFilePath[512];
private:
	void WriteLog(char* pszMsg);
	void WriteHex(char *str, int len);
public:
	FILE* fp;

public:
	pthread_mutex_t* criLog;

public:
	
	void LogError(char* pszSrcFile, int nLine, char* pszMsg);
	void LogMsg(char* pszSrcFile, int nLine, char* pszMsg);
	void LogMsg(char* pszSrcFile, int nLine, int rtn);
	void LogPkg(char* pszSrcFile, int nLine, char* pPkg, int nLen);
	void LogVal(char* pszSrcFile, int nLine, char* pszName,const char* pszValue);
	void LogVal(char* pszSrcFile, int nLine, char* pszName, long nValue);
	void Log(long lLevel, char *pszFileName, long lLineNo, char *pFormat, ...);


	CSFLog(const char* pszFilePath);
	virtual ~CSFLog();
};

#endif 


