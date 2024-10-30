#!/bin/bash

echo -e -n "Enter the name: "
read name

if [ -e $name ]
then
	if [ -f $name ]
	then
		echo "It is a regular file"
		echo -e -n "File size: "
		stat -c %s $name
		
	else 
		echo "It is directory"
		echo -e -n "The content of directory:"
		ls -l $name
		fi

fi

