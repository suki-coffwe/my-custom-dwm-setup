#include <stdio.h>
#include <ctype.h>

int main()
{
    // Index 0-25: letters 'a'-'z'
    // Index 26-35: digits '0'-'9'
    int seen[36] = {0};
    char word[256];

    // Keep reading words until EOF
    while (scanf("%255s", word) == 1)
    {
        char first_char = tolower((unsigned char)word[0]);

        if (first_char >= 'a' && first_char <= 'z')
        {
            seen[first_char - 'a'] = 1;
        }
        else if (isdigit((unsigned char)first_char))
        {
            seen[26 + (first_char - '0')] = 1;
        }
    }

    // Print remaining unobserved letters ('a'-'z')
    for (char c = 'a'; c <= 'z'; c++)
    {
        if (seen[c - 'a'] == 0)
        {
            printf("%c ", c);
        }
    }

    printf("| ");

    // Print remaining unobserved digits ('0'-'9')
    for (char d = '0'; d <= '9'; d++)
    {
        if (seen[26 + (d - '0')] == 0)
        {
            printf("%c ", d);
        }
    }
    printf("\n");

    return 0;
}
