/*
 *
 * 
 * 
 */

#include "iniFile.h"


// C++ includes
#include <iostream>
#include <fstream>
//#include <strstream>

// C Includes
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>

// Local Includes
#if defined(WIN32)
#	define iniEOL endl
#else
#	define iniEOL '\r' << endl
#endif

#ifndef ASSERT
#	define ASSERT
#endif

//////////////////////////////////////////////////////////////////////////

IniFile::IniFile()
{
	_sensitive = false;
}

IniFile::~IniFile()
{
	iniClean();
}

// clean up .
void IniFile::iniClean( void )
{
	_keys.clear();
	_file_path.empty();
	_sensitive = false;
}

//  load ini file to class, or return -1 if not error.
int IniFile::iniLoad( const char * str )
{
	ASSERT(NULL != str && '\0' != str[0]);

	int retVal = 0;
	string	 key, name, value, line;
	string::size_type nleft, nright;
    fstream	 ini_file;

    ini_file.open(str, ios::in);
	if (ini_file.fail()) return -1;
	
    while(getline(ini_file, line)) 
	{
		// 去除行开始回车，换行，制表符
		int length = 0;
		while (length<line.length() && (' '==line[length] || '\r'==line[length] || '\n'==line[length] || '\t'==line[length] || (line[length] < 0x20 && line[length] >= 0)))
			length ++;
		if (length > 0 && length<=line.length()) line = line.substr(length, line.length());
		// 去除行结尾回车，换行，制表符
		length = line.length() - 1;
		while (length > 0 && ('\r'==line[length] || '\n'==line[length] || ' '==line[length] || '\t'==line[length] || (line[length] < 0x20 && line[length] >= 0)))
			length --;
		if (length > 0) line = line.substr(0, line.length());
		// 忽略空行
		if (0 == line.length()) continue;

		if (string::npos == (nleft = line.find_first_of(";#[=")))//判断类型 
			retVal = -1;

		switch (line[nleft]) 
		{
		case '[':  // [key] 键值
			if (string::npos != (nright = line.find_last_of("]")) && nright > nleft) 
			{
				key = line.substr(nleft + 1, nright - nleft - 1);
				key = skipSpace(key);
				//如果没有键值则默认为 default
				if (0 == key.length()) key = "default";
				if (addKey(key.c_str())) retVal = -1;
			}
			else retVal = -1;  break;
			
		case '=':  // name = value 变量
			if (string::npos != (nright = line.find_first_of(";#")))
				line = line.substr(0, nright);
			name = line.substr(0, nleft);
			name = skipSpace(name);
			if (0 == name.length()) retVal = -1;
			value = line.substr(nleft + 1);
			value = skipSpace(value);
			if (0 == value.length()) retVal = -1;

			//如果没有键值则用default
			if (0 == key.length()) key = "default";
			if (addValue(key.c_str(), name.c_str(), value.c_str())) retVal = -1;
			break;
		}
	}
	ini_file.close(); return retVal;	
}

// set ini file path.
void IniFile::setPath(const char * str)
{
	ASSERT(NULL != str && '\0' != str[0]);

	_file_path = str; return ;
}

// set charaters sensitive.
void IniFile::iniSensitive(bool val /* false */ )
{
	_sensitive = val;
}

