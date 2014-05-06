#ifndef SFPORTABLELIB_H_
#define SFPORTABLELIB_H_



#ifndef WIN32
#define SFDLL_API
#elif defined SFDLL_EXPORTS
#define SFDLL_API 
#else
#define SFDLL_API 
#endif



typedef unsigned int SFDLL_uint32;
typedef signed int SFDLL_int32;



#ifdef WIN32
	#pragma comment(lib,"ws2_32")
	#include <WINSOCK2.H>
	#include <windows.h>
	#include <stdio.h>
	#include <sys/timeb.h>
	#include <io.h>
	#include <time.h>

	#define u_int64_t unsigned __int64

	#define pthread_mutex_t CRITICAL_SECTION
	#define pthread_mutex_lock(x) EnterCriticalSection(x)
	#define pthread_mutex_unlock(x) LeaveCriticalSection(x)
	#define pthread_mutex_init(x,y)	InitializeCriticalSection(x)
	#define pthread_mutex_destroy(x) DeleteCriticalSection(x)
	#define sleep(x) Sleep(x)
	#define pthread_t DWORD
	#define pthread_create(x1,x2,x3,x4) CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)x3,x4,0,x1)
	#define FCGX_Accept_r(x) 0
	#define FCGX_Init()	0
	#define FCGX_Finish_r(x)	0
	#define FCGX_FPrintF(x,y, z)	
	#define FCGX_InitRequest(x,y,z)
	#define FCGX_GetParam(x,y)  0
	#define strerror_r(x,y,z)
	#define __int64_t __int64
	#define strcasecmp stricmp
	#define strncasecmp strnicmp
	#define snprintf _snprintf
	#define rindex strrchr
	#define EINPROGRESS 1
	#define socklen_t int
//	#define gettimeofday(x,y) 

#else
	#include <sys/types.h>
	#include <sys/socket.h>	
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <fcntl.h>
	#include <string.h>
	#include <unistd.h>
	#include <strings.h>
	#include <stdlib.h>
	#include <errno.h>
	#include <sys/timeb.h>
	#include <sys/time.h>
	#include <dirent.h>

	#include <pthread.h>
	#define SOCKET int
	#define closesocket(x) close(x)
	#define strnicmp(x,y,z) strncasecmp(x,y,z)
	#define stricmp(x,y) strcasecmp(x,y)
	#define INVALID_SOCKET (-1)
	#define SOCKET_ERROR (-1)

	#define CRITICAL_SECTION pthread_mutex_t 
	#define EnterCriticalSection(x) pthread_mutex_lock(x)
	#define LeaveCriticalSection(x) pthread_mutex_unlock(x)
	#define	InitializeCriticalSection(x) pthread_mutex_init(x,NULL)
	#define DeleteCriticalSection(x) pthread_mutex_destroy(x)
	#define Sleep( x ) CSFUtil::SFSleep( 0 , x )
//	#define UINT64  u_int64_t
#endif

#define SERVERTIMEOUT -9999


#define Min(a,b)			((a)<(b)?(a):(b))
#define Max(a,b)			((a)>(b)?(a):(b))
#define SafeDelete(_x)		{ if (_x) { delete (_x); (_x) = NULL; } }
#define SafeDeleteArray(_x)	{ if (_x) { delete [] (_x); (_x) = NULL; } }


#endif//__GLOABLE__SYS__HEAD__

