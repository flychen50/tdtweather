/*
 *
 * 
 */

#ifndef _INI_FILE_H_ZONGSOBOT_
#define _INI_FILE_H_ZONGSOBOT_
// disable warning
#pragma warning(disable: 4786)



//////////////////////////////////////////////////////////////////////////


#include <string>
#include <vector>
#include "GlobalAPI.h"

#ifndef MAX_KEYNAME
#	define MAX_KEYNAME    128
#endif

#ifndef MAX_VALUENAME
#	define MAX_VALUENAME  128
#endif

#ifndef MAX_VALUEDATA
#	define MAX_VALUEDATA 2048
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////////
//配置文件格式说明:
//	配置文件可以带头部注释, 且注释以字符分号和井号(;#)开始, 每个变量可以属于
//某个键值也可以不属于某个键值, 默认其属于default键值, 变量的值可以带有引号
//(")也可不带, 对变量的注释, 需放置在变量下部或右部
//格式实例:
//      |;头部说明
//		|[键值]
//		|变量名称 = 变量值
//		|;对上部变量的说明
//		|变量名称 = "变量值"
//		|;对上部变量的说明
//////////////////////////////////////////////////////////////////////////


class SFDLL_API IniFile
{
public:
	enum errors { noID = -1 };
public: // construction and desturction
	IniFile();
	 ~IniFile();

public: // interface functions

	// enum errors { noID = -1 };

	// set charaters sensitive.
	 void  iniSensitive( bool val = false );	
	// set ini file path.
	 void       setPath( const char * str );
	//  load ini file to class, or return -1 if not error.
	 int        iniLoad( const char * str );
	// clean up.
	 void                  iniClean( void );
	
public: // members operation
	// returns number of keys currently in the ini.
	 size_t		      getNumKey( void ) const;
	
	// returns number of values stored for specified key.
	 size_t     getNumName(size_t keyID) const;
	 size_t getNumName(const char * key) const;
	
	// returns index of specified key or value, or noID if not found.
	 size_t					   findKey(const char * key) const;
	 size_t     findName(size_t keyID, const char * name) const;
	 size_t findName(const char * key, const char * name) const;
	
	// returns key or value name by index.
	 const char *                     getKey(size_t keyID) const;
	 const char *     getName(size_t keyID, size_t nameID) const;
	 const char * getName(const char * key, size_t nameID) const;
	
	// get string, int, float and bool.
	// returns default value if key/value not found.
	 const char *     getStr(size_t keyID, const char * name, const char * def = "") const;
	 const char * getStr(const char * key, const char * name, const char * def = "") const;
	 int                        getInt(size_t keyID, const char * name, int def = 0) const;
	 int                    getInt(const char * key, const char * name, int def = 0) const;
	 float               getFloat(size_t keyID, const char * name, float def = 0.0f) const;
	 float           getFloat(const char * key, const char * name, float def = 0.0f) const;
	 bool                 getBool(size_t keyID, const char * name, bool def = false) const;
	 bool             getBool(const char * key, const char * name, bool def = false) const;
		
protected: // members operation
	// add a key name.
	int	 										 addKey(const char * key);
	// add a new value if it doesn`t alread exit 
	int    addValue(size_t  keyID, const char * name, const char * value);
	int addValue(const char * key, const char * name, const char * value);

	string skipSpace(const string & str) const;
	string checkCase(const string & str) const;

private: // typedef struct _key_
	struct _key_
	{
		vector<string> names;
		vector<string> values;
	}; // typedef struct _key_

private:
	bool						_sensitive;		//是否为大小写敏感
	string						_file_path;		//配置文件保存路径
	
	vector<_key_>				_values;		 //name = value
	vector<string>				_keys;			//key name

}; // end class IniFile

#endif // #define _INI_FILE_H_ZONGSOBOT_


