#!/bin/bash

echo -e -n "Enter the number: "
read num

for((i = 2; i <= $num/2; i++))
	do
		ans=$((num%i))
		if [ $ans -eq 0 ]
		then
			echo "$num is not a prime number."
			exit 0
			fi
			done
			echo "$num is prime number."


