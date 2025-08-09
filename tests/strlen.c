#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "colors.h"

extern size_t ft_strlen(const char *str);

int main()
{
    printf(CYAN "=== Testing ft_strlen ===" RESET "\n");

    struct
    {
        const char *input;
        size_t expected;
        const char *description;
    } tests[] = {
        {"", 0, "Empty string"},
        {"a", 1, "Single character"},
        {"Hello", 5, "Normal string"},
        {"Assembly programming", 18, "String with spaces"},
        {"1234567890", 10, "Numeric string"},
        {"Special chars: !@#$%", 20, "Special characters"},
        {NULL, 0, NULL}};

    for (int i = 0; tests[i].input != NULL; i++)
    {
        size_t result = ft_strlen(tests[i].input);
        size_t expected = strlen(tests[i].input);

        printf("Test %d: %s\n", i + 1, tests[i].description);
        printf("  Input: \"%s\"\n", tests[i].input);
        printf("  Expected: %zu, Got: %zu", expected, result);

        (result == expected) ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    }

    return 0;
}
