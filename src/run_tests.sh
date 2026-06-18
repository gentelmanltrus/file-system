#!/bin/bash

echo "project through makefile"
make

if [ $? -ne 0 ]; then
    echo "compilation error!"
    exit 1
fi

echo "tests starting..."
# passing commands through eot (end of txt)
./file_system_app << EOT
mkdir test_folder/sub_folder/inner_folder
cd test_folder/sub_folder/inner_folder
touch file1.txt
touch file2.mp3
cd ..
touch file_in_sub.txt
report
tree
duplicates
quit
EOT

echo "tests completed"