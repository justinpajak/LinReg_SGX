#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <math.h>

using std::vector;

void print(vector<vector<float>> m);

void usage(int status);

vector<vector<float>> transpose(vector<vector<float>>& m);

vector<vector<float>> multiply(vector<vector<float>>& m1, vector<vector<float>>& m2);

void cofactors(vector<vector<float>>& m, vector<vector<float>>& cofs, int p, int q, int n);

float determinant(vector<vector<float>>& m, int n, int p);

vector<vector<float>> adjoint(vector<vector<float>>& m, int p);

vector<vector<float>> inverse(vector<vector<float>>& m, int p);

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
	std::cout << "finished X'" << std::endl;

	/* 2. Compute X' * X --> result is p x p matrix */
	vector<vector<float>> res = multiply(X_trans, X);
	std::cout << "finished X' * X" << std::endl;

	/* 3. Compute inverse of X' * X */
	res = inverse(res, p);
	std::cout << "finished inverse" << std::endl;

	/* 4. Multiply inverse result by X' */
	res = multiply(res, X_trans);
	std::cout << "finished * X'" << std::endl;
	
	/* 5. Multiply result by y (n x 1) matrix --> result is beta hat, (p x 1) matrix*/
	vector<vector<float>> beta = multiply(res, y);
	std::cout << "finished * y" << std::endl;

	//print(beta);

	/* Write data back to beta.txt file */
	FILE *b_data = fopen("beta.txt", "w");
	if (!b_data) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
	}
	for (int i = 0; i < beta.size(); i++) {
		fprintf(b_data, "%f\n", beta[i][0]);
	}
	fclose(b_data);

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

vector<vector<float>> adjoint(vector<vector<float>>& m, int p) {
	int sign = 1;
	vector<vector<float>> cofs(p, vector<float>(p));
	vector<vector<float>> adj(p, vector<float>(p));

	for (int i = 0; i < p; i++) {
		for (int j = 0; j < p; j++) {
			cofactors(m, cofs, i, j, p);
			sign = ((i + j) % 2 == 0) ? 1 : -1;
			adj[j][i] = sign * determinant(cofs, p - 1, p - 1);
		}
	}
	return adj;
}

vector<vector<float>> inverse(vector<vector<float>>& m, int p) {
	if (m.size() == 1 && m[0].size() == 1) {
		m[0][0] = pow(m[0][0], -1);
		return m;
	}
	float det = determinant(m, p, p);
	if (det == 0) {
		return m;
	}
	vector<vector<float>> a = adjoint(m, p);
	vector<vector<float>> inv(p, vector<float>(p));

	for (int i = 0; i < p; i++) {
		for (int j = 0; j < p; j++) {
			inv[i][j] = a[i][j]/det;	
		}
	}
	return inv;
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


