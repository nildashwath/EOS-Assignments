#!/bin/bash

echo -e -n "Enter num1:"
read n1
echo -e -n "Enter num2:"
read n2
echo -e -n "Enter num3:"
read n3

if [ $n1 -gt $n2 ]
then
	if  [ $n1 -gt $n3 ]
	then
	echo "N1 is greater : $n1"
	else
	echo "N3 is greater : $n3"
	fi
else
	if [ $n2 -gt $n3 ]
	then
		echo "N2 is greater : $n2"
	else
		echo "N3 is greater : $n3"
	fi
fi
