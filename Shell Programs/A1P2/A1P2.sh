#Write a shell script that counts and prints the total number of files in current working directory 
#and in all of its sub-directories. Also (i) print total no. of files (not directories) 
#in each subdirectory with the name of the subdirectory, (ii) print only those file names that have been created 
#within the past 2 days.

#!/bin/bash

echo "My current working directory is: $PWD"
# echo "Total number of files in this directory is: "
# find . -type d -or -type f -printf '.' | wc -c

files=( * )
let count=0
let subcount=0
for file in "${files[@]}"; do
    if [[ -d $file ]]; then
        subcount=0
        subfiles=( "$file"/* )
        for subfile in "${subfiles[A]}"; do
            if [[ -f $subfile ]]; then
                subcount=$((subcount+1))
            fi
        done
        echo "$file : $subcount"
        count=$((count+subcount))
    else
        count=$((count+1))
    fi
done
echo "Count : $count"