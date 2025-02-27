#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

void construct_dest_path(char *dest, const char *dir, const char *file) {
    int len = strlen(dir);
    memmove(dest, dir, len);
    
    if (dir[len - 1] != '/') {  // Add '/' if not present
        dest[len] = '/';
        len++;
    }
    
    memmove(dest + len, file, strlen(file) + 1); // Copy filename
}

int is_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) < 0)
        return 0;
    return st.type == T_DIR;
}

int main(int argc, char *argv[]) {
    char new_path[128];

    if (argc != 3) {
        printf(2, "Usage: mv <source> <destination>\n");
        exit();
    }

    // If destination is a directory, append the filename to it
    if (is_directory(argv[2])) {
        char *filename = argv[1];
        for (char *p = argv[1]; *p; p++)
            if (*p == '/') filename = p + 1;  // Get last part of path

        construct_dest_path(new_path, argv[2], filename);
    } else {
        memmove(new_path, argv[2], strlen(argv[2]) + 1);
    }

    // Try moving with link() + unlink()
    if (link(argv[1], new_path) == 0) {
        unlink(argv[1]);
        exit();
    }

    // If link() fails, fallback to copy + remove
    printf(2, "mv: cross-filesystem move, using copy + remove\n");

    int src_fd, dest_fd, n;
    char buf[512];

    if ((src_fd = open(argv[1], O_RDONLY)) < 0) {
        printf(2, "mv: cannot open %s\n", argv[1]);
        exit();
    }

    if ((dest_fd = open(new_path, O_WRONLY | O_CREATE)) < 0) {
        printf(2, "mv: cannot create %s\n", new_path);
        close(src_fd);
        exit();
    }

    while ((n = read(src_fd, buf, sizeof(buf))) > 0) {
        if (write(dest_fd, buf, n) != n) {
            printf(2, "mv: write error\n");
            close(src_fd);
            close(dest_fd);
            exit();
        }
    }

    close(src_fd);
    close(dest_fd);
    unlink(argv[1]);  // Delete original file after copying

    exit();
}
