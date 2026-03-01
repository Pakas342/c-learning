#include <stdio.h>

int main() {
  while (1) {
    /* done to clean the garbage in our stack memory space */
    char userInput[100] = {'\0'};

    if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
      break;
    } else {
      printf("%s", userInput);
    };
  }
}