// returns number of keys currently in the ini.
size_t IniFile::getNumKey() const
{
	return _keys.size();
}
// returns number of values stored for specified key.
size_t IniFile::getNumName(size_t keyID) const
{
	ASSERT(keyID >= 0 && keyID < _keys.size());
	ASSERT(_values[keyID].values.size() == _values[keyID].names.size());

	return _values[keyID].values.size();
}
// returns number of values stored for specified key.
size_t IniFile::getNumName(const char * key) const
{
	ASSERT(NULL != key && '\0' != key[0]);

	size_t keyID = findKey(key);
	if (noID == keyID) return noID;

	return getNumName(keyID);
}
// returns index of specified key, or noID if not found.
size_t IniFile::findKey(const char * key) const
{
	ASSERT(NULL != key && '\0' != key[0]);

	for (size_t keyID=0; keyID<_keys.size(); ++keyID)
	{
		if (_sensitive)
		{
			if (strcmp(key, _keys[keyID].c_str()))
				continue;
		}
		else 
		{
			if (stricmp(key, checkCase(_keys[keyID]).c_str()))
				continue;
		}
		return keyID;
	}
	return noID;
}
// returns index of specified key, or noID if not found.
size_t IniFile::findName(size_t keyID, const char * val) const
{
	ASSERT(NULL != val && '\0' != val[0]);
	ASSERT(keyID >= 0 && keyID < _keys.size());
	
	for (size_t valueID = 0; valueID < _values[keyID].names.size(); ++valueID)
	{
		if (_sensitive)
		{
			if (strcmp(val, _values[keyID].names[valueID].c_str()))
				continue;
		}
		else 
		{
			if (stricmp(val, checkCase(_values[keyID].names[valueID]).c_str()))
				continue;
		}
		return valueID;
	}
	return noID;	
}
// returns index of specified key, or noID if not found.
size_t IniFile::findName(const char * key, const char * val) const
{
	ASSERT(NULL != key && '\0' != key[0]);
	ASSERT(NULL != val && '\0' != val[0]);

	size_t keyID = findKey(key);
	if (noID == keyID) return noID;

	return findName(keyID, val);
}
// returns key names by index.
const char * IniFile::getKey(size_t keyID) const
{
	ASSERT(keyID >= 0 && keyID < _keys.size());

	return _keys[keyID].c_str();
}
// returns key names by index.
const char * IniFile::getName(const char * key, size_t valID) const
{
	ASSERT(NULL != key && '\0' != key[0]);
	ASSERT(valID >= 0 && _values[valID].values.size());

	size_t keyID = findKey(key);
	if (noID == keyID) return "";

	return getName(keyID, valID);
}
// returns key names by index.
const char * IniFile::getName(size_t keyID, size_t valID) const
{
	ASSERT(keyID >= 0 && keyID < _keys.size());
	ASSERT(valID >= 0 && _values[valID].values.size());

	return _values[keyID].values[valID].c_str();
}
// get string, int, float and bool.
// returns default value if key/value not found.
const char * IniFile::getStr(size_t keyID, const char * val, const char * def /* "" */) const
{
	ASSERT(keyID >= 0 && keyID < _keys.size());
	ASSERT(NULL != val && '\0' != val[0]);

	size_t valueID = findName(keyID, val);
	if (noID == valueID) return def;

	return _values[keyID].values[valueID].c_str();
}
// get string, int, float and bool.
// returns default value if key/value not found.
const char * IniFile::getStr(const char * key, const char * val, const char * def /* "" */) const
{
	ASSERT(NULL != key && '\0' != key[0]);
	ASSERT(NULL != val && '\0' != val[0]);

	size_t keyID = findKey(key);
	if (noID == keyID) return def;

	return getStr(keyID, val, def);
}
// get string, int, float and bool.
// returns default value if key/value not found.
int IniFile::getInt(size_t keyID, const char * val, int def /* 0 */) const
{
	ASSERT(keyID >= 0 && keyID < _keys.size());
	ASSERT(NULL != val && '\0' != val[0]);

	size_t valueID = findName(keyID, val);
	if (noID == valueID) return def;

	return atoi(_values[keyID].values[valueID].c_str());
}
// get string, int, float and bool.
// returns default value if key/value not found.
int IniFile::getInt(const char * key, const char * val, int def /* 0 */ ) const
{
	ASSERT(NULL != key && '\0' != key[0]);
	ASSERT(NULL != val && '\0' != val[0]);

	size_t keyID = findKey(key);
	if (noID == keyID) return def;

	return getInt(keyID, val, def);
}
// get string, int, float and bool.
// returns default value if key/value not found.
float IniFile::getFloat(size_t keyID, const char * val, float def /* 0.0f */ ) const
{
	ASSERT(keyID >= 0 && keyID < _keys.size());
	ASSERT(NULL != val && '\0' != val[0]);

	size_t valueID = findName(keyID, val);
	if (noID == valueID) return def;

	return atof(_values[keyID].values[valueID].c_str());
}
// get string, int, float and bool.
// returns default value if key/value not found.
float IniFile::getFloat(const char * key, const char * val, float def /* 0.0f */ ) const
{
	ASSERT(NULL != key && '\0' != key[0]);
	ASSERT(NULL != val && '\0' != val[0]);
	
	size_t keyID = findKey(key);
	if (noID == keyID) return def;
	
	return getFloat(keyID, val, def);	
}
// get string, int, float and bool.
// returns default value if key/value not found.
bool IniFile::getBool(size_t keyID, const char * val, bool def /* false */ ) const
{
	ASSERT(keyID >= 0 && keyID < _keys.size());
	ASSERT(NULL != val && '\0' != val[0]);
	
	size_t valueID = findName(keyID, val);
	if (noID == valueID) return def;
	
	if (0 == stricmp("true", _values[keyID].values[valueID].c_str()))
		return true;
	if (0 == stricmp("false", _values[keyID].values[valueID].c_str()))
		return false;

	return false;
}
// get string, int, float and bool.
// returns default value if key/value not found.
bool IniFile::getBool(const char * key, const char * val, bool def /* false */ ) const
{
	ASSERT(NULL != key && '\0' != key[0]);
	ASSERT(NULL != val && '\0' != val[0]);
	
	size_t keyID = findKey(key);
	if (noID == keyID) return def;
	
	return getBool(keyID, val, def);	
}
// add a key name.
int IniFile::addKey(const char * key)
{
	ASSERT(NULL != key && '\0' != key[0]);

	string str = key;
	struct _key_ val;

	if (noID != findKey(key))
		return -1;

	_keys.push_back(str);
	_values.push_back(val);

	return 0;
}
// add a new value if it doesn`t alread exit 
int IniFile::addValue(size_t  keyID, const char * name, const char * value)
{
	ASSERT(keyID >= 0 && keyID < _keys.size());
	ASSERT(NULL != name && '\0' != name[0]);
	ASSERT(NULL != value && '\0' != value[0]);
	
	string str1 = name, str2 = value;

	size_t valueID = findName(keyID, name);
	if (noID != valueID) return -1;

	_values[keyID].names.push_back(str1);
	_values[keyID].values.push_back(str2);

	return 0;
}

