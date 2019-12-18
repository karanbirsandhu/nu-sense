/* this manages and calls function to execute function
* from beginning to end
*/
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_statistics.h>

#include "fileio.h"
#include "data_process.h"
#include "sensor_history.h"
#include "sensor_validation.h"

#include "externs.h"
void handle_files(char *, double*, double*);
void sensor_validation(double*);
double data_processing(double*);

int sensor_number;
int temperatureMin = 30;
int temperatureMax = 60;
int data_number;
int group_number;

/* path and names of the files used in the program
* which include input file, output file and sensor history file */
char input_file_name[255] = "../data/input_data/sample_input.csv";
char history_file_name[255] = "../data/sensor_history/sensor_history.csv";
char output_file_name[255] = "../data/output_data/output.csv";

int main(int argc, char *argv[]) {

    printf("\nApplication is running....\n");

    /* user input for the number of sensors to process */
    printf("Insert number of sensors:");
    scanf("%d", &sensor_number);

    /* determining the time intervals for the sensors */
    int time_interval;

    printf("Insert the number of time intervals:");
    scanf("%d", &time_interval);

    double values[256];
    double time_value[256];
    double group_values[sensor_number];
    double fused;

    /* function performs reading of sensor data
    * @input : file name
    * @output : sensor time and associated values
    */
    handle_files(input_file_name, &time_value[0], &values[0]);

    /* number of calculations to be performed based on the user input */
    group_number = data_number / sensor_number;

    for (int i = 0; i < group_number; i++) {
        time_value[i] = time_value[i * sensor_number];
    }

    /* check whether the user input and actual file data matches */
    if (group_number != time_interval) {
        printf("ERROR: Time interval numbers, sensor numbers do not match with your input file!\n");
        exit(0);
    }
    
    sensor_validation(&values[0]);

    /* seperating multiple data computations in the output file*/
    FILE *fpout = fopen(output_file_name, "a");
    fprintf(fpout, "--------------\n");
    fclose(fpout);

    FILE *fphis = fopen(history_file_name, "a");
    fprintf(fphis, "--------------\n");
    fclose(fphis);

    for (int i = 0 ; i < group_number; i++) {
        for (int j = 0; j < sensor_number; j++) {
            group_values[j]  = values[i*sensor_number + j];
        }
        printf("\n---------------------Time interval %d---------------------\n\n", i);

        double fused = data_processing(&group_values[0]);
        float time_val_file = (float)time_value[i];

        write_data(time_val_file, fused, output_file_name);
    }
    return 1;
}

/* function checks whether an attempt made to open a file is succuessful or not */
void handle_files(char* input_file_name, double* time_value, double* values) {
    printf("\n");
    if (input_file_name != NULL) {
        data_number = read_data(&time_value[0], &values[0], input_file_name);

        if ( data_number == -1 ) {
            printf("ERROR: File open failed!\n");
            exit(0);
        } else if ( data_number == 0 ) {
            printf("ERROR: No. of sensors inserted and in file are different!\n");
            exit(0);
        } else if ( data_number > 0) {
            printf("Read data successfully!\n");
        }
    } else {
        printf("ERROR: Input file does not exist!\n");
        exit(0);
    }
}

/* function performs sensor validation for group values
* @input : group Values
*/
void sensor_validation(double* group_values) {

    int res = reading_validation(&group_values[0]);
    if (res == 1) {
        int check = frozen_value_check(&group_values[0]);

    }

    if (res == 0) {
        printf("ERROR: Temperature values are out of range! Check history file.\n");
        int check = frozen_value_check(&group_values[0]);
    }

}

/* function performs sensor fusion algo using group Values
* @input : group Values
* @output : fused result
*/
double data_processing(double* group_values) {

    printf("Sensor Values:\n");

    for (int i = 0; i < sensor_number; i++) {
        printf("x%d=%f, ", i, group_values[i]);
    }
    /* Step 1: Calc the Support Degree Matrix */

    gsl_matrix* D = gsl_matrix_alloc(sensor_number,sensor_number);
    support_degree_generator(D, &group_values[0]);    //function in data_process.c to get D - Support Degree Matrix



    /* Step 2: Calc eigenval & eigenvec */

    gsl_matrix* T = gsl_matrix_alloc(sensor_number,sensor_number);
    gsl_vector* evec = gsl_vector_alloc(sensor_number);
    gsl_matrix* Temp = gsl_matrix_alloc(sensor_number,sensor_number);

    gsl_matrix_memcpy(Temp, D);
    eigenvec_calc(evec, T, Temp);    //function in data_process.c to get evec - eigen group_values & T - vectors



    /* Step 3: Principal Comp Calc */

    gsl_matrix* y = gsl_matrix_alloc(sensor_number,sensor_number);
    principal_comp_calc(T, D, y);    //function in data_process.c to get T - Principal Components

    /* Step 4: Calc the contri rate of the kth principal comp */

    double alpha[sensor_number];
    contri_rate_calc_kth(evec, &alpha[0]);    //function in data_process.c to get alpha

    /* Step 5: Calc the contri rate of the m principal comp */

    double phi[sensor_number];
    major_contri_calc(&alpha[0], &phi[0]);    //function in data_process.c to get phi


    /* Step 6: Compute the integrated support degree score */

    gsl_vector* Z = gsl_vector_alloc(sensor_number);
    integ_supp_score_calc(&alpha[0], y, Z);    //function in data_process.c to get z_i
    
    /* Step 7-1: Eliminate incorrect data */
     
    int sensor_correction[sensor_number];
    elliminate_incorrect_data(Z, &sensor_correction[0]);    //function in data_process.c to elliminate incorrect datas
    
    /* Step 7-2: Compute the weight coefficient for each sensor */
     
    double omega[sensor_number];
    weight_coeff_calc(Z, &sensor_correction[0], &omega[0]);     //function in data_process.c to get omega

    /* Step 7-3: Compute the fused output */
     
    double fused;
    fused = fused_output(&omega[0], &group_values[0]);    //function in data_process.c to get fused output
    printf("FINAL STEP: \nThe fused output is %f\n", fused);

    /* Free memory */
    gsl_matrix_free(D);
    gsl_matrix_free(Temp);
    gsl_matrix_free(T);
    gsl_vector_free(evec);
    gsl_matrix_free(y);
    gsl_vector_free(Z);

    return 0;

}
