
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
	 if(!mysql_real_connect(&mysql,"192.168.5.32","test","test","b2b_extract",3307,NULL,0))
	 {
		printf(",%s\n",mysql_error(&mysql));
		return 0;
	 }else{
		printf("connect success\n");
	 
	 }
	 	char str[255];
		int p_id = 0;
while(1){

	sprintf(str,"select prod_name_true, prod_desc , keyvalue ,prod_id from extract_product_3 where prod_id > %d limit 1 ",p_id);
	
   if(mysql_query(&mysql,str))
	 {
		printf(mysql_error(&mysql));
		break;
	 }
		  MYSQL_RES *result;
		  MYSQL_ROW row;
		  if(!(result=mysql_use_result(&mysql)))
		  { 
		   printf("read res failed"); 
		  }else
		  {			
			   if(row=mysql_fetch_row(result))
			   {		p_id = atoi(row[3]);
						string prod_name_true = row[0];
						string prod_desc = row[1];
						string keyvalue = row[2];
						char*ptr_prod_name_true = (char*)prod_name_true.c_str();
						char*ptr_prod_desc = (char*)prod_desc.c_str();
						char*ptr_keyvalue = (char*)keyvalue.c_str();
						replace_No_Ascii(ptr_prod_name_true);
						replace_No_Ascii(ptr_prod_desc);
						replace_No_Ascii(ptr_keyvalue);
						if(strcmp(ptr_prod_name_true,row[0])||strcmp(ptr_prod_desc,row[1])||strcmp(ptr_keyvalue,row[2])){
							int desc_len = prod_desc.size();
							int name_len = prod_name_true.size();
							int key_len = keyvalue.size();
							cout<<desc_len<<" "<<name_len<<" "<<key_len<<endl;
							
							char* to_prod_name_true = (char*)malloc(name_len*2);
							memset(to_prod_name_true,0,name_len*2);
							
							char* to_prod_desc = (char*)malloc(desc_len*2);
							memset(to_prod_desc,0,desc_len*2);
							
							char* to_keyvalue = (char*)malloc(key_len*2);
							memset(to_keyvalue,0,key_len*2);
							
							char* str2 = (char*)malloc((desc_len+key_len+name_len)*2+1000);
							memset(str2,'\0',(desc_len+key_len+name_len)*2+1000);
							
							mysql_real_escape_string(&mysql,to_prod_name_true, prod_name_true.c_str(), strlen(ptr_prod_name_true)); 
							mysql_real_escape_string(&mysql,to_prod_desc, prod_desc.c_str(), strlen(ptr_prod_desc));
							mysql_real_escape_string(&mysql,to_keyvalue, keyvalue.c_str(), strlen(ptr_keyvalue));
							sprintf(str2,"update extract_product_3 set prod_name_true = '%s', prod_desc = '%s', keyvalue = '%s' where prod_id = %s ",to_prod_name_true,to_prod_desc,to_keyvalue,row[3]);
							mysql_free_result(result);
							if(mysql_query(&mysql,str2)){
								cout<<"update failed"<<mysql_error(&mysql)<<endl;
							}else{
								cout<<"update success"<<endl;
							}
							if(to_prod_name_true)
								free(to_prod_name_true);
							if(to_prod_desc)
								free(to_prod_desc);
							if(to_keyvalue)
								free(to_keyvalue);
							if(str2)
								free(str2);
						}else{
							mysql_free_result(result);
							printf("NO no_ascii prod_id:%d \n",p_id);
							
						}
				}else{
					break;
				}
		  }   
}
	mysql_close(&mysql);
	return 0;
}

