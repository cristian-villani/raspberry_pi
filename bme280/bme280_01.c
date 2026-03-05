#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define BME280_ADDR 0x76  // I2C address of the BME280 sensor

// Calibration constants
uint16_t T1;
int16_t T2, T3;
uint8_t H1, H3;
int16_t H2, H4, H5;
uint8_t H6;

// Global temperature fine variable for compensation
int32_t t_fine;

void handleSigint(int sig){
  printf("\nCaught Ctrl+C, cleaning up...\n");
  bcm2835_i2c_end();
  bcm2835_close();
  bcm2835_delay(10);
  sleep(3);
  exit(0);
}

// Function to initialize I2C communication
void init_i2c() {
  if(!bcm2835_init()) {
    printf("bcm2835 initialization failed. Exiting...\n");
    exit(1);
  }
  if(!bcm2835_i2c_begin()) {
    printf("I2C communication failed. Exiting...\n");
    bcm2835_close();
    exit(1);
  }
  bcm2835_i2c_setSlaveAddress(BME280_ADDR);
  bcm2835_i2c_set_baudrate(100000);  // Standard 100kHz baud rate
  bcm2835_gpio_fsel(21, BCM2835_GPIO_FSEL_OUTP);
  printf("I2C initialized, slave address set to 0x%X\n", BME280_ADDR);
}

// Function to read the raw temperature data (3 bytes)
void read_raw_temperature(uint8_t *buf) {
  uint8_t reg = 0xFA;  // Register to read raw temperature data
  // Read 3 bytes of raw data
  int ret = bcm2835_i2c_read_register_rs(&reg, buf, 3);
  if (ret != BCM2835_I2C_REASON_OK) {
    printf("I2C read failed, error code: %d\n", ret);
    exit(1);
  }
  printf("Raw Temperature Data: 0x%02X 0x%02X 0x%02X\n",
         buf[0], buf[1], buf[2]);
}

// Function to combine the 3 bytes into a single 20-bit raw temperature value
int32_t combine_raw_temperature(uint8_t *buf) {
  if (buf == NULL) {
    printf("Error: Null buffer passed to combine_raw_temperature\n");
    exit(1);  // Exit if buffer is NULL
  }

  // Combine the 3 bytes to form a 20-bit temperature value
  int32_t raw_temp = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);
  return raw_temp;
}

// Function to compensate the raw temperature value to Celsius
float convert_to_celsius(int32_t raw_temp) {
  // Calculate var1 and var2 using the raw temperature and calibration constants
  int32_t var1 = ((raw_temp >> 3) - ((int32_t)T1 << 1)) * (int32_t)T2;
  var1 = var1 >> 11;

  int32_t var2 = ((raw_temp >> 4) - (int32_t)T1);
  var2 = var2 * var2;
  var2 = var2 >> 12;
  var2 = var2 * (int32_t)T3;
  var2 = var2 >> 14;

  // Combine var1 and var2 to get temp_fine
  t_fine = var1 + var2;

  // Convert to Celsius (scaled by 100 for better precision)
  float temperature = (t_fine * 5 + 128) >> 8;
  return (float)temperature / 100.0;  // Return temperature in degrees Celsius
}

uint16_t read_raw_humidity() {
  uint8_t humidity_data[2];

  // Read both MSB and LSB in one go
  // Read 2 bytes (MSB and LSB)
  bcm2835_i2c_read_register_rs((uint8_t[]){0xFD}, humidity_data, 2);

  // Combine MSB and LSB
  uint16_t raw_hum = (humidity_data[0] << 8) | humidity_data[1];
  return raw_hum;
}


// Calibration constants
// uint16_t T1, T2, T3;  // Temperature calibration
// int16_t H2, H4, H5, H6;  // Humidity calibration
// uint8_t H1, H3;  // Humidity calibration (H1 and H3 are byte values)

