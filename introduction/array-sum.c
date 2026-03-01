#include <stdio.h>

int arr[] = {1, 2, 3, 4, 5, 6};
int total;

int main() {
  for (int i = 0; i < 5; i++) {
    total += arr[i];
  }
  printf("the total is %d \n", total);
  return 0;
}
