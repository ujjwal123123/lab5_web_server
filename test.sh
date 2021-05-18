#!/bin/bash

# wget -P test1 -nd -p localhost:7090/index.html
./webserver 7209 &
PID=$!
wget -P /dev/null --page-requisites localhost:7209/index.html

counter=1
while [$counter -le 5]
do
    wget -P /dev/null --page-requisites localhost:7090/index.html
    ((counter++))
done
kill $PID

# P: output directory
# nd: no directories