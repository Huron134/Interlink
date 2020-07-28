#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STRSIZE 1024
#define SEPARATOR ','
#define MAX_RECORD_COUNT 10000

struct column{
    char *record[MAX_RECORD_COUNT];
    struct column *next;
};

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

struct column *add_column(char *lexem){
    struct column *col = (struct column *) malloc(sizeof(struct column));

    col->record[0] = lexem;
    for (int i = 1; i < MAX_RECORD_COUNT; i++){
        col->record[i] = NULL;
    }
    col->next = NULL;

    return col;
}

void add_record(struct column *col, int position, char *date, char *time){
    if (!strcmp(date, col->record[0])){
        col->record[position] = time;
    } else {
        if (col->next != NULL){
            add_record(col->next, position, date, time);
        } else {
            col->next = add_column(date);
            add_record(col->next, position, date, time);
        }
    }

    return;
}

void get_line(struct column *table, FILE *fin){
    char *lexem[3];
    int i = 1, p = 1;

    for (int j = 0; j < 3; j++){
        lexem[j] = get_lexem(fin);
    }

    while (table->record[i] != NULL){
        if (!strcmp(table->record[i], lexem[0])){
            if(table->next == NULL) table->next = add_column(lexem[1]);
            add_record(table->next, i, lexem[1], lexem[2]);
            p = 0;
        }
        i++;
    }
    if (p) {
        table->record[i] = lexem[0];
        if(table->next == NULL) table->next = add_column(lexem[1]);
        add_record(table->next, i, lexem[1], lexem[2]);
    }

    return;
}

void print_line(struct column *table, int i, FILE *fout){
    if (table != NULL){
        if (table->record[i] != NULL){
            fprintf(fout, "%s%c", table->record[i], SEPARATOR);
        } else {
            fprintf(fout, "%s%c", "0", SEPARATOR);
        }
        print_line(table->next, i, fout);
    }

    return;
}

int main()
{
    FILE *fin = NULL, *fout = NULL;
    char path[STRSIZE], *lexem;
    struct column *csv_table;
    int i = 0;

    printf("IN file path: ");
    scanf("%s", path);
    if (NULL == (fin = fopen(path, "r"))) {
        printf("Wrong file location\n");
        exit(1);
    }
    printf("OUT file path: ");
    scanf("%s", path);
    if (NULL == (fout = fopen(path, "w"))) {
        printf("Wrong file location\n");
        exit(1);
    }

    lexem  = (char *) malloc(STRSIZE);
    strcpy(lexem, "Name / Date");
    csv_table = add_column(lexem);

    for (int i = 0; i < 3; i++){
        lexem = get_lexem(fin);
        free(lexem);
    }
    while (!feof(fin)){
        get_line(csv_table, fin);
    }

    while (csv_table->record[i] != NULL){
        print_line(csv_table, i, fout);
        fprintf(fout, "\n");
        i++;
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
