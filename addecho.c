/////////////////////// CONTRIBUTION BREAKDOWN ///////////////////////

// Saad Contributions - Lines (30 - 51 & 122 - 137)
// - Edge Case Testing (Throughout File)
// - Manual Testing (In Manual Tests folder but explained in video)
// - Program Structure 

// Zain Contributions - Lines (72 - 121)
// - Allocated memory for echoBuffer
// - created output file and processed audio samples
// - added echo after delay to output file
// - free memory

// Faisal Contributions - Lines (57 - 69 & 144 - 179)
// - Copying Header and Parsing Input 
// - Editing Video
// - Man Page


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <getopt.h>

#define HEADER_SIZE 22 // WAV file header size
#define MAX_16BIT 32767
#define MIN_16BIT -32768

// addEcho function adds the echo to the wav file
int addEcho(const char *inputFile, const char *outputFile, int delay, float volumeScale) {

/////////////////////// Start of Saad ///////////////////////
    FILE *input, *output;
    short header[HEADER_SIZE];
    short sample;
    int writeValue;
    int fileSize = 0, dataSize = 0;

    // open input and output files
    input = fopen(inputFile, "rb");
    if (!input) {
        perror("Error opening input file");
        return 1;
    }
    output = fopen(outputFile, "wb");
    if (!output) {
        if (fclose(input) != 0) {
            perror("Error closing output file");
        }
        return 1;
    }
    
/////////////////////// End of Saad ///////////////////////


/////////////////////// Start of Faisal ///////////////////////

    // read and copy header
  if (fread(header, sizeof(short), HEADER_SIZE, input) != HEADER_SIZE) {
        perror("Error reading header from input file");
        fclose(input);
        fclose(output);
        return 1;
    }

    memcpy(&fileSize, &header[2], sizeof(int)); 
    memcpy(&dataSize, &header[20], sizeof(int));
    fileSize += delay * sizeof(short); 
    dataSize += delay * sizeof(short);
    memcpy(&header[2], &fileSize, sizeof(int));
    memcpy(&header[20], &dataSize, sizeof(int));

    if (fwrite(header, sizeof(short), HEADER_SIZE, output) != HEADER_SIZE) {
        perror("Error writing modified header to output file");
        fclose(input);
        fclose(output);
        return 1;
    }

/////////////////////// End of Faisal ///////////////////////


/////////////////////// Start of Zain ///////////////////////

    // allocate the echo buffer using malloc
    short* echoBuffer = malloc(delay * sizeof(short));
    if (!echoBuffer) {
        perror("Failed to allocate echo buffer");
        if (fclose(input) != 0) {
            perror("Error closing input file");
        }
        if (fclose(input) != 0) {
            perror("Error closing output file");
        }
        return 1;
    }
    memset(echoBuffer, 0, delay * sizeof(short));

    // process audio samples
    int index = 0;
    while (fread(&sample, sizeof(sample), 1, input) == 1) {
        int echoSample = echoBuffer[index] / volumeScale;
        writeValue = sample + echoSample;
        fwrite(&writeValue, sizeof(short), 1, output);
        echoBuffer[index] = sample;
        index = (index + 1) % delay;
    }

    // write remaining echo to the file
    for (int i = 0; i < delay; i++) {
        int echoSample = echoBuffer[index] / volumeScale; 
        fwrite(&echoSample, sizeof(short), 1, output);
        index = (index + 1) % delay;
    }

    free(echoBuffer);


    if (fclose(input) != 0) {
        perror("Error closing input file");
        return 1;
    }

    if (fclose(output) != 0) {
        perror("Error closing output file");
        return 1;
    }

    return 0;
}

/////////////////////// End of Zain ///////////////////////



/////////////////////// Start of Saad ///////////////////////


int main(int argc, char *argv[]) {
    int delay = 8000; // default delay
    int volume = 4; // default volume scale
    int option;

    const char* shortOptions = "d:v:";
    const struct option longOptions[] = { // struct option is from getopt.h
        {"volume-scale", required_argument, NULL, 'v'},
        {"delay", required_argument, NULL, 'd'},
        {0, 0, 0, 0} // Terminate the array with a zero-filled element
    };

/////////////////////// End of Saad ///////////////////////


/////////////////////// Start of Faisal ///////////////////////

    while ((option = getopt_long(argc, argv, shortOptions, longOptions, NULL)) != -1) {
        switch (option) {
            case 'd': {
                char *endptr;
                long val = strtol(optarg, &endptr, 10);
                if (*endptr == '\0' && val > 0) { // check if entire string was consumed and value is non-negative (edge case)
                    delay = (int)val;
                } else {
                    fprintf(stderr, "Delay must be a non-negative integer\n");
                    return 2;
                }
                break;
            }
            case 'v': {
                char *endptr;
                long val = strtol(optarg, &endptr, 10);
                if (*endptr == '\0' && val > 0) { // check if entire string was consumed and value is non-negative (edge case)
                    volume = (int)val;
                } else {
                    fprintf(stderr, "Volume scale must be a non-negative integer\n");
                    return 2;
                }
                break;
            }
            case '?': // unrecognized option (edge case)
                fprintf(stderr, "Unrecognized option or missing option argument\n");
                fprintf(stderr, "Usage: %s [-d delay] [-v volume_scale] sourcewav destwav\n", argv[0]);
                return 2;
            default:
                fprintf(stderr, "Usage: %s [-d delay] [-v volume_scale] sourcewav destwav\n", argv[0]);
                return 2;
        }
    }
/////////////////////// End of Faisal ///////////////////////



/////////////////////// Start of Saad ///////////////////////

    // check if user provided input and output files, or provided too many files (edge case)
    if (argc < 2 || argc > 7  || argc - optind < 2) {
        fprintf(stderr, "Usage: %s [-d delay] [-v volume_scale] sourcewav destwav\n", argv[0]);
        return 2;
    }

    const char* inputFile = argv[argc - 2];
    const char* outputFile = argv[argc - 1];

    addEcho(inputFile, outputFile, delay, volume);
    return 0;
}

/////////////////////// End of Saad ///////////////////////