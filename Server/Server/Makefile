#Makefile
#SomeWhereServer
#mysql的头文件需要放在/usr/local/mysql-connector-c++-8.0.18/include/下,库文件在/usr/local/mysql-connector-c++-8.0.18/lib64下
#boost的头文件放在/usr/local/include,库文件放在/usr/local/lib内boost版本为1.71.0
#redis的头文件,库文件在Headers内

CXX = g++
EXEC_TARGET = Server
SOURCE_CPP = main.cpp ./MysqlConnect/MysqlConnect.cpp SomeWhereServer.cpp RequestHandle.cpp
OBJECT_TEST = main.o

LDFLAGS_COMMON = -std=c++11 -lRedisConnect -lboost_system -lboost_thread -lcrypto -lmysqlcppconn8 -lmysqlcppconn -lssl -pthread
LPATH_COMMON = -I./Headers -I/usr/local/include
LLIB_COMMON =  -L./Libs_centos/ -L/usr/local/lib/

build:
	$(CXX) $(SOURCE_CPP) $(LPATH_COMMON) $(LLIB_COMMON) -o $(EXEC_TARGET) $(LDFLAGS_COMMON)
clean:
	rm -f $(EXEC_TARGET) *.o

