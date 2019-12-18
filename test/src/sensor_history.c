/*This keeps track of sensor history and add it file for
 future reference*/
#include "sensor_history.h"
#include "externs.h"

/* function adds results to the file without truncating the previous results
* it holds data about list of stuck sensors and sensors having out-of-range values
*/
void sensor_history_add(FILE* fp, int stucked) {

    if ( fp != NULL) {
        if (stucked >0)
            fprintf(fp, "Sensor%d was stucked.\n", stucked);
    }

    printf("\nSensor history added in ../data/sensor_history/sensor_history.csv\n");
}

int sensor_history_del() {
    return 0;
}
