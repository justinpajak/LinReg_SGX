#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>

using std::vector;

void print(vector<vector<float>> m);

void usage(int status);

vector<vector<float>> transpose(vector<vector<float>>& m);

vector<vector<float>> multiply(vector<vector<float>>& m1, vector<vector<float>>& m2);

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

	/* Get input X (n x p) matrix */
	vector<vector<float>> X = {{1, 2}, {3, 4}, {5, 6}};
	FILE *d_vars = fopen("x.txt", "r");
	if (!d_vars) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
	}

	
	/* Get input y (n x 1) vector */
	vector<vector<float>> y = {{}};


	/* 1. Compute X' (Transpose of X) */
	print(X);
	print(transpose(X));	

	/* 2. Compute X' * X --> result is p x p matrix */

	
	/* 3. Compute inverse of X' * X */


	/* 4. Multiply inverse result by X' */

	
	/* 5. Multiply result by y (n x 1) matrix --> result is beta hat, (p x 1) matrix*/


	return 0;
}

vector<vector<float>> transpose(vector<vector<float>>& m) {

	vector<vector<float>> result(m[0].size(), vector<float>());
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m[i].size(); j++) {
			result[j].push_back(m[i][j]);
		}
	}
	return result;
}

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