int IniFile::addValue(const char * key, const char * name, const char * value)
{
	ASSERT(NULL != key && '\0' != key[0]);
	ASSERT(NULL != name && '\0' != name[0]);
	ASSERT(NULL != value && '\0' != value[0]);

	size_t keyID = findKey(key);
	if (noID == keyID && addKey(key))
		return -1;

	return addValue(findKey(key), name, value);
}
//////////////////////////////////////////////////////////////////////////
//功能: 如果配置文件不是大小写敏感的,则全部转化为小写,否则不变
//参数: s   待转换的字符串
//返回值: 转换后的字符串
//////////////////////////////////////////////////////////////////////////
string IniFile::checkCase(const string & str) const
{
	string tmp = str;
	if (_sensitive)
	{
		for (string::size_type i=0; i<str.length(); ++i)
			tmp[i] = tolower(tmp[i]);
	}
	return tmp;
}
//////////////////////////////////////////////////////////////////////////
//功能: 忽略空格和引号
//参数: 待处理的字符串
//返回值: 处理后的字符串, 否则返回空
//////////////////////////////////////////////////////////////////////////
string IniFile::skipSpace(const string & str) const
{
	string::size_type  nlen = str.length();
	string::size_type  nbgn = 0;
	string::size_type  nend = nlen;

	while (nbgn < nlen 
		&& (str[nbgn] == ' ' 
		   || str[nbgn] == '\"' 
		   || str[nbgn] == '\'' 
		   || str[nbgn] == '\t' 
		   || str[nbgn] == '\r'
		   || str[nbgn] == '\n'))nbgn++;
	while (nend >= nbgn 
		&& (str[nend - 1] == ' ' 
		   || str[nend - 1] == '\"' 
		   || str[nend - 1] == '\''
		   || str[nend - 1] == '\t'
		   || str[nend - 1] == '\r'
		   || str[nend - 1] == '\n'))nend--;
	
	if (nend > nbgn)
		return str.substr(nbgn, nend - nbgn);
	else return "";
}

