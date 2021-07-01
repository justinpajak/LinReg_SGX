#!/bin/bash

for i in {1..10}
do
	SGX=1 ./pal_loader ./linreg -n 1000000 -p 10 2> /dev/null
done
