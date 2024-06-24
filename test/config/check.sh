#!/bin/sh

if [ "$1" = "success" ]
then
	for file in /etc/nginx/conf_file/success/*
		do
			nginx -t -c $file
		done
elif [ "$1" = "fail" ]
then
	for file in /etc/nginx/conf_file/fail/*
		do
			nginx -t -c $file
		done
else
	echo "Invalid args"
fi
