#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<bcm2835.h>

/*               GPIO-PIN       BCM2835-Name
   Temperatur       17        RPI_V2_GPIO_P1_11
   Light            27        RPI_V2_GPIO_P1_13
   Pressure         22        RPI_V2_GPIO_P1_15
 */

#define TEMP_PIN  RPI_V2_GPIO_P1_11
#define LIGHT_PIN RPI_V2_GPIO_P1_13
#define PRESS_PIN RPI_V2_GPIO_P1_15

#define MESS_STEP 2

void check_value(uint8_t state, int value, char *sensorName, char *unit){
  if(state == HIGH) {
    printf("=========================================\n");
    printf("    Warning!                      \n");
    printf("=========================================\n");
    printf("%s above threshold\n", sensorName);
    printf("%d  %s  ", value, unit);
    fflush(stdout);
    system("date");
    printf("=========================================\n");
  }
  else if(state == LOW) {
    printf("%s below threshold\n", sensorName);
    fflush(stdout);
  }
  else {
    printf("Error reading pin\n");
    fflush(stdout);
  }
}

int readTemperatureSensor(){
  FILE *fp = fopen("/tmp/temperature", "r");
  int temperatureValue;
  if(fp != NULL){
    if(fscanf(fp,"%d", &temperatureValue) != 1){
      printf("Error reading temperature: no integer found\n");
      return -1;
    }
    fclose(fp);
    // printf("%d  degC  ", temperatureValue);
    // fflush(stdout);
    // system("date");
    return temperatureValue;
  }
  else{
    printf("Error opening temperature file\n");
    return -1;
  }
}

int readLightSensor(){
  FILE *fp = fopen("/tmp/light", "r");
  int lightValue;
  if(fp != NULL){
    if(fscanf(fp,"%d", &lightValue) != 1){
      printf("Error reading light value: no integer found\n");
      return -1;
    }
    fclose(fp);
    // printf("%d  lx  ", lightValue);
    // fflush(stdout);
    // system("date");
    return lightValue;
  }
  else{
    printf("Error opening light file\n");
    return -1;
  }
}

int readPressureSensor(){
  FILE *fp = fopen("/tmp/pressure", "r");
  int pressureValue;
  if(fp != NULL){
    if(fscanf(fp,"%d", &pressureValue) != 1){
      printf("Error reading pressure value: no integer found\n");
      return -1;
    }
    fclose(fp);
    // printf("%d  hPa  ", pressureValue);
    // fflush(stdout);
    // system("date");
    return pressureValue;
  }
  else{
    printf("Error opening pressure file\n");
    return -1;
  }
}

int main(){
  if(!bcm2835_init()){
    printf("BCM2835 initialization failed.\n");
    return -1;
  }
  bcm2835_gpio_fsel(TEMP_PIN, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(LIGHT_PIN, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(PRESS_PIN, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_set_pud(TEMP_PIN, BCM2835_GPIO_PUD_DOWN);
  bcm2835_gpio_set_pud(LIGHT_PIN, BCM2835_GPIO_PUD_DOWN);
  bcm2835_gpio_set_pud(PRESS_PIN, BCM2835_GPIO_PUD_DOWN);
  while(1){
    int temperatureValue = readTemperatureSensor();
    int lightValue = readLightSensor();
    int pressureValue = readPressureSensor();
    uint8_t state = bcm2835_gpio_lev(TEMP_PIN);
    check_value(state, temperatureValue, "Temperature", "degC");
    state = bcm2835_gpio_lev(LIGHT_PIN);
    check_value(state, lightValue, "Light", "lx");
    state = bcm2835_gpio_lev(PRESS_PIN);
    check_value(state, pressureValue, "Pressure", "hPa");
    sleep(MESS_STEP);
  }
  bcm2835_close();
  return 0;
}
