#include <stdio.h>

#define ASCII_OFFSET 48

int atoi(const char str[]) {
  int final_value = 0;
  int i = 0;
  int current_char = str[i];
  int is_negative = 0;
  int array_size = 0;

  while (current_char != 0) {
    array_size = i;
    i++;
    current_char = str[i];
  }

  i = 0;
  current_char = str[i];

  if (current_char == 45) {
    is_negative = 1;
    i++;
    current_char = str[i];
  }

  while (current_char != '\0') {
    if (current_char > ASCII_OFFSET + 10 || current_char < ASCII_OFFSET) {
      printf("Unknown symbol given");
      return 0;
    }
    int char_value = current_char - ASCII_OFFSET;
    int decimal_position = (array_size - i);
    for (int j = decimal_position; j > 0; j--) {
      char_value = char_value * 10;
    }
    final_value += char_value ;
    i++;
    current_char = str[i];
  }

  if (is_negative) {
    final_value = final_value * -1;
  }

  return final_value;
};

int main() {
  printf("%d\n", atoi("-1234"));
  return 0;
}
