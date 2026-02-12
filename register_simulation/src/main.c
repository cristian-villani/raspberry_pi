#include<stdio.h>
#include<stdlib.h>

int main(){
  
  int sensorValue;
  FILE *fp = fopen("/tmp/sensor", "a+");
  if (fp == NULL){
    printf("Error opening the file /tmp/sensor\n");
    return -1;
  }
  if(fscanf(fp,"%d", &sensorValue) != 1){
    printf("Error reading sensor data: value is not an integer\n");
    return -1;
  }
  printf("Sensor value read: %d\n", sensorValue);
  if(sensorValue > 50){
    printf("LED ON\n");
  }
  else{
    printf("LED OFF\n");
  }

  fclose(fp);
  return 0;
}
