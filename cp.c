#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUF_SIZE 512 

int main(int argc, char *argv[]) {
    int src_fd, dest_fd, bytes_read;
    char buffer[BUF_SIZE];

    if (argc != 3) {
        printf(2, "Usage: cp <source> <destination>\n");
        exit();
    }

    
    if ((src_fd = open(argv[1], O_RDONLY)) < 0) {
        printf(2, "cp: cannot open %s\n", argv[1]);
        exit();
    }

    // Open or create destination file 
    if ((dest_fd = open(argv[2], O_WRONLY | O_CREATE)) < 0) {
        printf(2, "cp: cannot create %s\n", argv[2]);
        close(src_fd);
        exit();
    }

    // Copy content
    while ((bytes_read = read(src_fd, buffer, BUF_SIZE)) > 0) {
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            printf(2, "cp: error writing to %s\n", argv[2]);
            close(src_fd);
            close(dest_fd);
            exit();
        }
    }

    close(src_fd);
    close(dest_fd);
    exit();
}
