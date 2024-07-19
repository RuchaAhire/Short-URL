#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_URL_LENGTH 2048
#define SHORT_URL_LENGTH 6
#define DATABASE_FILE "url_mapping.txt"

typedef struct {
    char long_url[MAX_URL_LENGTH];
    char short_url[SHORT_URL_LENGTH + 1];
} URLMapping;

// Function to generate a random short URL
void generate_short_url(char *short_url) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < SHORT_URL_LENGTH; ++i) {
        int key = rand() % (sizeof(charset) - 1);
        short_url[i] = charset[key];
    }
    short_url[SHORT_URL_LENGTH] = '\0';
}

// Function to save the URL mapping to a file
void save_mapping(const char *long_url, const char *short_url) {
    FILE *file = fopen(DATABASE_FILE, "a");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s %s\n", short_url, long_url);
    fclose(file);
}

// Function to retrieve the long URL from a short URL
int retrieve_long_url(const char *short_url, char *long_url) {
    FILE *file = fopen(DATABASE_FILE, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return 0;
    }

    char stored_short_url[SHORT_URL_LENGTH + 1];
    char stored_long_url[MAX_URL_LENGTH];

    while (fscanf(file, "%s %s", stored_short_url, stored_long_url) != EOF) {
        if (strcmp(stored_short_url, short_url) == 0) {
            strcpy(long_url, stored_long_url);
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int main() {
    srand(time(NULL));

    char long_url[MAX_URL_LENGTH];
    char short_url[SHORT_URL_LENGTH + 1];
    char retrieved_url[MAX_URL_LENGTH];

    int choice;
    printf("URL Shortener\n");
    printf("1. Shorten URL\n");
    printf("2. Retrieve URL\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); // To consume newline after entering choice

    switch (choice) {
        case 1:
            printf("Enter the long URL: ");
            fgets(long_url, MAX_URL_LENGTH, stdin);
            long_url[strcspn(long_url, "\n")] = '\0'; // Remove newline character

            generate_short_url(short_url);
            save_mapping(long_url, short_url);

            printf("Short URL: %s\n", short_url);
            break;
        
        case 2:
            printf("Enter the short URL: ");
            fgets(short_url, SHORT_URL_LENGTH + 1, stdin);
            short_url[strcspn(short_url, "\n")] = '\0'; // Remove newline character

            if (retrieve_long_url(short_url, retrieved_url)) {
                printf("Long URL: %s\n", retrieved_url);
            } else {
                printf("No mapping found for short URL: %s\n", short_url);
            }
            break;

        default:
            printf("Invalid choice\n");
            break;
    }

    return 0;
}
