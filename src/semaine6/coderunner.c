#include <stdbool.h>
#include <stdio.h>
#define SEPARATOR "#<ab@17943918#@>#"

int est_deb(char *chDeb, char *chaine) {
    if (*chDeb == '\0') {
        return 1;
    }
    if ((*chaine == '\0') || (*chaine != *chDeb)) {
        return 0;
    }
    return est_deb(chDeb + 1, chaine + 1);
}

int est_incluse(char *str1, char *str2) {
    if (est_deb(str1, str2))
        return 1;
    if (*str2 == '\0')
        return 0;
    return est_incluse(str1, str2 + 1);
}

int main() {
    {
        printf("%d\n", est_incluse("alpha", "alphabet"));
        ;
    }
    printf("%s\n", SEPARATOR);
    {
        printf("%d\n", est_incluse("alpaga", "alphabet"));
        ;
    }
    printf("%s\n", SEPARATOR);
    {
        printf("%d\n", est_incluse("abe", "alphabet"));
        ;
    }
    printf("%s\n", SEPARATOR);
    {
        printf("%d\n", est_incluse("beta", "alphabet"));
        ;
    }
    printf("%s\n", SEPARATOR);
    {
        printf("%d\n", est_incluse("", "alphabet"));
        ;
    }
    printf("%s\n", SEPARATOR);
    {
        printf("%d\n", est_incluse("bet", "alphabet"));
        ;
    }
    printf("%s\n", SEPARATOR);
    {
        printf("%d\n", est_incluse("ber", "alphabet"));
        ;
    }
    return 0;
}
