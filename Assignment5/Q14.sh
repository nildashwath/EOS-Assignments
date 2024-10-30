#!/bin/bash

# Display executable files in the current directory
echo "Executable files in the current directory:"
for file in *; do
    if [ -x "$file" ] && [ -f "$file" ]; then
        echo "$file"
    fi
done

