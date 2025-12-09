#include<stdio.h>
#include<stdlib.h>
#include<string.h>

long long max(long long a, long long b);

int main(){
    FILE *fp_r;
    FILE *fp_w;
    fp_r = fopen("testcase3.txt", "r");
    fp_w = fopen("output3.txt", "w");
    int T = 0;
    fscanf(fp_r, "%d ", &T);
    
    for(int repeat = 0;repeat < T;repeat++){
        char a[250];
        char *s = a;
        fgets(s, sizeof(a), fp_r);   //輸入一整行
        int len = 0;
        len = strlen(s);
        while(len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) {
            s[len-1] = '\0';
            len--;
        }
        long long *dp = (long long *) malloc(sizeof(long long)*(len + 1));

        *(dp + len) = 0;

        for(int i = len - 1;i >= 0;i--){
            *(dp + i) = 0;
            long long val = 0;
            if(*(s + i) == '0'){
                *(dp + i) = *(dp + i + 1);   //如果要處理的是0的話只有0一種切法
                continue;
            }
            for(int j = i;j < len;j++){
                val = val*10 + (*(s + j) - '0');
                if(val > __INT32_MAX__){
                    
                    break;
                }
                
                *(dp + i) = max(*(dp + i), *(dp + j + 1) + val); //如果這種切法有更大的和的話就更新
            }
        }
        fprintf(fp_w, "%lld\n", *dp);
        free(dp);
    }
    fclose(fp_r);
    fclose(fp_w);
    return 0;
}

long long max(long long a, long long b){
    return (a > b) ? a : b;
}