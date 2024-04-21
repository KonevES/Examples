#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <type_traits>

void swap_rows_matrices(double** a,int to_row,int from_row ){
    double *p1=a[to_row], *p2=a[from_row];
    a[from_row]=p1;
    a[to_row]=p2;
}

void swap_rows_vector(double* a,int to_row,int from_row ){
    double p1=a[to_row], p2=a[from_row];
    a[from_row]=p1;
    a[to_row]=p2;
}

long double eps=1E-18;
void method_of_rotation(double** matrix, double* answer_vector, int n){
    int i,j,k;
    double c,s,r,b,q,f1,f2;
    for (i=0;i<n;++i) {
        for (j = i + 1; j < n; ++j) { //моделирование умножения матрицы - на матрицу вращения. С и S - косинус и синус некоторого угла O.
            r=sqrt(matrix[i][i]*matrix[i][i]+matrix[j][i]*matrix[j][i]);
            c=matrix[i][i]/r;
            s=matrix[j][i]/r;
            for (k=0; k<n; ++k){
                b=matrix[i][k]*c+matrix[j][k]*s;
                q=matrix[i][k]*(-1*s)+matrix[j][k]*c;
                matrix[i][k]=b;
                matrix[j][k]=q;
            }
            f1=answer_vector[i]*c+answer_vector[j]*s;
            f2=answer_vector[i]*(-1*s)+answer_vector[j]*c;
            answer_vector[i]=f1;
            answer_vector[j]=f2;
        }
    }
}
int main(){
    int n;
    scanf("%d", &n);
    double **matrix;
    double *answer_matrix;
    answer_matrix=(double*) malloc(n*sizeof(double ));
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
    }
    method_of_rotation(matrix,answer_matrix,n); //приведение к треугольному виду методом вращения
    for (i = n - 1; i >= 0; --i) {
        answer_matrix[i]/=matrix[i][i];
        matrix[i][i]=1;
        for (j=i-1; j>=0; --j){
            double coef=matrix[j][i] / matrix[i][i];
            matrix[j][i]-=matrix[i][i]*coef;
            answer_matrix[j]-=answer_matrix[i]*coef;
        }
    }
    for (i=0;i<n;++i){
        double summ=0;
        std::cout<<answer_matrix[i]<<" ";
    }
    std::cout<<std::endl;
    return 0;
}
