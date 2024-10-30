#!/bin/bash

# Prompt the user to enter the basic salary
read -p "Enter the basic salary: " basic_salary

# Calculate DA and HRA based on the given percentages
da=$(echo "$basic_salary * 0.40" | bc)
hra=$(echo "$basic_salary * 0.20" | bc)

# Calculate the gross salary
gross_salary=$(echo "$basic_salary + $da + $hra" | bc)

# Display the gross salary
echo "Gross Salary: $gross_salary"

