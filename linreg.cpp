#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <math.h>
#include <chrono>
#include <unistd.h>
#include <fcntl.h>

using std::vector;

void print(vector<vector<double>> m);

void usage(int status);

vector<vector<double>> transpose(vector<vector<double>>& m);

vector<vector<double>> multiply(vector<vector<double>>& m1, vector<vector<double>>& m2);

void cofactors(vector<vector<double>>& m, vector<vector<double>>& cofs, int p, int q, int n);

double determinant(vector<vector<double>>& m, int n, int p);

void adjoint(vector<vector<double>>& m, vector<vector<double>>& a, int p);

void inverse(vector<vector<double>>& m, int p);

void readData(vector<vector<double>>& X, vector<vector<double>>& y, int p);

int main(int argc, char *argv[]) {

	int p = 10; // number of independent variables
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
	
	/* Read enc_x.txt and enc_y.txt into enclave. Decrypt and store in data structures */
	auto start = std::chrono::high_resolution_clock::now();
	vector<vector<double>> X(n, vector<double>(p));
	vector<vector<double>> y(n, vector<double>(1));
	readData(X, y, p);

	/* 1. Compute X' (Transpose of X) */
	vector<vector<double>> X_trans = transpose(X);

	/* 2. Compute X' * X --> result is p x p matrix */
	vector<vector<double>> left = multiply(X_trans, X);

	/* 3. Compute inverse of X' * X */
	inverse(left, p);

	/* 4. Compute X'y  */
	vector<vector<double>> right = multiply(X_trans, y);

	/* 5. Multiply (X'X)^-1 times (X'y) --> result is beta hat, (p x 1 matrix)  */
	vector<vector<double>> beta = multiply(left, right);
	
	/* Write data back to beta.txt file */
	FILE *b_data = fopen("beta.txt", "w");
	if (!b_data) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
	}
	for (int i = 0; i < beta.size(); i++) {
		fprintf(b_data, "%f\n", beta[i][0]);
	}
	fclose(b_data);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << duration.count() / float(1000000) << std::endl;
	
	return 0;
}

void readData(vector<vector<double>>& X, vector<vector<double>>& y, int p) {
	int data_x = open("x.txt", O_RDONLY);
	if (!data_x) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
		return;
	}
    int user = 0;
	char buffer[BUFSIZ];
	while (read(data_x, (char*)buffer, 6 * p) > 0) {
		buffer[6 * p] = '\0';
		char *token = strtok((char*)buffer, ",");
		int val = 0;
		while (token) {
			X[user][val] = atof(token);
			val++;
			token = strtok(NULL, ",");
		}
		user++;
	}
	close(data_x);

	int data_y = open("y.txt", O_RDONLY);
	if (!data_y) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
		return;
	}
	user = 0;
	while(read(data_y, (char*)buffer, 6)) {
		y[user][0] = atof((char*)buffer);
		user++;
	}
	close(data_y);
}

vector<vector<double>> transpose(vector<vector<double>>& m) {

	vector<vector<double>> result(m[0].size(), vector<double>(m.size()));
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m[i].size(); j++) {
			result[j][i] = m[i][j];
		}
	}
	return result;
}

vector<vector<double>> multiply(vector<vector<double>>& m1, vector<vector<double>>& m2) {


	vector<vector<double>> result(m1.size(), vector<double>(m2[0].size()));
	for (int i = 0; i < m1.size(); i++) {
		for (int j = 0; j < m2[0].size(); j++) {
			for (int k = 0; k < m1[0].size(); k++) {
				result[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
	return result;
}

void cofactors(vector<vector<double>>& m, vector<vector<double>>& cofs, int p, int q, int n) {
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

double determinant(vector<vector<double>>& m, int n, int p) {
	double det = 0;
	if (n == 1) {
		return m[0][0];
	}

	vector<vector<double>> cofs(p, vector<double>(p));
	int sign = 1;
	for (int i = 0; i < n; i++) {
		cofactors(m, cofs, 0, i, n);
		det += sign * m[0][i] * determinant(cofs, n - 1, p);
		sign = -sign;
	}

	return det;
}

void adjoint(vector<vector<double>>& m, vector<vector<double>>& a, int p) {

	if (p == 1) {
		a[0][0] = 1;
		return;
	}
	int sign = 1;
	vector<vector<double>> cofs(p, vector<double>(p));

	for (int i = 0; i < p; i++) {
		for (int j = 0; j < p; j++) {
			cofactors(m, cofs, i, j, p);
			sign = ((i + j) % 2 == 0) ? 1 : -1;
			a[j][i] = sign * determinant(cofs, p - 1, p - 1);
		}
	}
}

void inverse(vector<vector<double>>& m, int p) {
	double det = determinant(m, p, p);
	if (det == 0) {
		return;
	}
	vector<vector<double>> a(p, vector<double>(p));
	adjoint(m, a, p);

	for (int i = 0; i < p; i++) {
		for (int j = 0; j < p; j++) {
			m[i][j] = a[i][j]/det;	
		}
	}
}

void usage(int status) {

	fprintf(stderr, "Usage: ./linreg ...\n");
	fprintf(stderr, "       -p       Specify # of independent variables\n");
	fprintf(stderr, "       -n       Specify # of users (data points)\n");
	exit(status);
}

void print(vector<vector<double>> m) {
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m[i].size(); j++) {
			std::cout << m[i][j] << " ";
		}
		std::cout << std::endl;
	}
}


