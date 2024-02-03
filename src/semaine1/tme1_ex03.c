#include <stdio.h>

int alternative(int n1, int n2, int n3) {
    int res;
    if (n1 > 8) {
        res = 3;
    } else {
        if (n3 == 20) {
            res = 2;
        } else {
            if ((n2 >= 10) && (n3 >= 10)) {
                res = 1;
            } else {
                res = 0;
            }
        }
    }
    return res;
}

int main() {
    // Cas ou la fonction renvoie 3
    printf("res = %d\n", alternative(9, 1, 1));
    // Cas ou la fonction renvoie 2
    printf("res = %d\n", alternative(8, 1, 20));
    // Cas ou la fonction renvoie 1
    printf("res = %d\n", alternative(8, 10, 10));
    // Cas ou la fonction renvoie 0
    printf("res = %d\n", alternative(8, 1, 1));

    return 0;
}
