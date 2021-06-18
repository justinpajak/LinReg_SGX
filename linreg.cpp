#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <math.h>
#include <chrono>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
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

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);

void readAndDecrypt(vector<vector<double>>& X, vector<vector<double>>& y, int p);

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
	vector<vector<double>> X(n, vector<double>(p));
	vector<vector<double>> y(n, vector<double>(1));

	auto start = std::chrono::high_resolution_clock::now();
	readAndDecrypt(X, y, p);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Read in data and decrypt.	Time taken: " << duration.count() / double(1000000) << " seconds." << std::endl;

	/* 1. Compute X' (Transpose of X) */
	start = std::chrono::high_resolution_clock::now();
	vector<vector<double>> X_trans = transpose(X);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "finished X'. 			Time taken: " << duration.count() / double(1000000) << " seconds." << std::endl;

	/* 2. Compute X' * X --> result is p x p matrix */
	start = std::chrono::high_resolution_clock::now();
	vector<vector<double>> res = multiply(X_trans, X);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "finished X' * X. 		Time taken: " << duration.count() / double(1000000) << " seconds." << std::endl;

	/* 3. Compute inverse of X' * X */
	start = std::chrono::high_resolution_clock::now();
	inverse(res, p);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "finished inverse. 		Time taken: " << duration.count() / double(1000000) << " seconds." << std::endl;

	/* 4. Multiply inverse result by X' */
	start = std::chrono::high_resolution_clock::now();
	res = multiply(res, X_trans);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "finished * X'.			Time taken: " << duration.count() / double(1000000) << " seconds." << std::endl;

	/* 5. Multiply result by y (n x 1) matrix --> result is beta hat, (p x 1) matrix*/
	start = std::chrono::high_resolution_clock::now();
	vector<vector<double>> beta = multiply(res, y);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "finished * y. 			Time taken: "<< duration.count() / double(1000000) << " seconds." << std::endl;

	/* Write data back to beta.txt file */
	start = std::chrono::high_resolution_clock::now();
	FILE *b_data = fopen("beta.txt", "w");
	if (!b_data) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
	}
	for (int i = 0; i < beta.size(); i++) {
		fprintf(b_data, "%f\n", beta[i][0]);
	}
	fclose(b_data);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Wrote beta vector to file.	Time taken: " << duration.count() / double(1000000) << " seconds." << std::endl;

	return 0;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

void readAndDecrypt(vector<vector<double>>& X, vector<vector<double>>& y, int p) {
	int enc_x = open("enc_x.txt", O_RDONLY);
	if (!enc_x) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
		return;
	}
	unsigned char ciphertext[128];
	unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
    unsigned char *iv = (unsigned char *)"0123456789012345";
    int ciphertext_len;
    if (p <= 2) {
    	ciphertext_len = 16;
    } else if (p <= 5) {
    	ciphertext_len = 32;
    } else if (p <= 7) {
    	ciphertext_len = 48;
    } else if (p <= 10) {
    	ciphertext_len = 64;
    } else if (p <= 13) {
		ciphertext_len = 80;
	} else if (p <= 15) {
		ciphertext_len = 96;
	} else {
		ciphertext_len = 112;
	}
    int user = 0;
	while (read(enc_x, (char*)ciphertext, 128)) {
		unsigned char plaintext[128];
		int plaintext_len = decrypt(ciphertext, ciphertext_len, key, iv, plaintext);
		plaintext[plaintext_len] = '\0';
		char *token = strtok((char*)plaintext, ",");
		int val = 0;
		while (token) {
			X[user][val] = atof(token);
			val++;
			token = strtok(NULL, ",");
		}
		user++;
	}
	close(enc_x);
	
	int enc_y = open("enc_y.txt", O_RDONLY);
	if (!enc_y) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
		return;
	}
	user = 0;
	while(read(enc_y, (char*)ciphertext, 128)) {
		unsigned char plaintext[128];
		int plaintext_len = decrypt(ciphertext, ciphertext_len, key, iv, plaintext);
		plaintext[plaintext_len] = '\0';
		y[user][0] = atof((char*)plaintext);
		user++;
	}
	close(enc_y);
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


