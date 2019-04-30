#!/bin/bash

total_fails=0
test_fails=0

for i in $(eval echo {1..$1})
do
    fails=$(make run p=$2 tag=$3 | grep -c FAILED )
	total_fails=$((fails+total_fails))	

	if [ $fails -gt 0 ]
	then
		test_fails=$((test_fails+1))
	fi

	echo "$i: $test_fails / $1"
done

echo "$test_fails failed tests out of $1 executions"
echo "$total_fails failed assertions over the course of all $1 executions amongst all $2 processes"
