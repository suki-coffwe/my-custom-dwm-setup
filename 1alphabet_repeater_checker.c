#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main(void) {
    DIR *dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    int counts[256] = {0};
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        // Skip current directory, parent directory, and hidden files
        if (entry->d_name[0] == '.') {
            continue;
        }
        unsigned char first_char = (unsigned char)entry->d_name[0];
        counts[first_char]++;
    }
    closedir(dir);

    int found = 0;
    for (int i = 0; i < 256; i++) {
        if (counts[i] > 1) {
            if (!found) {
                printf("Repeated files starter(s): ");
                found = 1;
            } else {
                printf(", ");
            }
            printf("%c", (char)i);
        }
    }

    if (!found) {
        printf("NULL");
    }
    printf("\n");

    return EXIT_SUCCESS;
}
