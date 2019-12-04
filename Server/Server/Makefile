#Makefile
#SomeWhereServer
#mysql的头文件需要放在/usr/local/mysql-connector-c++-8.0.18/include/下,库文件在/usr/local/mysql-connector-c++-8.0.18/lib64下
#boost的头文件放在/usr/local/include,库文件放在/usr/local/lib内boost版本为1.71.0
#redis的头文件,库文件在Headers内

CXX = g++
EXEC_TARGET = Server
SOURCE_CPP = main.cpp ./RedisConnect/RedisConnect.cpp ./MysqlConnect/MysqlConnect.cpp SomeWhere_Login.cpp TcpConnect.cpp SomeWhereServer.cpp RequestHandle.cpp
OBJECT_TEST = main.o

LDFLAGS_COMMON = -std=c++11 -lboost_system -lhiredis -lcrypto -lmysqlcppconn8.2 -lmysqlcppconn8.2.8.0.18 -lmysqlcppconn.7 -lmysqlcppconn.7.8.0.18 -lmysqlcppconn-static -lmysqlcppconn8-static -lssl -pthread
LPATH_COMMON = -I./Headers -I/usr/local/mysql-connector-c++-8.0.18/include -I/usr/local/include
LLIB_COMMON = -L./Libs -L/usr/local/lib -L/usr/local/mysql-connector-c++-8.0.18/lib64

build:
	$(CXX) $(SOURCE_CPP) $(LDFLAGS_COMMON) $(LPATH_COMMON) $(LLIB_COMMON) -o $(EXEC_TARGET)
clean:
	rm -f $(EXEC_TARGET) *.o

