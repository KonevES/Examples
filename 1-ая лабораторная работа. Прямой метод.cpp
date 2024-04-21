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
double determinant_plus_forward_motion(double** matrix, double* answer_matrix, int n){
    int i,j;
    double det=1;
    for (i=0;i<n;++i) {
        int k = i;
        for (j = i + 1; j < n; ++j) {
            if (fabs(matrix[j][i]) > fabs(matrix[k][i])) {
                k = j;
            }
        }
        if (fabs(matrix[k][i]) < eps) {
            det = 0;
            break;
        }
        if (i != k) {
            swap_rows_matrices(matrix, k, i);
            swap_rows_vector(answer_matrix, k, i);
            det = -det;
        }
        det *= matrix[i][i];
        for (j = i + 1; j < n; ++j) {
            if (fabs(matrix[j][i]) > eps) {
                double coeff = matrix[j][i] / matrix[i][i];
                for (k = 0; k < n; ++k) {
                    matrix[j][k] -= matrix[i][k] * coeff;
                }
            answer_matrix[j]-=answer_matrix[i]*coeff;
            }
        }
    }
    return det;
}
int main(){
    int n;
    scanf("%d", &n);
    double **matrix;
    double *answer_matrix;
    double *solve_matrix;
    solve_matrix=(double*) malloc(n*sizeof(double ));
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
    double det=fabs(determinant_plus_forward_motion(matrix,answer_matrix,n));

    if (det<eps){
        printf("DOES NOT EXIST");
        printf("%lf",&det);
    }
    else {
        // обратный ход
        int k;
        for (i = n - 1; i >= 0; --i) {
            answer_matrix[i]/=matrix[i][i];
            matrix[i][i]=1;
            for (j=i-1; j>=0; --j){
                double coef=matrix[j][i] / matrix[i][i];
                matrix[j][i]-=matrix[i][i]*coef;
                answer_matrix[j]-=answer_matrix[i]*coef;
            }
        }
    }
    for (i=0;i<n;++i){
        double summ=0;
        std::cout<<answer_matrix[i]<<" ";
    }
    std::cout<<std::endl;
    std::cout<<"Determinant "<<det;
    return 0;
}
