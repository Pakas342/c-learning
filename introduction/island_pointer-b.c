#include <stdio.h>

int secret_location_gold = 10;
int *secret_map = &secret_location_gold;

int main() {
  int wallet = 0;
  int **map_to_secret_map = &secret_map;

  wallet = **map_to_secret_map;
  **map_to_secret_map = 0;


  printf("There are %d gold pots in the island\n", **map_to_secret_map);

  printf("There are %d gold pots in my wallet\n", wallet);
}
