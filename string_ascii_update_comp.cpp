
#include "string"
#include "vector"
#include "mysql.h"
#include <stdio.h>
#include <iostream>
#include "iniFile.h"
#include "SFLog.h"
using namespace std;
char*&   replace_No_Ascii(char*&   str)   
  {   
	for(int i=0;i<strlen(str);i++)
		if(str[i] >= 0 && str[i] <= 127)
			continue;
		else
			str[i] = ' ';
	return   str;   
  }   
string&   replace_all_distinct(string&   str,const   string&   old_value,const   string&   new_value)   
{   
  for(string::size_type   pos(0);   pos!=string::npos;   pos+=new_value.length())   {   

	  if(   (pos=str.find(old_value,pos))!=string::npos   )   
			str.replace(pos,old_value.length(),new_value);   
	  else  
		  break;   
	}   
return   str;   
}   
int main(int argc, char* argv[])
{
	CSFLog* log;
	IniFile* ini;
	log = new CSFLog("log.txt");
	ini = new IniFile();
	ini->iniLoad("config_comp.ini");
	MYSQL mysql; //
	mysql_init (&mysql);
	mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"vertical");
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	string ip;
	string user;
	string pass;
	string db;
	string update_table;
	int port;
	ip = ini->getStr("database","host","");
	user = ini->getStr("database","username","");
	pass = ini->getStr("database","password","");
	db = ini->getStr("database","db","");
	port = ini->getInt("database","port",3306);
	update_table = ini->getStr("sql","update_table","");
	if(!mysql_real_connect(&mysql,ip.c_str(),user.c_str(),pass.c_str(),db.c_str(),port,NULL,0))
	 {
		printf(",%s\n",mysql_error(&mysql));
		log->LogError("mysql select",0,(char*)mysql_error(&mysql));
		return 0;
	 }else{
		printf("connect success\n");
		log->LogMsg("mysql",0,"select connect success");
	 
	 }

	int tempid = 0;
	char str[100];
	
   while(1)
	 {	
		sprintf(str, "select * from %s where id > %d limit 1",update_table.c_str(),tempid);
		if(!mysql_query(&mysql,str)){
		 MYSQL_RES *result;
		  MYSQL_ROW row;
		  if(!(result=mysql_use_result(&mysql)))
		  { 
		   printf("read res failed"); 
		   log->LogError("res",0,"read failed");
		   return 0;
		  }else
		  {
		  if(row=mysql_fetch_row(result))
		   {
			string id = row[0];
			tempid = atoi(id.c_str());
			string comp_name = row[2];
			string about_us = row[3];
			string address = row[7];
			char*ptr_comp_name = (char*)comp_name.c_str();
			char*ptr_about_us = (char*)about_us.c_str();
			char*ptr_address = (char*)address.c_str();
			replace_No_Ascii(ptr_comp_name);
			replace_No_Ascii(ptr_about_us);
			replace_No_Ascii(ptr_address);
			if(strcmp(ptr_comp_name,row[2])||strcmp(ptr_about_us,row[3])||strcmp(ptr_address,row[7])){
				char str2[150000];
				memset(str2,'\0',150000);
				char to_comp_name[50000];
				char to_about_us[50000];
				char to_address[50000];
				mysql_real_escape_string(&mysql,to_comp_name, comp_name.c_str(), strlen(ptr_comp_name)); 
				mysql_real_escape_string(&mysql,to_about_us, about_us.c_str(), strlen(ptr_about_us));
				mysql_real_escape_string(&mysql,to_address, address.c_str(), strlen(ptr_address));
				mysql_free_result(result);
				sprintf(str2,"update %s set comp_name = '%s', about_us = '%s', address = '%s' where id = %s ",update_table.c_str(),to_comp_name,to_about_us,to_address,id.c_str());
				if(mysql_query(&mysql,str2)){
					log->LogError("update failed",tempid,(char*)mysql_error(&mysql));
				}else{
					log->LogMsg("update",tempid,"success"); 
				}
			}else{
				log->LogMsg("No non-ASCII",atoi(row[0]),"update");
				mysql_free_result(result);
			}
		  }else{
			log->LogError("row get",tempid,(char*)mysql_error(&mysql));
			return 0;
		  }
		}  
		}else{
			log->LogError("query get",tempid,(char*)mysql_error(&mysql));
			return 0;
		}
	}
	mysql_close(&mysql);
	return 0;
}

