#include <stdio.h>

int compare_tab(int tab1[], int tab2[]) {
  int i = 0;
  while (tab1[i] != -1 && tab2[i] != -1 && tab1[i] == tab2[i]) {
    i++;
  }
  return -!(tab1[i] == tab2[i]);
}

int main() {
  int tab1[] = { -1 };
  int tab2[] = { 1, 2, 43, 12123, 42, 465, 2342, 435, 67, 7, 8, 35, -1 };
  int tab3[] = { 23123, 123123123, -1 };
  printf("Code is working if you only see ones and no zeros\n");
  printf("%d\n", compare_tab(tab1, tab1) == 0);
  printf("%d\n", compare_tab(tab2, tab3) == -1);
  printf("%d\n", compare_tab(tab2, tab2) == 0);
  return 0;
}
