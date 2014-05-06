
#include "string"
#include "vector"
#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
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
	MYSQL mysql; //
	mysql_init (&mysql);
	mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"vertical");
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	 if(!mysql_real_connect(&mysql,"10.0.4.11","root","mysql008","weather",3306,NULL,0))
	 {
		printf(",%s\n",mysql_error(&mysql));
		return 0;
	 }else{
		printf("connect success\n");
	 
	 }



	 	MYSQL mysql2; //
	mysql_init (&mysql2);
	mysql_options(&mysql2,MYSQL_READ_DEFAULT_GROUP,"vertical");
	mysql_options(&mysql2, MYSQL_SET_CHARSET_NAME, "utf8");
	 if(!mysql_real_connect(&mysql2,"10.0.4.5","root","root","jbp_new",3306,NULL,0))
	 {
		printf(",%s\n",mysql_error(&mysql2));
		return 0;
	 }else{
		printf("connect success\n");
	 
	 }


	 	char str[255];
		int p_id = 0;


	sprintf(str,"SELECT * FROM weather_data ");
	
   if(mysql_query(&mysql,str))
	 {
		printf(mysql_error(&mysql));
		//break;
	 }
		  MYSQL_RES *result;
		  MYSQL_ROW row;
		  if(!(result=mysql_use_result(&mysql)))
		  { 
		   printf("read res failed"); 
		  }else
		  {			
			   if(row=mysql_fetch_row(result))
			   {		
						string str0 = row[0];
						string str1 = row[1];
						string str2 = row[2];
						string str3 = row[3];
						string str4 = row[4];
						cout<<str0<<str1<<str2<<str3<<str4<<endl;
								
				}else{
					mysql_free_result(result);
					printf("NO no_ascii prod_id:%d \n",p_id);
					
				}
		  }   

	mysql_free_result(result);
	mysql_close(&mysql);
	return 0;
}

