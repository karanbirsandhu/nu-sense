#include "data_process.h"
#include "externs.h"

/* function creates n*n support degree matrix using gsl matrix and group values
* first step of the algorithm */
void support_degree_generator(gsl_matrix* D, double * group_values) {
    double d_ij;
    for (int i = 0; i < sensor_number; i++) {
        for (int j = 0; j < sensor_number; j++) {
            if (i == j) {
                d_ij = exp(0);
            } else {
                d_ij = 1 / exp(fabs(group_values[i] - group_values[j]));
            }
            gsl_matrix_set(D, i, j, d_ij);
        }
    }

    printf("\nSTEP 1: Support Degree Matrix:\n");
    for (int i = 0; i < sensor_number; i++) {
        for (int j = 0; j < sensor_number; j++) {
            double a = gsl_matrix_get(D,i,j);
            printf("%f\t", a);
        }
        printf("\n");
    }

    printf("\n");
}

/* generate eigen values and corresponding eigen vectors using the support degree Matrix
* second step of the algorithm */
void eigenvec_calc(gsl_vector* evec, gsl_matrix* T, gsl_matrix* D) {

    gsl_eigen_symmv_workspace *w = gsl_eigen_symmv_alloc (sensor_number);

    gsl_eigen_symmv(D, evec, T, w);

    gsl_eigen_symmv_free (w);
    gsl_eigen_symmv_sort (evec, T, GSL_EIGEN_SORT_ABS_DESC);

    printf("STEP 2: \nEigenValues: \n");
    for(int i = 0; i < sensor_number; i++){
        double evec_i = gsl_vector_get (evec, i);
        printf ("%g, ", evec_i);
    }

    printf("\n\n");

    printf("EigenVectors: \n");
    for(int i = 0; i < sensor_number; i++){
        gsl_vector_view T_i    = gsl_matrix_column (T, i);
        printf ("eigenvector%d = ", i);
        for(int j = 0; j < sensor_number; j++){
            double evec_j = gsl_vector_get (&T_i.vector, j);
            printf ("%g, ", evec_j);
        }
        printf("\n");
    }
    printf("\n");
}

/* function calculates principal component using eigen values and eigen vectors */
void principal_comp_calc(gsl_matrix* T, gsl_matrix* D, gsl_matrix* Y) {

    gsl_vector* y_i = gsl_vector_alloc(sensor_number);

    printf("STEP 3: Principal Components of the D matrix:\n");

    for(int i = 0; i < sensor_number; i++){
        gsl_vector_view T_i    = gsl_matrix_column (T, i);
        gsl_blas_dgemv(CblasTrans, 1.0, D, &T_i.vector, 0.0, y_i);
        gsl_matrix_set_col(Y, i, y_i);

        printf("y%d= ", i);

        for(int j = 0; j < sensor_number; j++){
            double y_ij = gsl_vector_get (y_i, j);
            printf ("%g, ", y_ij);
        }
        printf("\n");
    }

    gsl_vector_free(y_i);

    printf("\n");

}
