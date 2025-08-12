#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "colors.h"

extern ssize_t ft_write(int fd, const void *buf, size_t count);

int main() {
    printf(BLUE "=== Testing ft_write ===\n" RESET);
    
    // Test 1: Write to stdout
    printf("Test 1: Writing to stdout\n");
    printf("  Expected output: ");
    char *msg1 = "Hello from ft_write!";
    ssize_t result1 = ft_write(1, msg1, strlen(msg1));
    printf("\n  Returned: %zd bytes", result1);
    
    (result1 == (ssize_t)strlen(msg1)) ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    
    // Test 2: Write to file
    printf("Test 2: Writing to file\n");
    int fd = open("/tmp/test_ft_write.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    ssize_t result2 = -1;
    int file_ok = 0;
    
    if (fd >= 0) {
        char *msg2 = "File content test";
        result2 = ft_write(fd, msg2, strlen(msg2));
        close(fd);
        
        // Verify file content
        FILE *f = fopen("/tmp/test_ft_write.txt", "r");
        if (f) {
            char buffer[100] = {0};
            fread(buffer, 1, sizeof(buffer), f);
            fclose(f);
            file_ok = (strcmp(buffer, msg2) == 0);
        }
        unlink("/tmp/test_ft_write.txt");
    }
    
    printf("  Expected: 17 bytes, Got: %zd bytes\n", result2);
    printf("  File content verification");
    
    (result2 == 17 && file_ok) ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    
    // Test 3: Write zero bytes
    printf("Test 3: Writing zero bytes\n");
    ssize_t result3 = ft_write(1, "test", 0);
    printf("  Expected: 0 bytes, Got: %zd bytes", result3);
    
    (result3 == 0) ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    
    // Test 4: Invalid file descriptor (error case)
    printf("Test 4: Invalid file descriptor\n");
    errno = 0;
    ssize_t result4 = ft_write(-1, "test", 4);
    printf("  Expected: -1 with errno, Got: %zd (errno: %d)", result4, errno);
    
    (result4 == -1 && errno != 0) ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    
    return 0;
}
