all: aes linreg

aes: aes.cpp
	g++ aes.cpp -o aes -lcrypto

linreg: linreg.cpp
	g++ linreg.cpp -o linreg -lcrypto

clean:
	rm aes linreg
