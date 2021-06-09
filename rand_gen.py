#!/usr/bin/env python3

import sys, random

def main():

	# Write to x.txt -> file that contains contains of X matrix (n x p)
	p = 3
	n = 3
	
	x = open("x.txt", "w")
	p_counter = 0
	for _ in range(n):
		for i in range(p):
			x.write("{0:.3f}".format(random.uniform(-5, 5)))
			if i != p - 1:
				x.write(",")
		x.write("\n");
	x.close()

	# Write to y.txt -> file that contains value of y vector (n x 1)
	y = open("y.txt", "w")
	for i in range(n):
		y.write("{0:.3f}\n".format(random.uniform(-5, 5)))
	y.close()


if __name__ == '__main__':
	main()
