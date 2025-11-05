#include<stdio.h>
#include<stdlib.h>

struct term
{
    int col;
    int row;
    int value;
};
typedef struct term Term;

Term* fast_transpose(Term *terms);
Term* to_terms(int *matrix, int rows, int cols, int numOfterms);

int main(){
    int matrix[6][6] = {{15,0,0,22,0,-15},
                        {0,11,3,0,0,0},
                        {0,0,0,-6,0,0},
                        {0,0,0,0,0,0},
                        {91,0,0,0,0,0},
                        {0,0,28,0,0,0}};
    int numOfterms = 0;
    for(int i = 0;i < 6;i++){
        for(int j = 0;j < 6;j++){
            if(matrix[i][j] != 0) numOfterms++;
        }
    }
    Term *a = to_terms(matrix, 6, 6, numOfterms);
    for(int i = 0;i <= numOfterms;i++){
        printf("%d %d %d\n", (a+i)->row, (a+i)->col, (a+i)->value);
    }
    printf("\n");
    Term *b = fast_transpose(a);
    for(int i = 0;i <= numOfterms;i++){
        printf("%d %d %d\n", (b+i)->row, (b+i)->col, (b+i)->value);
    }
    free(a);
    free(b);
    return 0;
}

Term* fast_transpose(Term *terms){
    int numOfterms = terms[0].value;
    Term *b = malloc(sizeof(Term) * (numOfterms + 1));
    int *row_terms = malloc(sizeof(int)*(terms[0].col - 1));
    int *starting_point = malloc(sizeof(int)*(terms[0].col - 1));
    int num_cols = terms->col;
    b->row = num_cols;
    b->col = terms->row;
    b->value = numOfterms;
    for(int i = 0;i < num_cols;i++){
        *(row_terms + i) = 0;
    }
    for(int i = 1;i <= numOfterms;i++){
        row_terms[(terms+i)->col]++;
    }
    starting_point[0] = 1;
    for(int i = 1;i < num_cols;i++){
        *(starting_point+i) = *(starting_point + i - 1) + *(row_terms + i - 1);
    }
    for(int i = 1;i <= numOfterms;i++){
        int j = *(starting_point + ((terms + i)->col));
        *(starting_point + ((terms + i)->col)) += 1;
        (b + j)->row = (terms + i)->col;
        (b + j)->col = (terms + i)->row;
        (b + j)->value = (terms + i)->value;
    }
    return b;
}

Term* to_terms(int *matrix, int rows, int cols, int numOfterms){
    Term *a = malloc(sizeof(Term) * (numOfterms + 1));
    a->row = rows;
    a->col = cols;
    a->value = numOfterms;
    int counter = 0;
    for (int i = 0;i < rows;i++){
        for(int j = 0;j < cols;j++){
            if(*(matrix + (i*cols + j)) != 0){
                (a + counter + 1)->row = i;
                (a + counter + 1)->col = j;
                (a + counter + 1)->value = *(matrix + (i*cols + j));
                counter++;
            }
        }
    }
    return a;
}