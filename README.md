# LinReg_SGX
Implementation of Linear Regression Algorithm in SGX using Graphene

- change p and n variables in rand_gen.py and run script to generate x.txt and y.txt (files with random floats)

- p and n variables must be the same in rand_gen.py and linreg.cpp. Then rand_gen.py must be ran before executing linreg.

1. g++ linreg.cpp -o linreg -lcrypto

2. make SGX=1 -f mk_graphene linreg.manifest.sgx linreg.token pal_loader

3. SGX=1 OMP_NUM_THREADS=1 ./pal_loader ./linreg

To clean:
make -f mk_graphene clean
