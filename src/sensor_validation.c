#include "sensor_validation.h"
#include "externs.h"
#include "sensor_history.h"

int data_number;
int group_number;

/* chceks whether the sensor readings are within a range or not */
int reading_validation(double *values) {

    int flag = 0;

    for (int i = 0; i < data_number; i++) {
        if ((values[i] < temperatureMin) || (values[i] > temperatureMax)) {
            FILE *fp = fopen("../data/sensor_history/sensor_history.csv", "a");

            fprintf(fp, "Sensor%d value are out of range!\n", (i%sensor_number)+1);
            fclose(fp);
            flag++;
        }
    }
    if (flag == 0)
        return 1;
    else
        return 0;
}