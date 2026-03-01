#include <stdio.h>

void gets(char *str) { fgets(str, 999999999, stdin); }

int main() {
  // I fill the array with empty bytes to clear existing garbage values
  char username[10] = {'\0'};
  char super_secret_admin_password[30] = "SUPER_SECRET_PLS_DONT_HACK";

  printf("Enter username and please don't hack me: ");
  // why no & here?
  gets(username);
  // the reasons is that, even tho gets needs a pointer, the compiler
  // understands that username is a array of chars, and decay it so gets
  // transformed to a pointer to the first value of the array

  printf("Your username is: %s", username);
  printf("Super secret admin password: %s", super_secret_admin_password);

  return 0;

  /* when calling this and giving a more than 10 char long password, it get
   * overflow into the next memory allocation which happens to be the
   * super_secret_admin_password, and get overriden. That's why gets is so
   * dangerous. */

  /* NOTE: not always the vars will be allocated together, it depends on the
   * compiler */
}
