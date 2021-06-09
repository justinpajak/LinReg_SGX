#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>

using std::vector;

void print(vector<vector<float>> m);

void usage(int status);

vector<vector<float>> transpose(vector<vector<float>>& m);

vector<vector<float>> multiply(vector<vector<float>>& m1, vector<vector<float>>& m2);

void cofactors(vector<vector<float>>& m, vector<vector<float>>& cofs, int p, int q, int n);

float determinant(vector<vector<float>>& m, int n, int p);

vector<vector<float>> adjoint(vector<vector<float>>& m);

vector<vector<float>> inverse(vector<vector<float>>& m);

int main(int argc, char *argv[]) {

	int p = 10; // number of independent variables
	int n = 1000; // number of users (points)

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
	vector<vector<float>> X(n, vector<float>(p));
	FILE *d_vars = fopen("x.txt", "r");
	if (!d_vars) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
	}

	char buffer[BUFSIZ];
	int line = 0;
	int val = 0;
	while (fgets(buffer, BUFSIZ, d_vars)) {
		char *token;
		token = strtok(buffer, ",");
		while(token != NULL) {
			X[line][val] = atof(token);
			val++;
			token = strtok(NULL, ",");
		}
		val = 0;
		line++;
	}
	fclose(d_vars);
	
	/* Get input y (n x 1) vector */
	vector<vector<float>> y(n, vector<float>(1));
	FILE *i_vars = fopen("y.txt", "r");
	if (!i_vars) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
	}
	line = 0;
	while (fgets(buffer, BUFSIZ, i_vars)) {
		y[line][0] = atof(buffer);
		line++;
	}
	fclose(i_vars);

	/* 1. Compute X' (Transpose of X) */
	vector<vector<float>> X_trans = transpose(X);	

	/* 2. Compute X' * X --> result is p x p matrix */
	vector<vector<float>> res = multiply(X_trans, X);
	
	/* 3. Compute inverse of X' * X */

	//printf("%f\n", determinant(res, p, p));


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

void cofactors(vector<vector<float>>& m, vector<vector<float>>& cofs, int p, int q, int n) {
	int i = 0;
	int j = 0;
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			if (r != p && c != q) {
				cofs[i][j] = m[r][c];
				j++;
				if (j == n - 1) {
					j = 0; 
					i++;
				}
			}	

		}
	}
}

float determinant(vector<vector<float>>& m, int n, int p) {
	float det = 0;
	if (n == 1) {
		return m[0][0];
	}

	vector<vector<float>> cofs(p, vector<float>(p));
	int sign = 1;
	for (int i = 0; i < n; i++) {
		cofactors(m, cofs, 0, i, n);
		det += sign * m[0][i] * determinant(cofs, n - 1, p);
		sign *= -1;
	}

	return det;
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


