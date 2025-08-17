#include <stdio.h>

/* print Farenheit-Celsius Table
 * for fahr - 0, 20, ..., 300
 * A new iteration based o the use of flotas it's added*/
int main() {
  float fahr, celsius;
  int lower, upper, step;

  lower = 0;   /* lower limit of the temperature table */
  upper = 300; /* upper limit of the temperature table */
  step = 20;   /* steps thaking by the table */

  fahr = lower;
  while (fahr <= upper) {
    /* doing integer division will fail as the decimal part will be lost,
     * so we need to use float division */
    /* before we did 5/9 * (fahr - 32), but that will return 0 everytime as 5/9
     * will lost decimal part */
    /* on Integer float operations the compiler will convert the integer to a
     * float with .0, but it's better to explicitly state the float as it's more
     * readable */
    celsius = 5.0 * (fahr - 32.0) / 9.0;
    /* adding 6.1f means a fractional number with 1 decimal point and 6 char
     * spaces, if you want, you could set 3d, which means decimal integer with 3
     * spaces. You can not set the width and just do %.1f or %d for ex*/
    printf("%3.0f%6.1f\n", fahr, celsius);
    fahr += step;
  }
}
