#include <stdio.h>
#include <string.h>
#include "colors.h"

extern int ft_strcmp(const char *s1, const char *s2);

int main() {
    printf(BLUE "=== Testing ft_strcmp ===\n" RESET);
    
    struct {
        char *s1;
        char *s2;
        char *description;
    } tests[] = {
        {"Hello", "Hello", "identical strings"},
        {"Hello", "World", "s1 < s2 (lexicographically)"},
        {"World", "Hello", "s1 > s2 (lexicographically)"},
        {"", "", "empty strings"},
        {"abc", "abd", "differ by one character"},
        {"test", "testing", "one string is prefix of another"},
        {"ABC", "abc", "case difference"},
        {"123", "124", "numeric strings"},
        {NULL, NULL, NULL}
    };

    for (int i = 0; tests[i].s1; i++) {
        int my_result = ft_strcmp(tests[i].s1, tests[i].s2);
        int real_result = strcmp(tests[i].s1, tests[i].s2);
        
        int my_norm = (my_result > 0) ? 1 : (my_result < 0) ? -1 : 0;
        int real_norm = (real_result > 0) ? 1 : (real_result < 0) ? -1 : 0;
        
        printf("Test %d: %s\n", i + 1, tests[i].description);
        printf("  Strings: \"%s\" vs \"%s\"\n", tests[i].s1, tests[i].s2);
        printf("  ft_strcmp: %d, strcmp: %d ", my_norm, real_norm);

        (my_norm == real_norm) ? printf(GREEN "✓ PASS\n\n" RESET) : printf(RED "✗ FAIL\n\n" RESET);
    }
}
