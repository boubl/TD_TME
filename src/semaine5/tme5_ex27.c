#include <stdio.h>
int word_count(char *string) {
  int count = 0;
  int i = 1;
  if (string[0] != ' ' && string[0] != '\0')
    count++;
  while (string[i] != '\0') {
    int was_space = string[i-1] == ' ';
    int is_space = string[i] == ' ' || string[i] == '\0';
    if (was_space && !is_space) count++;
    i++;
  }
  return count;
}

int main() {
  char *text1 = ""; // pas d'espace
  char *text2 = "double  espace";
  char *text3 = " hello world ";
  printf("text1: %d\n", word_count(text1));
  printf("text2: %d\n", word_count(text2));
  printf("text3: %d\n", word_count(text3));
}
