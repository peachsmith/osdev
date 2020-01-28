#include "libc/ctype.h"

int isalnum(int c)
{
    return (c >= 'A' && c <= 'Z')
        || (c >= 'a' && c <= 'z')
        || (c >= '0' && c <= '9');
}

int isalpha(int c)
{
    return (c >= 'A' && c <= 'Z')
        || (c >= 'a' && c <= 'z');
}

int iscntrl(int c)
{
    return (c >=0 && c <= 31) || c == 127;
}

int isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int isgraph(int c)
{
    return c >= 33 && c <= 126;
}

int islower(int c)
{
    return c >= 'a' && c <= 'z';
}

int isprint(int c)
{
    return c >= 32 && c <= 126;
}

int ispunct(int c)
{
    return c >= 'a' && c <= 'z';
}

int isspace(int c)
{
    switch (c)
    {
        case '\n':
        case '\t':
        case '\f':
        case '\v':
        case '\r':
        case ' ':
        return 1;

        default: return 0;
    }
}

int isupper(int c)
{
    return c >= 'A' && c <= 'Z';
}

int isxdigit(int c)
{
    return (c >= '0' && c <= '9')
        || (c >= 'A' && c <= 'F')
        || (c >= 'a' && c <= 'f');
}

int tolower(int c)
{
    if (isupper(c))
    {
        return c + ('a' - c);
    }

    return c;
}

int toupper(int c)
{
    if (islower(c))
    {
        return c - (c - 'A');
    }

    return c;
}
