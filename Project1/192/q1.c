#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

// Function declarations
void caesarEncrypt(char *text, int shift);
void caesarDecrypt(char *text, int shift);
void vigenereEncrypt(char *text, const char *key);
void vigenereDecrypt(char *text, const char *key);
void playfairEncrypt(char *text, const char *key);
void playfairDecrypt(char *text, const char *key);
void generatePlayfairMatrix(const char *key, char matrix[SIZE][SIZE]);
void printPlayfairMatrix(const char matrix[SIZE][SIZE]);
void prepareText(char *text);
void splitTextIntoDigraphs(char *text, char digraphs[][3], int *numDigraphs);

// Main Function
int main() {
    char choice;
    char key[100];
    char text[1000];

    // Read text from the file "process.txt"
    FILE *file = fopen("process.txt", "r");
    if (!file) {
        printf("Error opening process.txt!\n");
        return 1;
    }
    fgets(text, sizeof(text), file);
    fclose(file);

    printf("Select encryption method (C for Caesar, V for Vigenere, P for Playfair): ");
    scanf(" %c", &choice);

    printf("Encrypt or Decrypt? (E/D): ");
    char operation;
    scanf(" %c", &operation);

    // Processing based on the chosen cipher
    if (choice == 'C' || choice == 'c') {
        int shift;
        printf("Enter shift value for Caesar Cipher: ");
        scanf("%d", &shift);

        if (operation == 'E' || operation == 'e') {
            caesarEncrypt(text, shift);
            printf("Encrypted Text: %s\n", text);
        } else if (operation == 'D' || operation == 'd') {
            caesarDecrypt(text, shift);
            printf("Decrypted Text: %s\n", text);
        }
    } else if (choice == 'V' || choice == 'v') {
        printf("Enter key for Vigenère Cipher (max length 100): ");
        scanf("%s", key);

        if (operation == 'E' || operation == 'e') {
            vigenereEncrypt(text, key);
            printf("Encrypted Text: %s\n", text);
        } else if (operation == 'D' || operation == 'd') {
            vigenereDecrypt(text, key);
            printf("Decrypted Text: %s\n", text);
        }
    } else if (choice == 'P' || choice == 'p') {
        printf("Enter key for Playfair Cipher (max length 100): ");
        scanf("%s", key);

        if (operation == 'E' || operation == 'e') {
            playfairEncrypt(text, key);
            printf("Encrypted Text: %s\n", text);
        } else if (operation == 'D' || operation == 'd') {
            playfairDecrypt(text, key);
            printf("Decrypted Text: %s\n", text);
        }
    } else {
        printf("Invalid method selected.\n");
    }

    return 0;
}

// Function to preprocess text for Playfair
void prepareText(char *text) {
    char temp[1000]; // Temporary array to hold the processed text
    int j = 0;

    for (int i = 0; text[i]; i++) {
        if (text[i] != ' ') { // Remove spaces
            temp[j++] = tolower(text[i]);
        }
    }
    
    temp[j] = '\0'; // Null-terminate the temporary string

    // Handle repeated pairs and odd length
    j = 0;
    for (int i = 0; temp[i]; i += 2) {
        if (temp[i] == temp[i + 1]) { // If two letters are the same
            printf("Adding filler 'x' for repeated letters: %c%c\n", temp[i], temp[i]); // Print message
            temp[i + 1] = 'x'; // Replace with 'x'
        }
        temp[j++] = temp[i]; // Add first letter
        if (temp[i + 1]) {
            temp[j++] = temp[i + 1]; // Add second letter if exists
        }
    }
    
    if (j % 2 != 0) { // Check if the length is odd
        printf("Adding filler 'x' to make even length.\n");
        temp[j++] = 'x'; // Append 'x' to make even length
    }

    temp[j] = '\0';
    strcpy(text, temp); // Copy the modified text back
}

// Function to generate Playfair matrix
void generatePlayfairMatrix(const char *key, char matrix[SIZE][SIZE]) {
    int letterExists[26] = {0}; // Array to track which letters are in the matrix
    int k = 0;

    // First, add unique letters of the key to the matrix
    for (int i = 0; key[i] != '\0'; i++) {
        char currentChar = tolower(key[i]);
        // Ignore 'j' and consider it as 'i'
        if (currentChar == 'j') {
            currentChar = 'i';
        }
        if (!letterExists[currentChar - 'a']) {
            letterExists[currentChar - 'a'] = 1; // Mark letter as exists
            matrix[k / SIZE][k % SIZE] = currentChar; // Place in matrix
            k++;
        }
    }

    // Fill the matrix with remaining letters
    for (char ch = 'a'; ch <= 'z'; ch++) {
        if (ch == 'j') continue; // Skip 'j'
        if (!letterExists[ch - 'a']) {
            matrix[k / SIZE][k % SIZE] = ch;
            k++;
        }
    }
}

