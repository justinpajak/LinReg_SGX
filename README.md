# LinReg_SGX
Implementation of Linear Regression Algorithm in SGX using Graphene

Dependencies:
- Intel SGX: https://github.com/intel/linux-sgx
- Graphene: https://graphene.readthedocs.io/en/latest/quickstart.html#quick-start-with-sgx-support
- OpenSSL: https://www.openssl.org/

<hr/>
1. Compile program to encrypt data using AES-CBC with NI

	1. ~/LinReg_SGX$ make aes

<hr/>
2. Generate X matrix and y vetor data in x.txt and y.txt, encrypt, and store ciphertexts in enc_x.txt and enc_y.txt

	1. ~/LinReg_SGX$ ./scripts/gen.py -n 1000000 -p 10
    -  Generate p = 10 floats for n = 1000000 users

<hr/>
3. Run SGX Linear Regression Program

	1. ~/LinReg_SGX$ make linreg

	2. ~/LinReg_SGX$ make SGX=1 -f mk_graphene linreg.manifest.sgx linreg.token pal_loader

	3. ~/LinReg_SGX$ SGX=1 ./pal_loader ./linreg -n 1000000 -p 10
	
	- This program reads in encrypted data from enc_x.txt and enc_y.txt, decrypts it,
	  runs the linear regression on the data, and outputs the beta vector to beta.txt.

	- The total time taken to read in data, decrypt, and peform the linear regression algorithm is outputted.

<hr/>
4. Clean

	1. ~/LinReg$ make clean
	2. ~/LinReg$ make -f mk_graphene clean
	3. ~/LinReg$ ./scripts/cleaner.sh
