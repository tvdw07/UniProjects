#include "mystring.h"

size_t mystrlen(char *str)
{
    size_t length = 0;

    /* Das abschließende Nullbyte gehört nicht zur Stringlänge. */
    while (str[length] != '\0') {
        ++length;
    }

    return length;
}

char *mystrcpy(char *dest, const char *src)
{
    char *original_dest = dest;

    /* Die Kopie endet erst, nachdem auch das Nullbyte übertragen wurde. */
    while ((*dest++ = *src++) != '\0') {
    }

    return original_dest;
}

char *mystrcat(char *dest, const char *src)
{
    char *original_dest = dest;

    /* Zuerst das Ende des Zielstrings suchen, dann den Quellstring anhängen. */
    while (*dest != '\0') {
        ++dest;
    }
    while ((*dest++ = *src++) != '\0') {
    }

    return original_dest;
}

int mystrcmp(const char *s1, const char *s2)
{
    /* unsigned char sorgt für den geforderten Vergleich der Bytewerte. */
    while (*s1 != '\0' && (unsigned char)*s1 == (unsigned char)*s2) {
        ++s1;
        ++s2;
    }

    {
        unsigned char c1 = (unsigned char)*s1;
        unsigned char c2 = (unsigned char)*s2;

        if (c1 < c2) {
            return -1;
        }
        if (c1 > c2) {
            return 1;
        }
        return 0;
    }
}

void *mymemcpy(void *dest, const void *src, size_t n)
{
    unsigned char *dest_bytes = (unsigned char *)dest;
    const unsigned char *src_bytes = (const unsigned char *)src;

    /* Byteweise kopieren; Quell- und Zielbereich dürfen sich nicht überlappen. */
    for (size_t i = 0; i < n; ++i) {
        dest_bytes[i] = src_bytes[i];
    }

    return dest;
}

void *mymemset(void *buf, int ch, size_t n)
{
    unsigned char *bytes = (unsigned char *)buf;

    /* Der int-Wert wird für jedes geschriebene Byte auf 8 Bit reduziert. */
    for (size_t i = 0; i < n; ++i) {
        bytes[i] = (unsigned char)ch;
    }

    return buf;
}
