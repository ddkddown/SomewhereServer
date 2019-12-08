#!/bin/bash
if [ $(whoami) != "root" ]
then
	echo "current user should be root"
	exit 1 
fi


if ! cp -rf ../Libs /usr/local/lib/
then
	echo "cp -rf ../Libs to /usr/local/lib/ failed!"
	exit 1
fi

if ! cp -rf ../Headers /usr/local/include/
then
	echo "cp -rf ../Headers to /usr/local/include/ failed!"
	exit 1
fi

echo "local0.*						/var/log/somewhere_server"
