#include <stdio.h>

int arr[] = {1, 2, 3, 4, 5, 6};
int total;
int arrSize =  sizeof(arr) / sizeof(arr[0]);

int main() {
  for (int i = 0; i < 5; i++) {
    total += arr[i];
  }
  printf("the total is %d \n", total);
  float avrg = (float)total / (float)arrSize;
  printf("The avrg is %.2f \n", avrg);
  return 0;
}
