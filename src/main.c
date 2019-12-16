
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
