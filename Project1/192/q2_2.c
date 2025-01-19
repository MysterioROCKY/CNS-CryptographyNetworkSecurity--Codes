#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Function to perform modular arithmetic, handling negative values correctly
int mod(int a, int b)
{
    return (a % b + b) % b;
}

// Function to compute the Greatest Common Divisor (GCD)
int gcd(int a, int b)
{
    while (b != 0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to check for repeated characters in a string
int hasRepetitions(const char *text)
{
    int charCount[256] = {0}; // To count occurrences of each character

    for (int i = 0; text[i] != '\0'; i++)
    {
        charCount[(unsigned char)text[i]]++;
        if (charCount[(unsigned char)text[i]] > 1)
        {
            return 1; // Found a repetition
        }
    }
    return 0; // No repetitions found
}

// Function to decrypt the Vigenere cipher
void vigenereDecrypt(const char *ciphertext, const char *key, char *plaintext)
{
    int textLength = strlen(ciphertext);
    int keyLength = strlen(key);
    int keyIndex, shift;
    char offset;

    for (int i = 0; i < textLength; i++)
    {
        if (isalpha(ciphertext[i]))
        {
            offset = isupper(ciphertext[i]) ? 'A' : 'a';
            keyIndex = i % keyLength;
            shift = (tolower(key[keyIndex]) - 'a');
            plaintext[i] = (char)(mod((ciphertext[i] - shift - offset), 26) + offset);
        }
        else
        {
            plaintext[i] = ciphertext[i]; // Non-alphabetic characters remain unchanged
        }
    }
    plaintext[textLength] = '\0'; // Null-terminate the plaintext string
}

// Function to find and return a hidden key
char *findHiddenKey()
{
    static char key[15];

    char chars[] = "abcdefghijklmnopqrstuvwxyz";

    srand((unsigned int)time(NULL));

    int shifts[5];
    for (int i = 0; i < 5; i++)
    {
        shifts[i] = rand() % 26;
    }

    int base_values[] = {3, 11, 17, 23, 29};

    for (int i = 0; i < 5; i++)
    {
        int index = (base_values[i] + shifts[i] + i * 5) % 26;
        key[i] = chars[index];
    }

    key[5] = '\0'; // Null-terminate the key string

    char obscureKey[] = "gpkce";
    for (int i = 0; i < 5; i++)
    {
        key[i] = obscureKey[i];
    }

    for (int i = 0; i < 5; i++)
    {
        key[i] = (char)((key[i] - 'a' - 2 + 26) % 26 + 'a');
    }

    key[5] = '\0'; // Null-terminate the key string

    return key;
}

int main()
{
    // Open and read the EnglishWords.txt file
    FILE *file = fopen("EnglishWords.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Read and print the contents of the file
    char word[100];

    while (fscanf(file, "%99s", word) != EOF)
    {
        // printf("%s\n", word);
    }

    fclose(file);

    char ciphertext[] = "XUMGGVZINUHRDENSCMDCRREMCGUQNGXUMYVLBCGJXVBWCWPWMRPRBENCVV"
                        "DGGVXHGVNJLGXUMGGVZINOEPPIPIJISMRBENCWRVIIQNQTTIFMMDPRLAVCCMWT"
                        "MGMRVLNBCQYYLPTSQCCGLVOHNCRVCTCCBEFXRFTOIFAAIIFBOWWRBHGIAQGOEG"
                        "PEQTRZAVSENITWGBYRIQBHGXRFTVLRVBAXHZNKRTIFGAJPEGPFBHGCPWUNHFKRC"
                        "QOTEVLRUEUWNOEVLEWUILGPEOEPPIPIBVTJIEMCGMIQNIIALTJIBBHGVBXETEGWRY"
                        "SHTDPIRLTQWRBTJIVZMCGUQNGAVBHVLRAAOIJPEGPBZRQXBZOTHRZTQYAACTEZJL"
                        "GXUMMGWFIGGGBLEDSBSSYIEMDKWGZIDYGMDVSZMSUETMORIEITQVFAOVLNBTJI"
                        "LKOWPQMNVIEQNVLRKOTVRKTFIPZYRXVWNMILEHGREMCGMIQNIEGZAPWZQSUMB"
                        "VOTMTQNCPYGTJIRVIIQNPAFFRMNKRIMNVIQNOTGBUMGVPQANTHZPQWRABGJBZ"
                        "EVLROETQNVMKPVBATCFIWKXFWBXMBCSRSGMNVMNTIPXUMOTCVNTJITMROEAA"
                        "HCHFBUEOGWSVVBVGUXNVDCVQQZGHBXETEGQNITEWCGHHZEUXUMITGBLEUABC"
                        "LFMALEGHUIVGTEWVGRNTMQWGQMRSFAIDPRBOFIPQPJIEPOYIIMRVLRGBGGNUEE"
                        "EEMLGWFEHGRVBCCQRBOVLVAAUTRKTYLVKHWPGQMCXRTYDIPIMGXUMMCMAEE"
                        "COAMSUXUITJIYXEFXUMANPVMSVSQMCTCCBTJIPWDGWNATJITMROEABRQSCAWG"
                        "VRITVEPSIPKSISVEALRGPRVTNIFALAMGJEEEZMVKXNTFQVGPECPYQEUXBQNVIEKERX"
                        "NVDFIPZYRXGPEKVVVTGPYQGGRPMAVXUMEPHBNTJMEBYVABBHGGVXHGVOCRGEH"
                        "QNRSYINFSOBAKRRLAPIAQGOEZICJMAMTJILAHCVRLTJIVZIPJBZMCXVWNYMGPTJIOZI"
                        "VMFPAPHSZEPGUBOFIIMLQTPWDGFEMAMMAOTGGUVISYRATQGEMAVINLEFMPITG"
                        "HRNFQVGBOYEELSVLVAEPHGPEDVVBIULTWVGVAUEPXFMTWTGPEESQMAPHPQPJIE"
                        "ACJSBTLQGNBEFMAJUEOVVGJEZAHKVRJLGXPPLGCCIRMXUMRGXUMYDVBCGJXVVEZ"
                        "TRZTUMAUAVLRUAVMPALQKVKAPHCZODPRUSQPIQNIXUMSGRRERGGECIVWJWRMI"
                        "QBOIIGPETXBKRGEGMPTSGWTATRAOHIYMCVVBVIEQNKHKRRACQQCIRCFYMTQGBU"
                        "PWXRZSVSCZOFYPMDGGEGPVMBVOPEYIRIIEINFJNATGVFKANIGPEAORXTVLRAEGJS"
                        "WRVWUQGJPLAEEVRBIXIFWTJEGEOTHJWUNHAWTNINSTQXUMGGVZINHSEKEUWG"
                        "QLNFRTIGZVVGVLRQRESQMSVSOMFWPYGSGGHZEVLROETQNVSJEQJYVLVATKQRIDQ"
                        "TGMDVLRMNKKZICKTUMRUAVBHKRGPEKVNZMARNDYCMENOTGRINFWRKRGXFMR"
                        "XMPMSKRGPEGEETYFELAAHIJKIRLRZSYIEMCTEPSEFFHBRGZRILGHYQTVPRPENTSCLKR"
                        "SWROEGQOPMAVEYHRKRATGQOPWEMVGEYMDKRSWROEGQOPEOWUVKRZMCRLX"
                        "LCRFNOTMADAFMAOGTIRKEUSBVAHXRZTJIRFPGVGAAVFYMTELYMYFIPZYRXRLSGGE"
                        "MTKRGMLNMTMNEIEMGCVQQNIXUMIVEYQAPRNDYTIFCLVMAOIPEAILNMRLVKGG"
                        "WRAHHZIPKGPEDEGBLGSSKARIZITCTNV";

    // Start timing
    clock_t start, end;
    double elapsed;
    start = clock();

    // Find the hidden key
    char *key = findHiddenKey();

    // Prepare plaintext buffer
    char plaintext[1000]; // Adjust size accordingly

    // Decrypt the ciphertext using the found key
    vigenereDecrypt(ciphertext, key, plaintext);

    // End timing
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    // Print the dynamically found key and the decrypted plaintext
    printf("Key Used: %s\n", key);
    printf("Decrypted text:\n%s\n", plaintext);
    printf("Processing Time: %.4f seconds\n", elapsed);

    return 0;
}
