#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <type_traits>

long double eps=1E-8;
int iter=0;

bool rule_of_stop (double* x_prev, double* x_now, int n){
    double norma_of_x=0;
    for (int i=0; i<n; i++){
        norma_of_x+=sqrt((x_now[i]-x_prev[i])*(x_now[i]-x_prev[i]));
    }
    return (norma_of_x>eps);
}

void method_zaydale(double** matrix, double* answer_vector, double* x_vector_now, double* x_vector_next, int n){
    int i,j,k;
    bool flag;
    do {
        for (i=0; i<n; i++){
            for (j=0; j<n; j++){
                if (j<i){
                    x_vector_next[i]+=(-1)*matrix[i][j]/matrix[i][i]*x_vector_next[j];
                }
                else if (j>i){
                    x_vector_next[i]+=(-1)*matrix[i][j]/matrix[i][i]*x_vector_now[j];
                }
            }
            x_vector_next[i]+=answer_vector[i]/matrix[i][i];
        }
        flag = (rule_of_stop(x_vector_next,x_vector_now,n));

        for (i=0; i<n; i++){
            x_vector_now[i]=x_vector_next[i];
            x_vector_next[i]=0;
        }
        /*
        for (i=0; i<n; i++){
            std::cout<<x_vector_now[i]<<" ";
        }
        std::cout<<std::endl;
        */
        iter+=1;
    } while (flag);
}
int main(){
    int n;
    scanf("%d", &n);
    double **matrix;
    double *answer_matrix;
    double *x_vector_now;
    double *x_vector_next;
    answer_matrix=(double*) malloc(n*sizeof(double ));
    x_vector_now=(double*) malloc(n*sizeof(double ));
    x_vector_next=(double*) malloc(n*sizeof(double ));
    matrix= (double**) malloc(n*sizeof(double));
    int i, j;
    for (i = 0; i < n; ++i) {
        matrix[i] = (double *) malloc(n * sizeof(double));
    }
    for (i=0;i<n;++i){
        for (j = 0; j < n; ++j) {
            scanf("%lf", &matrix[i][j]);
        }
    }
    for (i=0;i<n;++i){
        scanf("%lf",&answer_matrix[i]);
        x_vector_now[i]=1;
        x_vector_next[i]=0;
    }

    method_zaydale(matrix,answer_matrix,x_vector_now,x_vector_next,n);
    for (i=0; i<n; i++){
        std::cout<<x_vector_now[i]<<" ";
    }
    std::cout<<std::endl;
    std::cout<<iter;
    return 0;
}
