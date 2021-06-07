# LinReg_SGX
Implementation of Linear Regression Algorithm in SGX using Graphene

1. g++ linreg.cpp -o linreg

2. make SGX=1 -f mk_graphene linreg.manifest.sgx linreg.token pal_loader

3. SGX=1 ./pal_loader ./linreg

To clean:
make -f mk_graphene clean
