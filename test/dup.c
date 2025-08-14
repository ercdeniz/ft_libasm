#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "colors.h"

extern char *ft_strdup(const char *str);

int main() {
    printf(MAGENTA "=== Testing ft_strdup ===" RESET "\n");
    
    struct {
        const char *input;
        const char *description;
    } tests[] = {
        {"Hello, World!", "Normal string duplication"},
        {"", "Empty string duplication"},
        {"This is a very long string that tests whether ft_strdup can handle longer strings correctly!", "Long string duplication"},
        {"Special chars: !@#$%^&*()", "String with special characters"},
        {"1234567890", "Numeric string"},
        {NULL, NULL}
    };
    
    for (int i = 0; tests[i].input != NULL; i++) {
        char *my_dup = ft_strdup(tests[i].input);
        char *real_dup = strdup(tests[i].input);
        
        printf("Test %d: %s\n", i + 1, tests[i].description);
        printf("  Original: \"%s\"\n", tests[i].input);
        printf("  strdup: \"%s\"\n", real_dup ? real_dup : "NULL");
        printf("  ft_strdup: \"%s\"", my_dup ? my_dup : "NULL");
        
        int test_ok = 0;
        if (my_dup && real_dup) {
            test_ok = (strcmp(my_dup, real_dup) == 0 && 
                      strcmp(my_dup, tests[i].input) == 0 && 
                      my_dup != tests[i].input);
        }
        
        test_ok ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
        
        free(my_dup);
        free(real_dup);
    }
    
    printf("Test 6: NULL input handling\n");
    errno = 0;
    char *null_result = ft_strdup(NULL);
    printf("  Input: NULL\n");
    printf("  Expected: NULL with errno, Got: %s (errno: %d)", null_result ? "non-NULL" : "NULL", errno);
    
    (null_result == NULL && errno != 0) ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    
    if (null_result)
        free(null_result);
    
    return 0;
}
