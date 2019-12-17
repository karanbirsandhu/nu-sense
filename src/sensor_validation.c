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

/* finding stuck sensors */
int frozen_value_check(double *values) {

    int stucked = 0;

    FILE *fp = fopen("../data/sensor_history/sensor_history.csv", "a");

    if (group_number > 3) {
        for (int i = 0; i < sensor_number; i++) {

            for (int j = 0; j < group_number - 3; j++) {

                if ((values[i+j*sensor_number] == values[i+(j+1)*sensor_number]) && (values[i+(j+1)*sensor_number] == values[i+(j+2)*sensor_number]) && (values[i+(j+2)*sensor_number] == values[i+(j+3)*sensor_number])) {

                    printf("\nSensor%d was stucked! Calculation will ignore this sensor value.\n", i+stucked+1);
                    sensor_history_add(fp, i+stucked+1);
                    stucked++;
                    eliminate_stuck_sensor(i, &values[0]);


                    break;
                }
            }
        }
    }

    if (stucked == 0) {
        sensor_history_add(fp, 0);
    }

    fclose(fp);

    return 0;
}
