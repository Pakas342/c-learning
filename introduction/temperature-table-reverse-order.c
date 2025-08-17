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

  for (fahr = upper; fahr >= lower; fahr -= step) {
    celsius = 5.0 * (fahr - 32.0) / 9.0;
    printf("%6.0f%6.1f\n", fahr, celsius);
  }
}
