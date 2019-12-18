#ifndef SENSOR_VALIDATOIN_H
#define SENSOR_VALIDATOIN_H

#include <stdio.h>

int reading_validation(double *);
void frozen_value_check(double*); 
void eliminate_stuck_sensor(int, double*);

#endif