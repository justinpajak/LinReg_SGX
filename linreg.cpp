#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>

using std::vector;

void print(vector<vector<float>> m);

void usage(int status);

int main(int argc, char *argv[]) {

	int p = 10; 	 // number of independent variables
	int n = 1000000; // number of users (points)

	int argind = 1;
	while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
		char *arg = argv[argind++];
		switch(arg[1]) {
			case 'h':
				usage(0);
				break;
			case 'p':
				p = atoi(argv[argind++]);
				break;
			case 'n':
				n = atoi(argv[argind++]);
				break;
			default:
				break;
		}
	}

	

	return 0;
}

	// Get input from user "X" (n x p) matrix.
	// n = # of users
	// p = # of independent variables (feature values)
	//vector<vector<float>> test = {{4, 45, 32}, {234, 5, 2}, {344, 44, 3}, {32, 0, 34}};

	// Get input from user "y" (n x 1) vector
	// n = # of users
	// y = dependent variable for each user 


	/* 1. Compute X' (Transpose of X) */
		

	/* 2. Compute X' * X --> result is p x p matrix */

	
	/* 3. Compute inverse of X' * X */


	/* 4. Multiply inverse result by X' */

	
	/* 5. Multiply result by y (n x 1) matrix --> result is beta hat, (p x 1) matrix*/


void usage(int status) {

	fprintf(stderr, "Usage: ./linreg ...\n");
	fprintf(stderr, "       -p       Specify # of independent variables\n");
	fprintf(stderr, "       -n       Specify # of users (data points)\n");
	exit(status);
}

void print(vector<vector<float>> m) {
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m[i].size(); j++) {
			std::cout << m[i][j] << " ";
		}
		std::cout << std::endl;
	}
}


