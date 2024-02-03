// SKETCHPAD 01: Test avec les pointeurs

#include <stdio.h>

// INFO: les fonction foo et bar sont identiques, seuls les noms des parametres changent

void foo(int *ptr, int num) {
    printf("Nous sommes dans la fonction foo\n\n");

    printf("ptr == %p\n", ptr);     // affiche la valeur de ptr, ou l'adresse vers lequel il pointe
    printf("*ptr == %d\n", *ptr);   // affiche la valeur à l'adresse vers lequel pointe ptr
    printf("&ptr == %p\n\n", &ptr); // affiche l'adresse de ptr

    printf("num == %d\n", num); // affiche la valeur de num
    // on pourrait mais c'est pas bien de déreferencer des valeurs au hasard comme num
    printf("&num == %p\n\n", &num); // affiche l'adresse de num

    printf("Au revoir fonction foo\n\n");
}

void bar(int *a, int b) {
    printf("Nous sommes dans la fonction bar\n\n");

    printf("a == %p\n", a);     // affiche la valeur de a, ou l'adresse vers lequel il pointe
    printf("*a == %d\n", *a);   // affiche la valeur à l'adresse vers lequel pointe a
    printf("&a == %p\n\n", &a); // affiche l'adresse de a

    printf("b == %d\n", b); // affiche la valeur de b
    // on pourrait mais c'est pas bien de déreferencer des valeurs au hasard comme b
    printf("&b == %p\n\n", &b); // affiche l'adresse de b

    printf("Au revoir fonction bar\n\n");
}

int main() {
    printf(
        "INFO: les fonction foo et bar sont identiques, seuls les noms des parametres changent\n");
    printf("Nous sommes dans la fonction main\n\n");

    int a = 69;
    int b = 42;

    printf("a == %d\n", a); // affiche la valeur de a
    // on pourrait mais c'est pas bien de déreferencer des valeurs au hasard comme a
    printf("&a == %p\n\n", &a); // affiche l'adresse de a

    printf("b == %d\n", b); // affiche la valeur de b
    // on pourrait mais c'est pas bien de déreferencer des valeurs au hasard comme b
    printf("&b == %p\n\n", &b); // affiche l'adresse de b

    foo(&a, b);
    bar(&a, b);

    printf("Au revoir fonction main\n");
}
