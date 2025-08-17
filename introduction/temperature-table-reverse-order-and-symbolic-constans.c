#include <stdio.h>

#define LOWER 0   /* lower limit of the temperature table */
#define UPPER 300 /* upper limit of the temperature table */
#define STEP 20   /* steps thaking by the table */

/* print Farenheit-Celsius Table
 * for fahr - 0, 20, ..., 300
 * A new iteration based o the use of flotas it's added*/
int main() {
  float fahr, celsius;

  for (fahr = UPPER; fahr >= LOWER; fahr -= STEP) {
    celsius = 5.0 * (fahr - 32.0) / 9.0;
    printf("%6.0f%6.1f\n", fahr, celsius);
  }
}
