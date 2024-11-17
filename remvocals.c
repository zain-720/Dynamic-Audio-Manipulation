#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Function to remove vocals from the WAV file
void removeVocals(const char *inputFile, const char *outputFile) {
    FILE *input, *output;
    char header[44];
    int16_t left, right, combined;

    // Open input file
    input = fopen(inputFile, "rb");
    if (!input) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // Open output file
    output = fopen(outputFile, "wb");
    if (!output) {
        perror("Error opening output file");
        fclose(input); // Close input file if output file open fails
        exit(EXIT_FAILURE);
    }

    // Copy the header from input to output
    fread(header, sizeof(char), 44, input);
    fwrite(header, sizeof(char), 44, output);

    // Process the rest of the file
    while (fread(&left, sizeof(int16_t), 1, input) == 1 && fread(&right, sizeof(int16_t), 1, input) == 1) {
        combined = (left - right) / 2;
        fwrite(&combined, sizeof(int16_t), 1, output);
        fwrite(&combined, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}

int main(int argc, char *argv[]) {
    // Ensure correct command line usage
    if (argc != 3) {
        fprintf(stderr, "Usage: %s sourcewav destwav\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Call the vocal removal function
    removeVocals(argv[1], argv[2]);

    return 0;
}
