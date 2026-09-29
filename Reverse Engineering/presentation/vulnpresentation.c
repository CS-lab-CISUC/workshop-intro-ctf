#include <stdio.h>
#include <string.h>
void win() {
  printf("You win!\n");
  printf("Here's your flag: CSLAB{th1s_is_th3_usu4l_fl4g_f0Rm4t_12345}\n");
}

void vuln() {
  char buffer[64];
  fgets(buffer, 64, stdin);
  if (strcmp(buffer, "Jo1NCSLAB\n") == 0) {
    win();
  } else {
    printf("Better luck next time!\n");
  }
}

int main() {
  printf("Insert the admin's password:\n");
  vuln();
  return 0;
}
