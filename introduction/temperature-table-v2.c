#include <stdio.h>

int main() {
  int upper, lower, step;
  float far, cel;

  lower = 0;
  upper = 300;
  step = 20;

  far = lower;

  while (far <= upper) {
    /* I need to do this instead of 5/9 because 5/9 is an integer division,
     * which means it will truncate to 0, causing all the thing to return 0 */
    cel = 5 * (far - 32) / 9;

    /* this is the classic version of the print, which only says, print a float,
     * tab, print a another float printf("%f\t%f\n", far, cel);
     *
     * We can make it better
     * This one stablishes the space the output will take
     * printf("%3f\t%6f\n", far, cel); */

    /* and this version, stablish the amount of decimal points */
    printf("%3.0f\t%6.2f\n", far, cel);

    far += step;
  }
}
