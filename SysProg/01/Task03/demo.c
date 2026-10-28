#include <stdio.h>

#include "mystring.h"

int main(void)
{
    char greeting[] = "Hallo";
    char suffix[] = ", Welt!";
    char joined[32];
    char copied[32];
    char empty[] = "";
    char filled[6];
    char byte_source[] = "Bytes kopieren";
    char byte_copy[sizeof byte_source];

    /* String kopieren und danach erweitern. */
    mystrcpy(joined, greeting);
    mystrcat(joined, suffix);
    mystrcpy(copied, joined);

    printf("mystrlen(\"%s\") = %lu\n", joined, mystrlen(joined));
    printf("mystrlen(leerer String) = %lu\n", mystrlen(empty));
    printf("mystrcpy + mystrcat: %s\n", joined);
    printf("mystrcmp(gleich) = %d\n", mystrcmp(joined, copied));
    printf("mystrcmp(Apfel, Birne) = %d\n", mystrcmp("Apfel", "Birne"));

    /* Den vollständigen Byteblock einschließlich Nullbyte kopieren. */
    mymemcpy(byte_copy, byte_source, sizeof byte_source);
    printf("mymemcpy: %s\n", byte_copy);

    /* Fünf Punkte setzen und das letzte Byte als Terminator verwenden. */
    mymemset(filled, '.', sizeof filled - 1);
    filled[sizeof filled - 1] = '\0';
    printf("mymemset: %s\n", filled);

    return 0;
}
