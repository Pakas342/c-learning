#include <stdio.h>

int secret_location_gold = 10;

int main() {
  int wallet = 0;
  int *map_to_secret = &secret_location_gold;

  wallet = *map_to_secret;
  *map_to_secret = 0;

  printf("There are %d gold pots in my wallet\n", wallet);
  printf("There are %d gold pots in the island\n", *map_to_secret);
}
