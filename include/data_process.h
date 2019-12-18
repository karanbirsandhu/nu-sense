#ifndef DATA_PROCESS_H
#define DATA_PROCESS_H

#include <stdio.h>
#include <string.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_statistics.h>
#include <math.h>

void support_degree_generator(gsl_matrix*, double*);
void eigenvec_calc(gsl_vector*, gsl_matrix*, gsl_matrix*);
void principal_comp_calc(gsl_matrix*, gsl_matrix*, gsl_matrix*);
void contri_rate_calc_kth(gsl_vector*, double*);
void major_contri_calc(double*, double*);
void integ_supp_score_calc(double*, gsl_matrix*, gsl_vector*);
void elliminate_incorrect_data(gsl_vector*, int*);
void weight_coeff_calc(gsl_vector*, int*, double*);
double fused_output(double*, double*);

#endif