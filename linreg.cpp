#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>

using std::vector;

void print(vector<vector<float>> m);

void usage(int status);

vector<vector<float>> transpose(vector<vector<float>>& m);

vector<vector<float>> multiply(vector<vector<float>>& m1, vector<vector<float>>& m2);

float determinant(vector<vector<float>>& m, int p);

vector<vector<float>> inverse(vector<vector<float>>& m);

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
	char buffer[BUFSIZ];
	while (fgets(buffer, BUFSIZ, d_vars)) {
		printf("%s", buffer);
	}

	fclose(d_vars);
	

	
	/* Get input y (n x 1) vector */
	vector<vector<float>> y = {{}};


	/* 1. Compute X' (Transpose of X) */
	vector<vector<float>> X_trans = transpose(X);	
	
	/* 2. Compute X' * X --> result is p x p matrix */
	vector<vector<float>> res = multiply(X_trans, X);
	print(res);
	
	/* 3. Compute inverse of X' * X */
	

	/* 4. Multiply inverse result by X' */

	
	/* 5. Multiply result by y (n x 1) matrix --> result is beta hat, (p x 1) matrix*/


	return 0;
}

vector<vector<float>> transpose(vector<vector<float>>& m) {

	vector<vector<float>> result(m[0].size(), vector<float>(m.size()));
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m[i].size(); j++) {
			result[j][i] = m[i][j];
		}
	}
	return result;
}

vector<vector<float>> multiply(vector<vector<float>>& m1, vector<vector<float>>& m2) {

	vector<vector<float>> result(m1.size(), vector<float>(m2[0].size()));
	for (int i = 0; i < m1.size(); i++) {
		for (int j = 0; j < m2[0].size(); j++) {
			for (int k = 0; k < m1[0].size(); k++) {
				result[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
	return result;
}

float determinant(vector<vector<float>>& m, int p) {

}

vector<vector<float>> inverse(vector<vector<float>>& m) {

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


