#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<bcm2835.h>

/*               GPIO-PIN       BCM2835-Name
   Temperatur       17        RPI_V2_GPIO_P1_11
   Light            27        RPI_V2_GPIO_P1_13
   Pressure         22        RPI_V2_GPIO_P1_15
 */

#define TEMP_PIN  RPI_V2_GPIO_P1_11
#define LIGHT_PIN RPI_V2_GPIO_P1_13
#define PRESS_PIN RPI_V2_GPIO_P1_15

#define MESS_DELAY 30

int TEMPERATURE_THRESHOLD = 37;
int PRESSURE_THRESHOLD = 1000;
int LIGHT_THRESHOLD = 560;

void temperatureSensor(){
  FILE *fp = fopen("/tmp/temperature", "w");
  int temperatureValue = rand() % 81;
  printf("%d  degC  ", temperatureValue);
  fflush(stdout);
  system("date");
  if(fp != NULL){
    fprintf(fp,"%d\n", temperatureValue);
    fclose(fp);
  }
  else{
    printf("Error opening temperature file\n");
  }
  if(temperatureValue > TEMPERATURE_THRESHOLD){
    printf("Temperatur set to HIGH\n");
    bcm2835_gpio_write(TEMP_PIN, HIGH);
  }
  else{
    printf("Temperatur set to LOW\n");
    bcm2835_gpio_write(TEMP_PIN, LOW);
  }
}

void lightSensor(){
  FILE *fp = fopen("/tmp/light", "w");
  int lightValue = rand() % 1001;
  printf("%d  lx  ", lightValue);
  fflush(stdout);
  system("date");
  if(fp != NULL){
    fprintf(fp,"%d\n", lightValue);
    fclose(fp);
  }
  else{
    printf("Error opening light file\n");
  }
  if(lightValue > LIGHT_THRESHOLD){
    printf("Light value set to HIGH\n");
    bcm2835_gpio_write(LIGHT_PIN, HIGH);
  }
  else{
    printf("Light value set to LOW\n");
    bcm2835_gpio_write(LIGHT_PIN, LOW);
  }

}

void pressureSensor(){
  FILE *fp = fopen("/tmp/pressure", "w");
  int pressureValue = 950 + rand() % 101;
  printf("%d  hPa  ", pressureValue);
  fflush(stdout);
  system("date");
  if(fp != NULL){
    fprintf(fp,"%d\n", pressureValue);
    fclose(fp);
  }
  else{
    printf("Error opening pressure file\n");
  }
  if(pressureValue > PRESSURE_THRESHOLD){
    printf("Pressure set to HIGH\n");
    bcm2835_gpio_write(PRESS_PIN, HIGH);
  }
  else{
    printf("Pressure set to LOW\n");
    bcm2835_gpio_write(PRESS_PIN, LOW);
  }
}

int main(){
  srand(time(NULL));
  if(!bcm2835_init()){
    printf("BCM2835 initialization failed.\n");
    return -1;
  }
  bcm2835_gpio_fsel(TEMP_PIN, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LIGHT_PIN, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(PRESS_PIN, BCM2835_GPIO_FSEL_OUTP);
  while(1){
    lightSensor();
    temperatureSensor();
    pressureSensor();
    sleep(MESS_DELAY);
  }
  bcm2835_close();
  return 0;
}
