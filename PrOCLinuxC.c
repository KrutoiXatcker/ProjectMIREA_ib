#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>    
#include <unistd.h>    
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Function to print a string from a file
bool print_str(FILE* fp) {
    char buffer[81];
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
        return true;
    }
    else {
        return false;
    }
}

int main(int argc, char* argv[], char* envp[]) {
    // Check for too many arguments
    if (argc > 5) {
        printf("error\n");
        return 1;
    }

    // Handle -w option
    if (strcmp(argv[1], "-w") == 0) {
        printf("Processing -w option\n");

        FILE* fr = fopen(argv[2], "r");
        FILE* fw = fopen(argv[3], "w");
        if (fr == NULL || fw == NULL) {
            printf("Error opening file\n");
            return 1;
        }

        // Copy contents from one file to another
        int ch;
        while ((ch = fgetc(fr)) != EOF) {
            fputc(ch, fw);
        }

        fclose(fr);
        fclose(fw);
        printf("File copied successfully\n");
    }

    // Handle -s option
    if (strcmp(argv[1], "-s") == 0) {
        if (argc < 3) {
            printf("No file specified\n");
            return 1;
        }
        else {
            FILE* fp = fopen(argv[2], "r");
            if (!fp) {
                printf("Failed to open file.\n");
                exit(1);
            }

            // Read and print in blocks
            int N = strtol(argv[3], NULL, 10);
            int line_count = 1;

            while (print_str(fp)) {
                if (line_count == N && N != 0) {
                    char BUF;
                    scanf("%c", &BUF); // Wait for user input
                    line_count = 1;     // Reset line counter
                }
                else {
                    ++line_count;
                }
            }
            fclose(fp);
        }
    }

    // Handle -WR option
    if (strcmp(argv[1], "-WR") == 0) {
        FILE* fp = fopen(argv[2], "w");
        if (fp == NULL) {
            perror("Error opening file");
            return 1;
        }
        else {
            while (1) {
                printf("> ");
                char* bufstr = (char*)malloc(65 * sizeof(char)); // Allocate memory for input
                if (bufstr == NULL) {
                    perror("Memory allocation error");
                    return 1;
                }

                if (fgets(bufstr, 65, stdin) != NULL) {
                    bufstr[strcspn(bufstr, "\n")] = 0; // Remove newline character

                    // Check for exit command
                    if (strcmp(bufstr, "XE") == 0) {
                        free(bufstr);
                        break;
                    }

                    fprintf(fp, "%s\n", bufstr); // Write to file
                    free(bufstr);
                }
                else {
                    perror("Input error");
                    free(bufstr);
                    return 1;
                }
            }
        }
        fclose(fp);
    }

    return 0;
}
