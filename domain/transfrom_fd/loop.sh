#!/bin/bash
while [ 1 ]
do
	./a.out
	sleep 1;
	cat 1.txt | wc -c
done
