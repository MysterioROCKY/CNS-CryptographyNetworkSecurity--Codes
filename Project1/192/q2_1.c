#include <stdio.h>
#include <string.h>
#include <time.h>

#define ALPHABET_SIZE 26

// Frequency of letters in the English language
double englishFrequencies[ALPHABET_SIZE] = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 0.06094, 0.06996, 
    0.00153, 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987, 
    0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 0.00074
};

// Function to decrypt the ciphertext using the given key
void decrypt(char *ciphertext, char *plaintext, int key) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            plaintext[i] = ((ciphertext[i] - 'a' - key + ALPHABET_SIZE) % ALPHABET_SIZE) + 'a';
        } else if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = ((ciphertext[i] - 'A' - key + ALPHABET_SIZE) % ALPHABET_SIZE) + 'A';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[len] = '\0';
}

// Function to compute the chi-squared statistic for a given key
double computeChiSquared(char *ciphertext, int key) {
    int len = strlen(ciphertext);
    int letterCounts[ALPHABET_SIZE] = {0};
    double chiSquared = 0.0;
    int letterTotal = 0;
    
    // Decrypt and count letter frequencies
    for (int i = 0; i < len; i++) {
        if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            int decryptedChar = ((ciphertext[i] - 'a' - key + ALPHABET_SIZE) % ALPHABET_SIZE) + 'a';
            letterCounts[decryptedChar - 'a']++;
            letterTotal++;
        } else if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            int decryptedChar = ((ciphertext[i] - 'A' - key + ALPHABET_SIZE) % ALPHABET_SIZE) + 'A';
            letterCounts[decryptedChar - 'A']++;
            letterTotal++;
        }
    }

    // Calculate the chi-squared statistic without using pow
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double observed = letterCounts[i];
        double expected = englishFrequencies[i] * letterTotal;
        if (expected > 0) {
            double difference = observed - expected;
            chiSquared += (difference * difference) / expected;
        }
    }

    return chiSquared;
}

// Function to perform the frequency analysis attack
void frequencyAnalysisAttack(char *ciphertext) {
    char plaintext[strlen(ciphertext) + 1];
    int bestKey = 0;
    double minChiSquared = __DBL_MAX__;

    for (int key = 0; key < ALPHABET_SIZE; key++) {
        double chiSquared = computeChiSquared(ciphertext, key);
        if (chiSquared < minChiSquared) {
            minChiSquared = chiSquared;
            bestKey = key;
        }
    }

    decrypt(ciphertext, plaintext, bestKey);

    printf("Decrypted Plaintext: %s\n", plaintext);
    printf("Key Used: %d\n", bestKey);
}

int main() {
    // Use lowercase letters to ensure proper handling
    char ciphertext[] = "MUYDJUDTJERUWYDEDJXUVYHIJEVVURHKQHOKDHUIJHYSJUTIKRCQHYDUMQHVQH"
"UMUIXQBBUDTUQLEHYDIFYJUEVJXYIJEAUUFJXUKDYJUTIJQJUIEVQCUHYSQDUKJHQB"
"YDJXUULUDJEVJXYIDEJIKSSUUTYDWMUCQAUCUNYSEQFHEFEIQBEVQBBYQDSUEDJX"
"UVEBBEMYDWRQIYICQAUMQHJEWUJXUHCQAUFUQSUJEWUJXUHWUDUHEKIVYDQ"
"DSYQBIKFFEHJQDTQDKDTUHIJQDTYDWEDEKHFQHJJXQJCUNYSEYIJEHUSEDGKUHJXU"
"BEIJJUHHYJEHOYDJUNQIDUMCUNYSEQDTQHYPEDQJXUIUJJBUCUDJYDTUJQYBYIBUVJ"
"JEOEKOEKMYBBYDVEHCJXUFHUIYTUDJEVJXUQRELUCEIJIUSHUJBOQIIEEDQIJXUEKJRH"
"UQAEVMQHMYJXJXUKDYJUTIJQJUIEVQCUHYSQYISUHJQYDQDTQTTJXUIKWWUIJYEDJ"
"XQJXUIXEKBTEDXYIEMDYDYJYQJYLUYDLYJUZQFQDJEYCCUTYQJUQTXUHUDSUQDTQJJ"
"XUIQCUJYCUCUTYQJURUJMUUDZQFQDQDTEKHIUBLUIFBUQIUSQBBJXUFHUIYTUDJIQ"
"JJUDJYEDJEJXUVQSJJXQJJXUHKJXBUIIUCFBEOCUDJEVEKHIKRCQHYDUIDEMEVVUHIJX"
"UFHEIFUSJEVSECFUBBYDWUDWBQDTYDQVUMCEDJXIJECQAUFUQSU";
    
    clock_t start = clock();
    frequencyAnalysisAttack(ciphertext);
    clock_t end = clock();

    double timeTaken = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Processing Time: %f seconds\n", timeTaken);

    return 0;
}



