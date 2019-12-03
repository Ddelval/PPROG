#include <stdio.h>
#include "table.h"
#include "type.h"


int main() {
        int ncols = 4;
        type_t t[] =  {INT, STR, STR, INT};
        table_t * ta;
        table_create("myfile.dat", ncols, t);
        ta = table_open("myfile.dat");

        type_t *q = table_types(ta);
        for (int i=0; i<table_ncols(ta); i++)
                printf("%d  ",q[i]);

        printf("\n%d,\n",table_ncols(ta));

        int last = table_last_pos(ta);
        printf("last position: %d\n\n\n", last);

        void ** sent = NULL;
        sent = (void **) calloc(table_ncols(ta), sizeof(void *));
        for(int i = 0; i < table_ncols(ta); i++) {
                sent[i] = sent[0] + i * sizeof(void *);
        }

        int num = 7, aux = 0, num2 = 789;
        char str1[6] = "Pedro", str2[5] = "Luis";

        sent[0] = (void*) &num;
        sent[1] = (void*) str1;
        sent[2] = (void*) str2;
        sent[3] = (void*) &num2;

        aux = table_insert_record(ta,sent);
        printf("The function table_insert_record returns: %d\n", aux);
        aux = table_read_record(ta, last);
        printf("The function table_read_record returns: %d\n", aux);

        for (int i = 0; i < table_ncols(ta); i++){
          print_value(stdout, t[i], table_get_col(ta, i));
          printf("\n");
        }

        free(sent);

        table_close(ta);
}
