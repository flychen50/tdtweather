all:
	g++ -o tdtweather SFLog.cpp iniFile.cpp tdtweather.cpp -I /usr/local/include/mysql -L /usr/local/lib/mysql  -lmysqlclient_r -Wl,-rpath,/usr/local/lib/mysql -pthread -g
	