void read_calibration()
{
  uint8_t calib1[26];
  uint8_t calib2[7];

  uint8_t reg1 = 0x88;
  bcm2835_i2c_read_register_rs(&reg1, calib1, 26);

  uint8_t reg2 = 0xE1;
  bcm2835_i2c_read_register_rs(&reg2, calib2, 7);

  // Temperature
  T1 = (uint16_t)(calib1[1] << 8 | calib1[0]);
  T2 = (int16_t)(calib1[3] << 8 | calib1[2]);
  T3 = (int16_t)(calib1[5] << 8 | calib1[4]);

  // Humidity
  H1 = calib1[25];

  H2 = (int16_t)(calib2[1] << 8 | calib2[0]);
  H3 = calib2[2];
  H4 = (int16_t)((calib2[3] << 4) | (calib2[4] & 0x0F));
  if (H4 & 0x800) H4 |= 0xF000;   // sign extend 12-bit
  H5 = (int16_t)((calib2[5] << 4) | (calib2[4] >> 4));
  if (H5 & 0x800) H5 |= 0xF000;   // sign extend 12-bit
  H6 = (int8_t)calib2[6];
}

// Adapted from the sensor datasheet documentation
// Returns humidity in %rH as as double
double bme280_compensate_H_double(int32_t adc_H)
{
  double var_H;
  var_H = (((double)t_fine) - 76800.0);
  var_H = (adc_H - (((double)H4) * 64.0 + ((double)H5) / 16384.0 * var_H)) * 
          (((double)H2) / 65536.0 * (1.0 + ((double)H6) / 67108864.0 * var_H *
          (1.0 + ((double)H3) / 67108864.0 * var_H)));
  var_H = var_H * (1.0 - ((double)H1) * var_H / 524288.0);
  if (var_H > 100.0)
    var_H = 100.0;
  else if (var_H < 0.0)
    var_H = 0.0;
  return var_H;
}

int main() {
  uint8_t raw_data[3];  // Buffer to store raw temperature data
  uint8_t raw_hum_data[2];  // Buffer to store raw humidity data

  init_i2c();  // Initialize I2C communication

  // Read calibration constants
  read_calibration();
  printf("T1=%u\n", T1);
  printf("T2=%d\n", T2);
  printf("T3=%d\n", T3);
  printf("H1=%u\n", H1);
  printf("H2=%d\n", H2);
  printf("H3=%u\n", H3);
  printf("H4=%d\n", H4);
  printf("H5=%d\n", H5);
  printf("H6=%d\n", H6);
  uint8_t ctrl_hum[2]  = {0xF2, 0x01};  // humidity oversampling x1
  uint8_t ctrl_meas[2] = {0xF4, 0x27};  // temp x1, press x1, normal mode
  uint8_t config[2]    = {0xF5, 0xA0};  // standby 1000ms

  bcm2835_i2c_write(ctrl_hum, 2);
  bcm2835_i2c_write(ctrl_meas, 2);
  bcm2835_i2c_write(config, 2);

  signal(SIGINT, handleSigint);
  FILE *fp=fopen("results.txt","w");
  if(fp == NULL){
    printf("Error opening results file\n");
    return -1;
  }
  while(1) {
    // Read raw temperature data
    read_raw_temperature(raw_data);

    // Combine the 3 bytes into a single 20-bit raw temperature value
    int32_t raw_temp = combine_raw_temperature(raw_data);
    float temp_value = convert_to_celsius(raw_temp);
    printf("Temperature: %.2f°C\n", temp_value);

    // Read raw humidity data
    uint16_t raw_hum = read_raw_humidity();
    // Print raw humidity
    printf("Raw Humidity: 0x%02X 0x%02X\n", raw_hum >> 8, raw_hum & 0xFF);

    // Compensate humidity
    double humidity = bme280_compensate_H_double(raw_hum);
    printf("Humidity: %.2f%%\n", humidity);

    fprintf(fp, "%.2f  %.2f\n", temp_value, humidity);
    if(humidity > 60.0){
      bcm2835_gpio_write(21, HIGH);
    }
    sleep(5);  // Wait for 5 seconds before the next reading
    bcm2835_gpio_write(21, LOW);
  }

  bcm2835_i2c_end();  // End I2C communication
  bcm2835_close();  // Close the bcm2835 library

  return 0;
}
