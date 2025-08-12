#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "colors.h"

extern ssize_t ft_read(int fd, void *buf, size_t count);

int main() {
    printf(CYAN "=== Testing ft_read ===" RESET "\n");
    
    // Test 1: Read from file
    printf("Test 1: Reading from file\n");
    
    // Create test file
    int fd_write = open("/tmp/test_ft_read.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    char *test_content = "Hello, test content!";
    int test_ok = 0;
    
    if (fd_write >= 0) {
        write(fd_write, test_content, strlen(test_content));
        close(fd_write);
        
        // Test reading
        int fd_read = open("/tmp/test_ft_read.txt", O_RDONLY);
        if (fd_read >= 0) {
            char buffer[50] = {0};
            ssize_t result1 = ft_read(fd_read, buffer, strlen(test_content));
            close(fd_read);
            
            printf("  Expected: \"%s\"\n", test_content);
            printf("  Got: \"%s\" (%zd bytes)", buffer, result1);
            
            test_ok = (result1 == (ssize_t)strlen(test_content) && strcmp(buffer, test_content) == 0);
        }
        unlink("/tmp/test_ft_read.txt");
    }
    
    test_ok ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    
    // Test 2: Read partial content
    printf("Test 2: Reading partial content\n");
    
    fd_write = open("/tmp/test_ft_read2.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    char *full_content = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int partial_ok = 0;
    
    if (fd_write >= 0) {
        write(fd_write, full_content, strlen(full_content));
        close(fd_write);
        
        int fd_read = open("/tmp/test_ft_read2.txt", O_RDONLY);
        if (fd_read >= 0) {
            char buffer[11] = {0};
            ssize_t result2 = ft_read(fd_read, buffer, 10);
            close(fd_read);
            
            printf("  Full content: \"%s\"\n", full_content);
            printf("  Read 10 chars: \"%s\" (%zd bytes)", buffer, result2);
            
            partial_ok = (result2 == 10 && strncmp(buffer, full_content, 10) == 0);
        }
        unlink("/tmp/test_ft_read2.txt");
    }
    
    partial_ok ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    
    // Test 3: Read zero bytes
    printf("Test 3: Reading zero bytes\n");
    fd_write = open("/tmp/test_ft_read3.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    int zero_ok = 0;
    
    if (fd_write >= 0) {
        write(fd_write, "test", 4);
        close(fd_write);
        
        int fd_read = open("/tmp/test_ft_read3.txt", O_RDONLY);
        if (fd_read >= 0) {
            char buffer[10];
            ssize_t result3 = ft_read(fd_read, buffer, 0);
            close(fd_read);
            
            printf("  Expected: 0 bytes, Got: %zd bytes", result3);
            zero_ok = (result3 == 0);
        }
        unlink("/tmp/test_ft_read3.txt");
    }
    
    zero_ok ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    
    // Test 4: Read from empty file (EOF)
    printf("Test 4: Reading from empty file\n");
    fd_write = open("/tmp/test_ft_read4.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    int eof_ok = 0;
    
    if (fd_write >= 0) {
        close(fd_write);  // Create empty file
        
        int fd_read = open("/tmp/test_ft_read4.txt", O_RDONLY);
        if (fd_read >= 0) {
            char buffer[10];
            ssize_t result4 = ft_read(fd_read, buffer, 10);
            close(fd_read);
            
            printf("  Expected: 0 (EOF), Got: %zd", result4);
            eof_ok = (result4 == 0);
        }
        unlink("/tmp/test_ft_read4.txt");
    }
    
    eof_ok ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    
    // Test 5: Invalid file descriptor (error case)
    printf("Test 5: Invalid file descriptor\n");
    errno = 0;
    char buffer[10];
    ssize_t result5 = ft_read(-1, buffer, 10);
    printf("  Expected: -1 with errno, Got: %zd (errno: %d)", result5, errno);
    
    (result5 == -1 && errno != 0) ? printf(GREEN " ✅ PASS\n\n" RESET) : printf(RED " ❌ FAIL\n\n" RESET);
    
    return 0;
}
