#include<stdio.h>
#include<stdlib.h>

#include <sys/time.h>

void allocate_mem(float ** a, float ** b, float ** c);
void get_walltime(double *wct);

int main(){
    int i, j, k;
    float *a, *b, *c, *pa, *pb, *pc;
    float sum = 0;
    double ts, te;
    allocate_mem(&a, &b, &c);
    // initialisation of arrays
    for(i = 0; i < N*N; i++){
        a[i] = 2.0;
        b[i] = 3.0;
        c[i] = 20.0;
    }
    // get start time
    get_walltime(&ts);  
    // payload
    pc = c;
    for(k = 0; k < N; k++) { // for loop #1
        pb = b;
        for(j = 0; j < N; j++){ // for loop #2
            pa = a + k * N;
            sum = 0.0;
            for (i = 0; i < N; i++) { // for loop #3
                sum += (*pa) * (*pb);
                pa++, pb++;
            }
            *pc = sum;
            pc++;
        }
    }
    // get end time
    get_walltime(&te);
    for (i = 0; i < N*N; i++){
        if(c[i] != 6 * N){
            printf("wrong outcome\n");
            break;
        }
    }
    // printf("it took %lfsec %lfMflops\n", te - ts, (unsigned long) N * N * N/((te-ts)*1e6));
    printf("%d\t%lf\t%lf\n", N, te - ts, (unsigned long) N * N * N/((te-ts)*1e6));

    free(a);
    free(b);
    free(c);
    return 0;
}

void allocate_mem(float ** a, float ** b, float ** c){
    *a = (float *) malloc(sizeof(float) * N * N);
    if(!*a) {
        printf("Problem Alocating Memory\n");
        exit(-1);
    }
    *b = (float *) malloc(sizeof(float) * N * N);
    if(!*b) {
        printf("Problem Alocating Memory\n");
        free(*a);
        exit(-1);
    }
    *c = (float *) malloc(sizeof(float) * N * N);
    if(!*c) {
        printf("Problem Alocating Memory\n");
        free(*a);
        free(*b);
        exit(-1);
    }
}

void get_walltime(double *wct) {
    struct timeval tp;
    gettimeofday(&tp,NULL);
    *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}