// Function to print Playfair matrix
void printPlayfairMatrix(const char matrix[SIZE][SIZE]) {
    printf("Playfair Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to encrypt using Playfair Cipher
void playfairEncrypt(char *text, const char *key) {
    char matrix[SIZE][SIZE];
    generatePlayfairMatrix(key, matrix);

    printf("\nPlayfair Matrix for Encryption:\n");
    printPlayfairMatrix(matrix);

    prepareText(text);

    char digraphs[100][3]; // To hold digraphs
    int numDigraphs = 0;
    splitTextIntoDigraphs(text, digraphs, &numDigraphs);

    // Encrypting digraphs
    for (int i = 0; i < numDigraphs; i++) {
        char first = digraphs[i][0];
        char second = digraphs[i][1];

        int row1, col1, row2, col2;

        // Find positions of first letter
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                if (matrix[r][c] == first) {
                    row1 = r;
                    col1 = c;
                }
                if (matrix[r][c] == second) {
                    row2 = r;
                    col2 = c;
                }
            }
        }

        if (row1 == row2) {
            // Same row, shift right
            text[i * 2] = matrix[row1][(col1 + 1) % SIZE]; // Wrap around
            text[i * 2 + 1] = matrix[row2][(col2 + 1) % SIZE]; // Wrap around
        } else if (col1 == col2) {
            // Same column, shift down
            text[i * 2] = matrix[(row1 + 1) % SIZE][col1]; // Wrap around
            text[i * 2 + 1] = matrix[(row2 + 1) % SIZE][col2]; // Wrap around
        } else {
            // Form a rectangle
            text[i * 2] = matrix[row1][col2];
            text[i * 2 + 1] = matrix[row2][col1];
        }
    }
    text[numDigraphs * 2] = '\0'; // Null-terminate the transformed text
}

// Function to decrypt using Playfair Cipher
void playfairDecrypt(char *text, const char *key) {
    char matrix[SIZE][SIZE];
    generatePlayfairMatrix(key, matrix);

    printf("\nPlayfair Matrix for Decryption:\n");
    printPlayfairMatrix(matrix);

    prepareText(text);

    char digraphs[100][3]; // To hold digraphs
    int numDigraphs = 0;
    splitTextIntoDigraphs(text, digraphs, &numDigraphs);

    // Decrypting digraphs
    for (int i = 0; i < numDigraphs; i++) {
        char first = digraphs[i][0];
        char second = digraphs[i][1];

        int row1, col1, row2, col2;

        // Find positions of first letter
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                if (matrix[r][c] == first) {
                    row1 = r;
                    col1 = c;
                }
                if (matrix[r][c] == second) {
                    row2 = r;
                    col2 = c;
                }
            }
        }

        if (row1 == row2) {
            // Same row, shift left
            text[i * 2] = matrix[row1][(col1 - 1 + SIZE) % SIZE]; // Wrap around
            text[i * 2 + 1] = matrix[row2][(col2 - 1 + SIZE) % SIZE]; // Wrap around
        } else if (col1 == col2) {
            // Same column, shift up
            text[i * 2] = matrix[(row1 - 1 + SIZE) % SIZE][col1]; // Wrap around
            text[i * 2 + 1] = matrix[(row2 - 1 + SIZE) % SIZE][col2]; // Wrap around
        } else {
            // Form a rectangle
            text[i * 2] = matrix[row1][col2];
            text[i * 2 + 1] = matrix[row2][col1];
        }
    }
    text[numDigraphs * 2] = '\0'; // Null-terminate the transformed text
}

// Function to split the text into digraphs
void splitTextIntoDigraphs(char *text, char digraphs[][3], int *numDigraphs) {
    int i = 0, j = 0;
    
    while (text[i]) {
        digraphs[j][0] = text[i];

        // Check for the next letter and handle repetitions
        if (text[i + 1] == text[i]) {
            printf("Adding filler 'x' for repeated letters: %c%c\n", text[i], text[i]); // Print message
            digraphs[j][1] = 'x'; // Replace with 'x'
        } else {
            digraphs[j][1] = (text[i + 1]) ? text[i + 1] : 'x'; // If odd, pad with 'x'
        }
        
        digraphs[j][2] = '\0'; // Null-terminate the digraph
        i += 2; // Move to next pair
        j++;
    }

    *numDigraphs = j; // Update number of digraphs
}

// Function to encrypt using Caesar Cipher
void caesarEncrypt(char *text, int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = islower(text[i]) ? 'a' : 'A';
            text[i] = (text[i] - base + shift) % 26 + base;
        }
    }
}

// Function to decrypt using Caesar Cipher
void caesarDecrypt(char *text, int shift) {
    caesarEncrypt(text, 26 - (shift % 26)); // Decrypting by shifting backwards
}

// Function to encrypt using Vigenère Cipher
void vigenereEncrypt(char *text, const char *key) {
    int keyLen = strlen(key);
    for (int i = 0, j = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = islower(text[i]) ? 'a' : 'A';
            text[i] = (text[i] - base + (tolower(key[j % keyLen]) - 'a')) % 26 + base;
            j++;
        }
    }
}

// Function to decrypt using Vigenère Cipher
void vigenereDecrypt(char *text, const char *key) {
    int keyLen = strlen(key);
    for (int i = 0, j = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = islower(text[i]) ? 'a' : 'A';
            text[i] = (text[i] - base - (tolower(key[j % keyLen]) - 'a') + 26) % 26 + base;
            j++;
        }
    }
}
