#!/bin/bash

echo -e -n "1. Date\n2. Cal\n3. Ls\n4. Pwd\n5. Exit\n"
echo "Select any one"
read choice

case $choice in

1)
	date
	;;
2)	
	cal
	;;
3)
	ls
	;;
4)
	pwd
	;;
5)	
	exit
	;;	
*)	
	echo"Invaild opration."
esac
