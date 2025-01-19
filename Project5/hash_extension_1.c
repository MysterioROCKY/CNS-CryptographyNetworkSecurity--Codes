/*
How to Run the code :->

Step 1) Add this file "hash_extension_1.c" in hash_extender folder
Step 2) Go into the hash_extender folder by "cd hash_extender"
Step 3) In terminal run command : gcc -o hash_extension_1 hash_extension_1.c -lssl -lcrypto
Step 4) In same terminal run command : ./hash_extension_1

-> OpenSSL 3.0 deprecated the older MD5-related functions (MD5_Init, MD5_Update, and MD5_Final) in favor of 
   newer APIs for hashing (such as EVP functions). These older functions also generate warnings in the code 
   when used with OpenSSL 3.0.
   - So, I have switched to the EVP API in the below code.

Output: Original hash (H(secret || data)): e1a03458f948979ea797afcfd1e4794a
        H(secret || data || append): 728ab9e2811aae1f3769508e10e286fe        
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/evp.h>

// Function to print the hash in hex format
void print_hash(unsigned char *hash, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

// Function to calculate padding for the original message
void calculate_padding(size_t original_length, unsigned char *padding, size_t *padding_length) {
    size_t total_length = original_length;
    size_t message_bit_length = original_length * 8;

    // Step 1: Start padding with 0x80 (10000000 in binary)
    padding[0] = 0x80;
    memset(padding + 1, 0, 63);

    // Step 2: Add the length of the original message in bits at the end of the padding
    for (int i = 0; i < 8; i++) {
        padding[56 + i] = (message_bit_length >> (8 * (7 - i))) & 0xFF;
    }

    // Calculate the total padding length
    *padding_length = (total_length % 64 <= 56) ? (56 - total_length % 64) + 8
                                                : (120 - total_length % 64) + 8;
}

// Function to simulate the length extension attack
void length_extension_attack(
    unsigned char *original_hash, size_t original_length, const char *append) {

    unsigned char padding[64];
    size_t padding_length;
    unsigned char final_hash[EVP_MAX_MD_SIZE];
    unsigned int final_hash_length;

    // Step 1: Calculate the padding for the original message
    calculate_padding(original_length, padding, &padding_length);

    // Step 2: Use EVP_MD_CTX to perform the attack
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();

    if (!ctx) {
        fprintf(stderr, "Failed to create EVP_MD_CTX\n");
        return;
    }

    // Step 3: Simulate the hash extension
    // Re-initialize the digest context with the original hash state
    EVP_DigestInit_ex(ctx, md, NULL);
    EVP_DigestUpdate(ctx, padding, padding_length); // Process the padding
    EVP_DigestUpdate(ctx, append, strlen(append));  // Process the appended data

    // Step 4: Finalize the hash
    EVP_DigestFinal_ex(ctx, final_hash, &final_hash_length);
    EVP_MD_CTX_free(ctx);

    // Print the resulting hash
    printf("H(secret || data || append): ");
    print_hash(final_hash, final_hash_length);
}

int main() {
    // Known data (original message) and append
    const char *secret = "Hello, World!. This is a demonstration of message extension attacks. SE21UCSE192";
    const char *data = "My full name is Sanyam Agrawal.";
    const char *append = "SE21UCSE192";

    unsigned char original_hash[EVP_MAX_MD_SIZE];
    unsigned int original_hash_length;

    // Step 1: Compute H(secret || data)
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();

    if (!ctx) {
        fprintf(stderr, "Failed to create EVP_MD_CTX\n");
        return 1;
    }

    EVP_DigestInit_ex(ctx, md, NULL);
    EVP_DigestUpdate(ctx, secret, strlen(secret)); // Process the secret
    EVP_DigestUpdate(ctx, data, strlen(data));     // Process the data
    EVP_DigestFinal_ex(ctx, original_hash, &original_hash_length);
    EVP_MD_CTX_free(ctx);

    printf("Original hash (H(secret || data)): ");
    print_hash(original_hash, original_hash_length);

    // Step 2: Perform the length extension attack
    length_extension_attack(original_hash, strlen(secret) + strlen(data), append);

    return 0;
}