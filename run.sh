#!/bin/bash

for i in {1...20}
do
	time SGX=1 ./pal_loader ./linreg -n 5000000 -p 2 2> /dev/null | head -1
done
