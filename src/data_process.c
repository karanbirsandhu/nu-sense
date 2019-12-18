/* this file implements sensor fusion algorithm and
* performs step1 - step 7
*/

#include "data_process.h"
#include "externs.h"

float q_percent;

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

/* calculation of contribution rate of every sensor using eigen vectors */
void contri_rate_calc_kth(gsl_vector* evec, double* alpha) {
    
    double sum = 0;
    
    for (int i = 0; i < sensor_number; i++) {
        double evec_i = gsl_vector_get (evec, i);
        sum = sum + evec_i;
    }

    printf("STEP 4: ALPHAS: \n");
    for (int i = 0; i < sensor_number; i++) {
        double evec_i = gsl_vector_get (evec, i);
        alpha[i] = evec_i / sum;
        printf("alpha%d=%f, ", i, alpha[i]);
    }

    printf("\n\n");
}

/* calculation of accumulated contribution rate for all sensors using previous calculations  */
void major_contri_calc(double *alpha, double *phi) {

    printf("STEP 5: PHIS: \n");
    phi[0] = alpha[0];
    printf("phi0=%f, ", phi[0]);
    for (int i = 1; i < sensor_number; i++) {
        phi[i] = phi[i-1]+ alpha[i];
        printf("phi%d=%f, ", i, phi[i]);
    }

    printf("\n\n");
}

/* calculation of integrated support degree for every sensor using
* output of step 3 and step 5 */
void integ_supp_score_calc(double* alpha, gsl_matrix* y, gsl_vector* Z) {

    gsl_vector* z_i = gsl_vector_alloc(sensor_number);
    for (int i = 0; i < sensor_number; i++) {
        gsl_vector_view y_i    = gsl_matrix_column (y, i);
        gsl_vector_memcpy(z_i, &y_i.vector);
        gsl_vector_scale(z_i, alpha[i]);

        gsl_vector_add(Z, z_i);
    }

    printf("STEP 6: Z: \n");
    for(int j = 0; j < sensor_number; j++){
        double z_ij = gsl_vector_get (Z, j);
        printf ("z%d=%g, ", j, z_ij);
    }

    printf("\n\n");

    gsl_vector_free(z_i);
}

/* function performs elimination of incorrect data using predefined equations */
void elliminate_incorrect_data(gsl_vector* Z, int* sensor_correction) {
    double z_sum = 0;
    double z[sensor_number];
    for (int i = 0; i < sensor_number; i++) {
        z[i] = gsl_vector_get (Z, i);
        z_sum = z_sum + z[i];
    }

    printf("STEP 7-1:\nElliminate incorrect data:\n");
    printf("Sigma of z_i=%f\n", z_sum);
    printf("Abs of sigma/sensor_number=%f\n", fabs(z_sum/sensor_number));

    printf("70 percent of Abs of sigma/sensor_number=%f\n", fabs(z_sum/sensor_number)*q_percent);
    
    for (int i = 0; i < sensor_number; i++) {
        if (fabs(z[i]) < (fabs(z_sum / sensor_number)) * q_percent) {
            sensor_correction[i] = 0;
            printf("z-%d does not satisfy!  fabs(z[i])=%f<%f\n", i, fabs(z[i]), (fabs(z_sum / sensor_number)) * q_percent);
        } else {
            sensor_correction[i] = 1;
            printf("z-%d satisfies!  fabs(z[i])=%f>%f\n", i, fabs(z[i]), (fabs(z_sum / sensor_number)) * q_percent);
        }
    }

    printf("\n");
}

/* calculation of weight coefficient in accordance with integrated
* support degree calculated in step 6.
* eliminated values from the previous step are left out */
void weight_coeff_calc(gsl_vector* Z, int* sensor_correction, double* omega) {
    double z_sum = 0;
    double z[sensor_number];
    
    for (int i = 0; i < sensor_number; i++) {
        z[i] = gsl_vector_get (Z, i);
        z_sum = z_sum + z[i] * sensor_correction[i];
    }
    
    
    printf("Sum of z_i disregarded %f",z_sum);

    printf("STEP 7-2: Weight coefficient Omegas:\n");
    for (int i = 0; i < sensor_number; i++) {
        omega[i] = (sensor_correction[i]*z[i]) / z_sum ;
        printf("omega%d=%f, ", i, omega[i]);
    }
    printf("\n\n");
}

/* generates the fused output */
double fused_output(double* omega, double* group_values) {
    double fused = 0;
    for (int i = 0; i < sensor_number; i++) {
        fused = fused + omega[i] * group_values[i];
    }
    return fused;
}
