#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STRSIZE 1024
#define SEPARATOR ','

char *get_lexem(FILE *fin){
    char ch, *lexem = (char*) malloc(sizeof(char) * STRSIZE);
    int count = 0;

    while(!feof(fin) && count < STRSIZE - 1){
        fscanf(fin, "%c", &ch);
        if (ch != '\n' && ch != SEPARATOR){
            lexem[count] = ch;
            count++;
        } else break;
    }
    lexem[count] = '\0';

    return lexem;
}

int main()
{
    FILE *fin = NULL, *fout = NULL;
    char path[STRSIZE], *lexem;

    printf("File path: ");
    scanf("%s", path);
    if (NULL == (fin = fopen(path, "r"))) {
        printf("Wrong file location\n");
        exit(1);
    }

    while (!feof(fin)){
        lexem = get_lexem(fin);
        printf("%s\n", lexem);
        free(lexem);
    }

    fclose(fin);
    return 0;
}
