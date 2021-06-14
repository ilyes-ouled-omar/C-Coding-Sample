/** @file
 *  @brief Source file for a (dummy) Torque Calculation.
 */

#include "torque_calculation.h"
#include <stdio.h>


angle_value_t calculate_moving_average(angle_value_t Angle, angle_value_t Angle_intervall[])
{
    uint8_t i = 0;
    angle_value_t mean_average = Angle;
    for (i=0;i<MEAN_AVERAGE_INTERVAL_LENGTH; i++)
    {
	    mean_average += (Angle_intervall[i]);
	    if(i>0)
	        (Angle_intervall[i-1]) = (Angle_intervall[i]);
	}
	Angle_intervall[MEAN_AVERAGE_INTERVAL_LENGTH-1] = Angle;
	return ((angle_value_t)(mean_average / (MEAN_AVERAGE_INTERVAL_LENGTH+1)));
}

angle_value_t calculate_angle(float ADC1_value, float ADC2_value, angle_value_t Angle_intervall[])
{
    angle_value_t angle1 = 0;
    angle_value_t angle2 = 0;
    if (ADC1_value < 0 || ADC2_value < 0 || ADC1_value > 1023 || ADC2_value > 1023)
        return ADC_OUT_OF_RANGE;
    angle1 = (angle_value_t)((((ADC1_value/1023)*5) - 0.5)/0.1); //ADC values between [0,5v] -> [0,1023]
    printf("angle1 = %d\n", angle1);
    angle2 = (angle_value_t)((((ADC2_value/1023)*5) -1.0)/0.08); //ADC values between [0,5v] -> [0,1023]
    printf("angle2 = %d\n", angle2);
    if ((angle1 == angle2) && (angle1 >= 0))
	    return (calculate_moving_average(angle1, Angle_intervall));  
	else if(angle1 != angle2)
	    return DIFFERENT_ANGLES;
	else
	    return NEGATIVE_ANGLE;
}

torque_value_t torque_calculation(angle_value_t angle, uint8_t speed)
{
    if ((speed != 0) && (speed != 50))
        return UNDEFINED_SPEED;
    if (50 == speed)
    {
        if (angle <= 30)
            return (2 * angle - 30);
        else //Out of the scope
            return NOT_IMPLEMENTED_ANGLE;
	}
    else
    {
	    if (angle <=10)
	        return (1.8 * angle);
	    else if (angle <=20)
	        return (1.7 * angle + 1);
	    else if (angle <=30)
	        return (1.5 * angle + 5);
	    else
	        return NOT_IMPLEMENTED_ANGLE;
	}
    
}
