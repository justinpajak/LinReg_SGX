#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <string.h>
#include <iostream>

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char * key,
            unsigned char *iv, unsigned char *ciphertext);

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);

int main(int argc, char *argv[]) {

	/* 256 bit key */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* 128 bit IV */
    unsigned char *iv = (unsigned char *)"0123456789012345";

    /* Plaintext and Ciphertext declaration */
    unsigned char *plaintext;
    unsigned char ciphertext[128];
    int ciphertext_len, decryptedtext_len;

    /* Loop through x.txt file and encrypt each line */
    FILE* x_data = fopen("./data/x.txt", "r+");
    if (!x_data) {
        fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
        return 1;
    }
    FILE* x_enc = fopen("enc_x.txt", "w+");
    if (!x_enc) {
        fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
        return 1;
    }
    char buffer[BUFSIZ];
    while (fgets(buffer, BUFSIZ, x_data)) {
        plaintext = (unsigned char*)buffer;
        ciphertext_len = encrypt(plaintext, strlen((char*)plaintext), key, iv, ciphertext);
        fwrite((const char*)ciphertext, 128, 1, x_enc);
    };

    /* Loop through y.txt file and encrypt each float */
    FILE* y_data = fopen("./data/y.txt", "r+");
    if (!y_data) {
        fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
        return 1;
    }
    FILE* y_enc = fopen("enc_y.txt", "w+");
    if (!y_enc) {
        fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
        return 1;
    }
    while (fgets(buffer, BUFSIZ, y_data)) {
        plaintext = (unsigned char*)buffer;
        ciphertext_len = encrypt(plaintext, strlen((char*)plaintext), key, iv, ciphertext);
        fwrite((const char*)ciphertext, 128, 1, y_enc);
    }

    // test protocol: Decrypt all of enc.txt to dec.txt 
    /*FILE* dec_data = fopen("d.txt", "w");
    if (!dec_data) {
        fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
        return 1;
    }
    fseek(x_enc, 0L, SEEK_SET);
    while(fread((char*)ciphertext, 128, 1, x_enc)) {
        unsigned char decryptedtext[128];
        decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
        decryptedtext[decryptedtext_len] = '\0';
        fputs((const char*)decryptedtext, dec_data);
    }
    fclose(dec_data);*/

    fclose(x_data);
    fclose(x_enc);
    fclose(y_data);
    fclose(y_enc);

	return 0;
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;

	EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
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

