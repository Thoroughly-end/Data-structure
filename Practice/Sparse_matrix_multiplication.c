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
Term* multiple(Term *A, Term *B);
void storeSum(Term *d, int row, int column, int sum);
int COMPARE(int a, int b);

int main(){
    int a[3][3] = {{5,0,0},{0,11,3},{0,0,2}};
    int b[3][3] = {{5,0,2},{0,11,0},{0,3,0}};

    int numOfterms = 0;
    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 3;j++){
            if(a[i][j] != 0) numOfterms++;
        }
    }
    Term *A = to_terms(a, 3, 3, numOfterms);
    numOfterms = 0;
    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 3;j++){
            if(b[i][j] != 0) numOfterms++;
        }
    }
    Term *B = to_terms(b, 3, 3, numOfterms);
    Term *D = multiple(A, B);
    for(int i = 0;i <= D[0].value;i++){
        printf("%d %d %d\n", (D+i)->row, (D+i)->col, (D+i)->value);
    }
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

Term* multiple(Term *A, Term *B){
    Term *termsA = malloc(sizeof(Term)*(A[0].value + 2));
    Term *TtermsB = malloc(sizeof(Term)*(B[0].value + 2));
    Term *D = malloc(sizeof(Term)*A[0].row*B[0].col);
    D[0].row = A[0].row;
    D[0].col = B[0].col;
    D[0].value = 0;

    for(int i = 0;i <= A[0].value;i++){
        (termsA + i)->row = (A + i)->row;
        (termsA + i)->col = (A + i)->col;
        (termsA + i)->value = (A + i)->value;
    }
    termsA[A[0].value + 1].row = A[A[0].value].row + 1;
    termsA[A[0].value + 1].col = 0;
    termsA[A[0].value + 1].value = 0;
    B = fast_transpose(B);
    for(int i = 0;i <= B[0].value;i++){
        (TtermsB + i)->row = (B + i)->row;
        (TtermsB + i)->col = (B + i)->col;
        (TtermsB + i)->value = (B + i)->value;
    }
    TtermsB[B[0].value + 1].row = B[B[0].value].row + 1;
    TtermsB[B[0].value + 1].col = 0;
    TtermsB[B[0].value + 1].value = 0;

    int rowsA = termsA[0].row, colsA = termsA[0].col, totalA = termsA[0].value;
    int row = termsA[1].row; //the row we are going to multiple.
    int rowsB = TtermsB[0].row, colsB = TtermsB[0].col, totalB = TtermsB[0].value;
    int column = TtermsB[1].row; //the column we are going to multiple.
    int rowBegin = 1; // A should start with where.
    for(int i = 1;i <= totalA;){
        column = TtermsB[1].row;
        int sum = 0;
        for(int j = 1; j <= totalB + 1;){
            if(termsA[i].row != row){
                //if the next term's row is not equal to(greater than) the row we are going to process.
                storeSum(D, row, column, sum);
                //printf("%d\n", sum);
                sum = 0;
                i = rowBegin;
                for(; TtermsB[j].row == column; j++); //make TtermB[j] greater than the column we are processing right now.
                column = TtermsB[j].row; //set the column we are going to process.
            } else if (TtermsB[j].row != column){
                //if the next term's column is not equal to(greater than) the column we are going to process.
                storeSum(D, row, column, sum);
                //printf("%d", sum);
                sum = 0;
                i = rowBegin;
                column = TtermsB[j].row; //So the TtermB[j].row is already greater than the column we are processing right now.
            } else switch (COMPARE(termsA[i].col, TtermsB[j].col))
            {
            case -1:
                i++;
                break;
            case 0:
                sum += termsA[i++].value*TtermsB[j++].value;
                break;
            case 1:
                j++;
                break;
            default:
                break;
            }
        }
        for(; termsA[i].row == row; i++); //make termA[i].row greater than the row we are processing right now.
        rowBegin = i; //where with should start finding at A for saving time.
        row = termsA[i].row; //set the row we are going to process.
    }
    Term *newD = malloc(sizeof(Term)*(D[0].value + 1)); //get rid of the zero after data.
    for(int i = 0;i <= D[0].value;i++){
        (newD + i)->row = (D + i)->row;
        (newD + i)->col = (D + i)->col;
        (newD + i)->value = (D + i)->value;
    }
    return newD;
}

void storeSum(Term *d, int row, int column, int sum){
    if(sum){
        d[++d[0].value].row = row;
        d[d[0].value].col = column;
        d[d[0].value].value = sum;
    }
}

int COMPARE(int a, int b){
    if(a == b){
        return 0;
    } else if (a > b){
        return 1;
    } else {
        return -1;
    }
}