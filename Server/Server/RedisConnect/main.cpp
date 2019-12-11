#include "RedisConnect.hpp"
#include <iostream>

using namespace std;

int main()
{
	RedisConnect tmp;
	if(tmp.get_redis_status())
	{
		cout<<"redis connect";
	}
	else
	{
		cout<<"redis not connect";
	}
	return 0;
}
