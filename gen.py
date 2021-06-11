#!/usr/bin/env python3

import sys, random

def main():

	p = 10
	n = 1000000
	arguments = sys.argv[1:]
	while arguments and arguments[0].startswith("-"):
		arg = arguments.pop(0)
		if arg == '-n':
			n = int(arguments.pop(0))
		if arg == '-p':
			p = int(arguments.pop(0))
	
	# Write to x.txt -> file that contians contents of X matrix (n x p)
	x = open("x.txt", "w")
	p_counter = 0
	for _ in range(n):
		for i in range(p):
			x.write("{0:.6f}".format(random.uniform(0, 0.1)))
			if i != p - 1:
				x.write(",")
		x.write("\n");
	x.close()

	# Write to y.txt -> file that contains value of y vector (n x 1)
	y = open("y.txt", "w")
	for i in range(n):
		y.write("{0:.3f}\n".format(random.uniform(0, 1)))
	y.close()


if __name__ == '__main__':
	main()
