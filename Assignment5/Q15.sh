#!/bin/bash

# Accept filenames from the user
read -p "Enter the first filename: " file1
read -p "Enter the second filename: " file2

# Check if the first file exists
if [ ! -f "$file1" ]; then
    echo "File $file1 does not exist."
    exit 1
fi

# Read the content of the first file, reverse the case, and append it to the second file
tr 'a-zA-Z' 'A-Za-z' < "$file1" >> "$file2"

echo "Contents of $file1 have been appended to $file2 in reverse case."

