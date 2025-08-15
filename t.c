#include <malloc.h>
#include <stdio.h>

int freadline(FILE* stream, char** out) {
    if (stream == NULL) return -1;
    if (out == NULL) return -2;
    
    size_t capacity = 16; 
    size_t length = 0;
    char* buffer = calloc(capacity, sizeof(char));
    if (buffer == NULL) {
        return -3;
    }

    int c;
    while ((c = fgetc(stream)) != EOF && c != '\n' && !ferror(stream)) {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char* temp = realloc(buffer, capacity * sizeof(char));
            if (temp == NULL) {
                free(buffer);
                return -3;
            }
            buffer = temp;
        }
        
        buffer[length++] = (char)c;
    }
    if (ferror(stream)) {
        free(buffer);
        return -4;
    }
    if (length == 0 && c == EOF) {
        char* temp = realloc(buffer, 1 * sizeof(char));
        if (temp == NULL) {
            free(buffer);
            return -3;
        }
        buffer = temp;
        buffer[0] = '\0';
        *out = buffer;
        return 0;
    }

    if (length == capacity) {
        capacity *= 2;
        char* temp = realloc(buffer, (capacity + 1) * sizeof(char));
        if (temp == NULL) {
            free(buffer);
            return -3;
        }
        buffer = temp;
        buffer[length] = '\0';
    }

    *out = buffer;
    return 0;
}

int main() {
    char *s = NULL;
    printf("Code %d", freadline(stdin, &s));
    printf("String: %s", s);
}
