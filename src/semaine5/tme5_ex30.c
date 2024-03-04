#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN 0
#define MAX 10

int init(int *tab) {
  int size = rand() % (MAX - MIN + 1) + MIN;
  for (int i = 0; i < size; i++) {
    int random = rand() % 2;
    tab[i] = random;
  }
  tab[size] = -1;
  return size;
}

void print_tab(int *tab, int size) {
  printf("{");
  for (int i = 0; i < size; i++) {
    printf(" %d", tab[i]);
    if (i + 1 < size)
      printf(",");
  }
  printf(" }\n");
}

int compress_tab(int *tab_brut, int *tab_compress) {
  int counter = 1;
  int i = 1;
  int i_c = 0;
  while (tab_brut[i-1] != -1) {
    if (tab_brut[i] == tab_brut[i-1])
      counter++;
    else {
      if (counter > 1) {
        tab_compress[i_c] = counter;
        i_c++;
      }
      tab_compress[i_c] = tab_brut[i-1];
      i_c++;
      counter = 1;
    }
    i++;
  }
  tab_compress[i_c] = -1;
  return i_c;
}

int decompress_tab(int *tab_brut, int *tab_compress) {
  int i = 0;
  int i_c = 0;
  while (tab_compress[i_c] != -1) {
    int n = tab_compress[i_c++];
    if (n > 1) {
      int next = tab_compress[i_c++];
      for (int j = 0; j < n; j++) {
        tab_brut[i++] = next;
      }
    } else {
      tab_brut[i++] = n;
    }
  }
  tab_brut[i] = -1;
  return i; 
}

void affiche_tab(int tab[]){
  int i=0;

  while (tab[i] != -1){
    printf("%d ",tab[i]);
    i++;
  }
  printf("\n");
}

int main() {
  // srand(time(NULL));
  // int tab[MAX+1];
  // int size = init(tab);
  // print_tab(tab, size);
  //
  // int tab_c[MAX+1];
  // int size_c = compress_tab(tab, tab_c);
  // print_tab(tab_c, size_c);
  //
  // int tab_dc[MAX+1];
  // int size_dc = decompress_tab(tab_dc, tab_c);
  // print_tab(tab_dc, size_dc);
  {
    int tab1[20];
    int tab2[]={2,0,4,1,0,3,1,2,0,1,4,0,-1};
    decompress_tab(tab1,tab2);
    affiche_tab(tab1);;
   }
}
