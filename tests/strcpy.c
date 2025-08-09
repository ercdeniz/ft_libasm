#include <stdio.h>
#include <string.h>
#include "colors.h"

extern char *ft_strcpy(char *dst, const char *src);

int main() {
    printf(CYAN "=== Testing ft_strcpy ===" RESET "\n");

    struct {
        const char *src;
        const char *description;
    } tests[] = {
        {"", "Empty string"},
        {"a", "Single character"},
        {"Hello", "Normal string"},
        {"Assembly", "Assembly word"},
        {"1234567890", "Numeric string"},
        {NULL, NULL}
    };

    for (int i = 0; tests[i].src != NULL; i++) {
        char dst[100] = {0};
        char expected[100] = {0};
        
        char *result = ft_strcpy(dst, tests[i].src);
        strcpy(expected, tests[i].src);
        
        printf("Test %d: %s\n", i + 1, tests[i].description);
        printf("  Src: \"%s\"\n", tests[i].src);
        printf("  Expected: \"%s\", Got: \"%s\"", expected, dst);

        (strcmp(dst, expected) == 0 && result == dst) ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    }

    return 0;
}
