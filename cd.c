#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(2, "usage: cd <directory>\n");
        exit();
    }

    if (chdir(argv[1]) < 0) {
        printf(2, "cd: cannot change directory to %s\n", argv[1]);
        exit();
    }

    exit();
}
