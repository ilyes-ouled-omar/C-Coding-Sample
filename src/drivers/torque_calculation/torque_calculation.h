/** @file
 *  @brief Header file for a (dummy) Torque Calculation.
 */
#ifndef DRIVERS_TORQUE_CALCULATION_TORQUE_CALCULATION_H_
#define DRIVERS_TORQUE_CALCULATION_TORQUE_CALCULATION_H_

#include <stdint.h>
#define MEAN_AVERAGE_INTERVAL_LENGTH 5


/**
 * Error codes Enum
 **/
typedef enum {
  DIFFERENT_ANGLES = -1,
  NEGATIVE_ANGLE = -2,
  UNDEFINED_SPEED = -3,
  ADC_OUT_OF_RANGE = -4,
  NOT_IMPLEMENTED_ANGLE = -1000,
  TORQUE_CALCULATION_ERRORS_LAST
} torque_calculation_errors;

/** Type of angle values
 *  int8_t to have an equal angle value using both ADC1 and ADC2 channels + to be able to return error codes
 **/
typedef int8_t angle_value_t;

/** Type of torque values
 *  
 **/
typedef float torque_value_t;
/**
 * Calculate Moving average as defined in https://en.wikipedia.org/wiki/Moving_average
 * Inputs: Current angle -> float, previous MEAN_AVERAGE_INTERVAL_LENGTH values -> float[MEAN_AVERAGE_INTERVAL_LENGTH]
 * Return values: Mean average -> float
 * Comments: Array of previous Angle values should be updated each time the function is called and we have correct angle value : FIFO 
 **/
angle_value_t calculate_moving_average(angle_value_t Angle, angle_value_t Angle_intervall[]);

/**
 * Calculate the angle depending on the ADC values
 * Inputs: ADC1 value -> float, ADC2 value ->float, previous MEAN_AVERAGE_INTERVAL_LENGTH values of angles -> float[MEAN_AVERAGE_INTERVAL_LENGTH],
 * Return values : positive angle (float). If an error is encountered, a negative value is returned
 * Comments: At this stage, we will process angle values > 30, to be able to extend torque calculation without modifying the function
 **/
angle_value_t calculate_angle(float ADC1_value, float ADC2_value, angle_value_t Angle_intervall[]);

/**
 * Calculate Torque
 * Inputs: angle value -> float, speed value ->float,
 * Return values : float. If an error is encountered, a negative value is returned
 * Comments: ./doc/Pedalmap.png defines torque value depending on speed and angle
 * 			In our case, angle is in [0.0,30.0]. We may define torque equation depending on speed
 * 			speed = 50 -> torque = (30-(-30))/(30) * angle + (-30) = 2 * angle - 30
 * 			speed = 0 -> Let's assume that torque is a linear function within intervals [0,10], [10,20] and [10,30]
 * 						if angle in [0,10] -> torque = 18 /10 * angle = 1.8 * angle
 * 						if angle in [10,20] -> torque = (35-18)/10 * angle + 1 = 1.7 * angle + 1
 * 						if angle in [20,30] -> torque = (50-35)/10 * angle + 5 = 1.5 * angle + 5
 * 						else return error
 * 
 **/
torque_value_t torque_calculation(angle_value_t angle, uint8_t speed);

#endif  //  TORQUE_CALCULATION_TORQUE_CALCULATION_H
