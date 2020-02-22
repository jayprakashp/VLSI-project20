//
//  rng.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//

#include <string.h>
#include "rng.h"
#include "aes.h"
//#include <openssl/conf.h>
//#include <openssl/evp.h>
//#include <openssl/err.h>


AES256_CTR_DRBG_struct  DRBG_ctx;

void print_aes256_struct() {
	int loop;
	for(loop=0;loop<32;loop++)
		printf("Key_enc[%d]=%d;\n",loop,DRBG_ctx.Key[loop]);
	for(loop=0;loop<16;loop++)
		printf("V_enc[%d]=%d;\n",loop,DRBG_ctx.V[loop]);
	printf("reseed_counter_enc=%d\n",DRBG_ctx.reseed_counter);
}

void fprint_aes256_struct(FILE *fp) {
	int loop;
	for(loop=0;loop<32;loop++)
		fprintf(fp, "Key_enc[%d]=%d;\n",loop,DRBG_ctx.Key[loop]);
	for(loop=0;loop<16;loop++)
		fprintf(fp, "V_enc[%d]=%d;\n",loop,DRBG_ctx.V[loop]);
	fprintf(fp, "reseed_counter_enc=%d;\n",DRBG_ctx.reseed_counter);
}

void write_aes256_struct(unsigned char Key1[32], unsigned char V1[16], int reseed_counter1) {
	int loop;
	for(loop=0;loop<32;loop++)
		DRBG_ctx.Key[loop]=Key1[loop];
	for(loop=0;loop<16;loop++)
		DRBG_ctx.V[loop]=V1[loop];
	DRBG_ctx.reseed_counter=reseed_counter1;
}

void    AES256_ECB(unsigned char *key, unsigned char *ctr, unsigned char *buffer);

/*
 seedexpander_init()
 ctx            - stores the current state of an instance of the seed expander
 seed           - a 32 byte random value
 diversifier    - an 8 byte diversifier
 maxlen         - maximum number of bytes (less than 2**32) generated under this seed and diversifier
 */
int
seedexpander_init(AES_XOF_struct *ctx,
                  unsigned char *seed,
                  unsigned char *diversifier,
                  unsigned long maxlen)
{
    if ( maxlen >= 0x100000000 )
        return RNG_BAD_MAXLEN;
    
    ctx->length_remaining = maxlen;
    
    memcpy(ctx->key, seed, 32);
    
    memcpy(ctx->ctr, diversifier, 8);
    ctx->ctr[11] = maxlen % 256;
    maxlen >>= 8;
    ctx->ctr[10] = maxlen % 256;
    maxlen >>= 8;
    ctx->ctr[9] = maxlen % 256;
    maxlen >>= 8;
    ctx->ctr[8] = maxlen % 256;
    memset(ctx->ctr+12, 0x00, 4);
    
    ctx->buffer_pos = 16;
    memset(ctx->buffer, 0x00, 16);
    
    return RNG_SUCCESS;
}

/*
 seedexpander()
    ctx  - stores the current state of an instance of the seed expander
    x    - returns the XOF data
    xlen - number of bytes to return
 */
int
seedexpander(AES_XOF_struct *ctx, unsigned char *x, unsigned long xlen)
{
    unsigned long   offset;
    
    if ( x == NULL )
        return RNG_BAD_OUTBUF;
    if ( xlen >= ctx->length_remaining )
        return RNG_BAD_REQ_LEN;
    
    ctx->length_remaining -= xlen;
    
    offset = 0;
    while ( xlen > 0 ) {
        if ( xlen <= (16-ctx->buffer_pos) ) { // buffer has what we need
            memcpy(x+offset, ctx->buffer+ctx->buffer_pos, xlen);
            ctx->buffer_pos += xlen;
            
            return RNG_SUCCESS;
        }
        
        // take what's in the buffer
        memcpy(x+offset, ctx->buffer+ctx->buffer_pos, 16-ctx->buffer_pos);
        xlen -= 16-ctx->buffer_pos;
        offset += 16-ctx->buffer_pos;
        
        AES256_ECB(ctx->key, ctx->ctr, ctx->buffer);
        ctx->buffer_pos = 0;
        
        //increment the counter
        for (int i=15; i>=12; i--) {
            if ( ctx->ctr[i] == 0xff )
                ctx->ctr[i] = 0x00;
            else {
                ctx->ctr[i]++;
                break;
            }
        }
        
    }
    
    return RNG_SUCCESS;
}


void handleErrors(void)
{
    // ERR_print_errors_fp(stderr);
    // abort();
}

