#include "led.h"


//LED ³õÊ¼»¯
void LED_InitRGB(void)
{
   // MicoGpioInitialize((mico_gpio_t)R_PIN,  OUTPUT_PUSH_PULL);
   // MicoGpioInitialize( (mico_gpio_t)G_PIN, OUTPUT_PUSH_PULL);
   // MicoGpioInitialize( (mico_gpio_t)B_PIN, OUTPUT_PUSH_PULL);
}

u8 LED_openRGB(u8 R_value, u8 G_value, u8 B_value)
{
    
    if ( R_value > 100 || G_value> 100 || B_value> 100)
    {
        printf("param error!\n\r");
        return 0;
    }
    
    MicoPwmInitialize( (mico_pwm_t)MICO_PWM_R, 50, (float)R_value);
    MicoPwmStart( (mico_pwm_t)MICO_PWM_R);
    
    MicoPwmInitialize( (mico_pwm_t)MICO_PWM_G, 50, (float)G_value);
    MicoPwmStart( (mico_pwm_t)MICO_PWM_G);
    
    MicoPwmInitialize( (mico_pwm_t)MICO_PWM_B, 50, (float)B_value);
    MicoPwmStart( (mico_pwm_t)MICO_PWM_B);
    
    return 1;
}

void LED_closeRGB(void)
{
    MicoPwmStop((mico_pwm_t)MICO_PWM_R);
    MicoPwmStop((mico_pwm_t)MICO_PWM_G);
    MicoPwmStop((mico_pwm_t)MICO_PWM_B);
}		   


#if 0 ////////////////////////////////////////////////////////
float constrain(float value, float min, float max){
  if(value >= max)
    return max;
  if(value <=min )
    return min;
  return value;
}

float Percent(float value){
  return value = (((float)value / 255.0) * 100.0);
}

void H2R_HSBtoRGB(float hue, float sat, float bright, float *color) {
		
  // constrain all input variables to expected range
  hue = constrain(hue, 0, 360);
  sat = constrain(sat, 0, 100);
  bright = constrain(bright, 0, 100);

  // define maximum value for RGB array elements
  float max_rgb_val = H2R_MAX_RGB_val;

  // convert saturation and brightness value to decimals and init r, g, b variables
  float sat_f = (float)sat / 100.0;
  float bright_f = (float)bright / 100.0;        
  float r, g, b;
    
  // If brightness is 0 then color is black (achromatic)
  // therefore, R, G and B values will all equal to 0
  if (bright <= 0) {      
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
  } 
	
  // If saturation is 0 then color is gray (achromatic)
  // therefore, R, G and B values will all equal the current brightness
  if (sat <= 0) {      
    color[0] = bright_f * max_rgb_val;
    color[1] = bright_f * max_rgb_val;
    color[2] = bright_f * max_rgb_val;
  } 
    
  // if saturation and brightness are greater than 0 then calculate 
  // R, G and B values based on the current hue and brightness
  else {  
    if (hue >= 0 && hue < 120) {
      float hue_primary = 1.0 - ((float)hue / 120.0);
      float hue_secondary = (float)hue / 120.0;
      float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
      float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
      float sat_tertiary = 1.0 - sat_f;
      r = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
      g = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
      b = (bright_f * max_rgb_val) * sat_tertiary;  
    }
    else if (hue >= 120 && hue < 240) {
      float hue_primary = 1.0 - (((float)hue-120.0) / 120.0);
      float hue_secondary = ((float)hue-120.0) / 120.0;
      float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
      float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
      float sat_tertiary = 1.0 - sat_f;
      r = (bright_f * max_rgb_val) * sat_tertiary;  
      g = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
      b = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
    }
    else if (hue >= 240 && hue <= 360) {
      float hue_primary = 1.0 - (((float)hue-240.0) / 120.0);
      float hue_secondary = ((float)hue-240.0) / 120.0;
      float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
      float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
      float sat_tertiary = 1.0 - sat_f;
      r = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
      g = (bright_f * max_rgb_val) * sat_tertiary;  
      b = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
    }
    
    color[0] = r;
    color[1] = g;
    color[2] = b;

  }
  color[0] = Percent(color[0]);
  color[1] = Percent(color[1]);
  color[2] = Percent(color[2]);

}
#endif

 
