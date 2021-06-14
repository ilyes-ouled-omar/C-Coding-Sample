/** @file
 *  @brief Main file.
 *  @description Just contains some example code. Adapt it in the way you like.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "drivers/adc_driver/adc_driver.h"
#include "drivers/error_led/error_led.h"
#include "drivers/torque_calculation/torque_calculation.h"

/*Create Angle array	Angle dddd for the Moving average*/
angle_value_t Angle_intervall[MEAN_AVERAGE_INTERVAL_LENGTH] = {25,20,15,20,20};

void INIT()
{
  error_led_init();
  /*Intialize both ADC_CHANNEL1 and ADC_CHANNEL2*/
  adc_init(ADC_CHANNEL0);
  adc_init(ADC_CHANNEL1);
}

int main(int argc, char *argv[]) {
  adc_value_t ADC1_value = 0;
  adc_value_t ADC2_value = 0;
  angle_value_t angle = 0;
  uint8_t speed = 0;
  torque_value_t torque_value = 0;

  /*Initialization*/
  INIT();
  
  /*For testing purposes, we will asign ADC1_value with 88 and ADC2 value with 71*/
  adc_read_set_output(ADC_CHANNEL0, 512, ADC_RET_OK);
  adc_read(ADC_CHANNEL0, &ADC1_value);
  adc_read_set_output(ADC_CHANNEL1, 532, ADC_RET_OK);
  adc_read(ADC_CHANNEL1, &ADC2_value);
  
  /*Calculate angle. If error Goto error*/
  angle = calculate_angle(ADC1_value, ADC2_value, Angle_intervall);
  if (angle < 0)
  {
    printf("error code = %d\n",angle);
    goto error;
  }
  torque_value = torque_calculation(angle, speed);
  
  if(NOT_IMPLEMENTED_ANGLE == torque_value)
  {
    printf("error code = %f\n",torque_value);
    goto error;
  }
  else
    printf("torque_value = %f\n",torque_value);
  /*Verify Angle_intervall is updated correctly*/
  //for (int i=0;i<5;i++)
	//{
		//printf("%d\n",Angle_intervall[i]);
	//}
  
  return 0;
  error:
    error_led_set(true);
    return -1;

}