// Use whatever AES implementation you have. This uses AES from openSSL library
//    key - 256-bit AES key
//    ctr - a 128-bit plaintext value
//    buffer - a 128-bit ciphertext value
void
AES256_ECB(unsigned char *key, unsigned char *ctr, unsigned char *buffer)
{
    
   
    struct AES_ctx ctx;
    
    int len = 16;
    
    int ciphertext_len;
    
    /* Create and initialise the context */
    AES_init_ctx(&ctx, key);

    // memcpy(buffer,ctr,16);

    // for (int i = 0; i < 16; i++) {
    // 	buffer[i] = ctr[i];
    // }

    uint8_t test[4][4];
    
    test[0][0] = ctr[0];
    test[0][1] = ctr[1];
    test[0][2] = ctr[2];
    test[0][3] = ctr[3];
    test[1][0] = ctr[4];
    test[1][1] = ctr[5];
    test[1][2] = ctr[6];
    test[1][3] = ctr[7];
    test[2][0] = ctr[8];
    test[2][1] = ctr[9];
    test[2][2] = ctr[10];
    test[2][3] = ctr[11];
    test[3][0] = ctr[12];
    test[3][1] = ctr[13];
    test[3][2] = ctr[14];
    test[3][3] = ctr[15];


    AES_ECB_encrypt(&ctx,(state_t*)test);

    buffer[0] = test[0][0];
    buffer[1] = test[0][1];
    buffer[2] = test[0][2];
    buffer[3] = test[0][3];
    buffer[4] = test[1][0];
    buffer[5] = test[1][1];
    buffer[6] = test[1][2];
    buffer[7] = test[1][3];
    buffer[8] = test[2][0];
    buffer[9] = test[2][1];
    buffer[10] = test[2][2];
    buffer[11] = test[2][3];
    buffer[12] = test[3][0];
    buffer[13] = test[3][1];
    buffer[14] = test[3][2];
    buffer[15] = test[3][3];


    ciphertext_len = len;
    
}

void
randombytes_init(unsigned char *entropy_input,
                 unsigned char *personalization_string,
                 int security_strength)
{
    unsigned char   seed_material[48];
    
    memcpy(seed_material, entropy_input, 48);
    if (personalization_string)
        for (int i=0; i<48; i++)
            seed_material[i] ^= personalization_string[i];
    memset(DRBG_ctx.Key, 0x00, 32);
    memset(DRBG_ctx.V, 0x00, 16);
    AES256_CTR_DRBG_Update(seed_material, DRBG_ctx.Key, DRBG_ctx.V);
    DRBG_ctx.reseed_counter = 1;
}

int
randombytes(unsigned char *x, unsigned long long xlen)
{
    unsigned char   block[16];
    int             i = 0;
    
    sign_label1:while ( xlen > 0 ) {
        //increment V
        for (int j=15; j>=0; j--) {
            if ( DRBG_ctx.V[j] == 0xff )
                DRBG_ctx.V[j] = 0x00;
            else {
                DRBG_ctx.V[j]++;
                break;
            }
        }
        AES256_ECB(DRBG_ctx.Key, DRBG_ctx.V, block);
        // if ( xlen > 15 ) {
        //     memcpy(x+i, block, 16);
        //     i += 16;
        //     xlen -= 16;
        // }
        // else {
        //     memcpy(x+i, block, xlen);
        //     xlen = 0;
        // }
        if ( xlen > 15 ) {
            //memcpy(x+i, block, 16);
        	int loop;
        	for (loop=0;loop<16;loop++)
        		x[i+loop] = block[loop];
            i += 16;
            xlen -= 16;
        }
        else {
        	int loop;
        	for (loop=0;loop<xlen;loop++)
        	      x[i+loop] = block[loop];
            //memcpy(x+i, block, xlen);
            xlen = 0;
        }
    }
    AES256_CTR_DRBG_Update(NULL, DRBG_ctx.Key, DRBG_ctx.V);
    DRBG_ctx.reseed_counter++;
    
    return RNG_SUCCESS;
}

int
randombytes_sign(unsigned char *x, unsigned long long xlen)
{
    unsigned char   block[16];
    int             i = 0;

    //sign_label1:while ( xlen > 0 ) {
        //increment V
        for (int j=15; j>=0; j--) {
            if ( DRBG_ctx.V[j] == 0xff )
                DRBG_ctx.V[j] = 0x00;
            else {
                DRBG_ctx.V[j]++;
                break;
            }
        }
        AES256_ECB(DRBG_ctx.Key, DRBG_ctx.V, block);
        // if ( xlen > 15 ) {
        //     memcpy(x+i, block, 16);
        //     i += 16;
        //     xlen -= 16;
        // }
        // else {
        //     memcpy(x+i, block, xlen);
        //     xlen = 0;
        // }
        //if ( xlen > 15 ) {
            //memcpy(x+i, block, 16);
        	int loop;
        	for (loop=0;loop<16;loop++)
        		x[i+loop] = block[loop];
            i += 16;
            xlen -= 16;
       // }
       /* else {
        	int loop;
        	for (loop=0;loop<xlen;loop++)
        	      x[i+loop] = block[loop];
            //memcpy(x+i, block, xlen);
            xlen = 0;
        }*/
    //}
    AES256_CTR_DRBG_Update(NULL, DRBG_ctx.Key, DRBG_ctx.V);
    DRBG_ctx.reseed_counter++;

    return RNG_SUCCESS;
}

void
AES256_CTR_DRBG_Update(unsigned char *provided_data,
                       unsigned char *Key,
                       unsigned char *V)
{
    unsigned char   temp[48];
    int i=0;
    for (i=0; i<3; i++) {
        //increment V
        for (int j=15; j>=0; j--) {
            if ( V[j] == 0xff )
                V[j] = 0x00;
            else {
                V[j]++;
                break;
            }
        }
        
        AES256_ECB(Key, V, temp+16*i);
    }
    if ( provided_data != NULL )
        for (i=0; i<48; i++)
            temp[i] ^= provided_data[i];
    // memcpy(Key, temp, 32);
    // memcpy(V, temp+32, 16)
    for (i=0;i<32;i++)
    	Key[i] = temp[i];
    for (i=0;i<16;i++)
    	V[i]=temp[32+i];;
}
