#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

bool isPalindrome(char *p, int i, int j);
int length(char *p);
int min(int a, int b);

int main(){
    int numOftestcase = 0;

    FILE *fp_r;
    FILE *fp_w;
    fp_r = fopen("testcase2.txt", "r");
    fp_w = fopen("output2.txt", "w");
    if (fp_r == NULL || fp_w == NULL) {
        printf("error\n");
        return -1;
    }

    fscanf(fp_r, "%d", &numOftestcase);
    for(int repeat = 0;repeat < numOftestcase;repeat++){
        char *p;
        char str[1001];
        fscanf(fp_r,"%s", str);
        p = str;
        int Length = length(p);
        int *dp = (int *) malloc(sizeof(int)*Length);
        for(int i = 0;i < Length;i++){
            *(dp + i) = i;
        }
        for(int i = 0;i < Length;i++){
            for(int j = 0;j <= i;j++){
                if(j == 0 && isPalindrome(p, j ,i)){
                    *(dp + i) = 0;
                    break;
                }
                if(j != 0 && isPalindrome(p , j, i)){
                    *(dp + i) = min(*(dp + (j - 1)) + 1, *(dp + i));
                }
            }
        }
        /*for(int i = 0;i < Length;i++){
            printf("%d ", *(dp + i));
        }*/
        //printf("\n");
        fprintf(fp_w, "%d\n", *(dp + Length - 1) + 1);
        //printf("%d\n", *(dp + Length - 1) + 1);
        free(dp);
    }
    fclose(fp_r);
    fclose(fp_w);
    return 0;
}

bool isPalindrome(char *p, int begin, int end){
    int length = end - begin + 1;
    if(length == 1) return true;
    for(int i = 0;i <= floor((float)(length)/2);i++){
        if(*(p + begin + i) != *(p + end - i)){
            return false;
        }
    }
    return true;
}

int length(char *p){
    int length = 0;
    for(int i = 0;i < 1001;i++){
        if(*(p + i) == '\0'){
            length = i;
            break;
        }
    }
    return length;
}

int min(int a, int b){
    if(a > b){
        return b;
    } else {
        return a;
    }
}