#include "types.h"
#include "user.h"

int
main(void) {
  int free_memory = get_free_memory();  // Call the system call to get free memory
  printf(1, "Free memory: %d bytes\n", free_memory);
  exit();
}