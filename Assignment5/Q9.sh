#!/bin/bash

echo "Enter the number"
read number

fact=1

while [ $number -gt 1 ]
do
	fact=$((fact * number))
	number=$((number - 1))
	done	
	echo $fact
