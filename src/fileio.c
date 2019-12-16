#include "fileio.h"
#include "externs.h"

/* function reads data from the file for specific time interval
* @input : time interval, file name, group Values
* @output : input associated sensor data
*/

int read_data(double *timeinterval, double *group_values, char* input_file_name) {

    FILE *fp = fopen(input_file_name, "r");

    int data_number = 0;

    if (!fp) {
        printf("Can't open file\n");
        return -1;
    } else {
        char buf[1024];
        int row_count = 0;
        int field_count = 0;

        while (fgets(buf, 1024, fp)) {
            field_count = 0;
            row_count++;

            if (row_count == 1) {
                continue;
            }

            char *field = strtok(buf, ",");
            while (field) {
                if (field_count == 0) {
                    timeinterval[data_number] = atof(field);
                }
                if (field_count == 2) {
                    group_values[data_number] = atof(field);
                    data_number++;
                }
                field = strtok(NULL, ",");
                field_count++;
            }
        }
        if (data_number % sensor_number != 0) {
            return 0;
        }
    }
    return data_number;
}

/* functions writes fused data values to the file using
 time values and computed fused value and name of the file */
 
void write_data(float time_val_file, double fused, char* output_file_name) {
    FILE *fp = fopen(output_file_name, "a");
    if ( fp != NULL) {
        fprintf(fp, "%.1f,%f\n",time_val_file, fused);
        fclose(fp);
    } else {
        fclose(fp);
        FILE *fp = fopen(output_file_name, "w");
        fprintf(fp, "%.1f,%f\n", time_val_file,fused);
        fclose(fp);
    }
    printf("Fused output was added in %s\n\n", output_file_name);
}
