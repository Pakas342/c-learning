#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char dest[5];
  // in the man pages it says that for strcpy to work it needs the strlen of the
  // copied string +1, in this case, 5 it's exactly strlen. this happens because
  // in c string have a null terminator '\0' and. that marks the end of the
  // array, and is not counted by strlen
  strcpy(dest, "Hello");

  return 0;
}
