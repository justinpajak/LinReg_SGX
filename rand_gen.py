#!/usr/bin/env python3

import sys, random

def main():
	p = 10
	n = 1000000
	
	x = open("x.txt", "w")
	p_counter = 0
	for i in range(n):
		if p_counter == 10:
			p_counter = 0
			x.write("\n")

		x.write("{0:.3f}".format(random.uniform(0, 100)))
		p_counter += 1
		if p_counter < 10:
			x.write(",");
	x.close()

if __name__ == '__main__':
	main()
