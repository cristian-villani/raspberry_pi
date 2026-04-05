#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>

#define uint8_t char

#define BME280_ADDR 0x76  // I2C address of the BME280 sensor

// Calibration constants
uint16_t T1;
int16_t T2, T3;
uint8_t H1, H3;
int16_t H2, H4, H5;
uint8_t H6;
uint16_t P1;
int16_t P2, P3, P4, P5, P6, P7, P8, P9;

// Global temperature fine variable for compensation
int32_t t_fine;

// CO2-read-command
unsigned char co2_command[] = {0xFF, 0x01, 0x86, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x79};

// Function to read the raw temperature data (3 bytes)
void read_raw_temperature(uint8_t *buf) {
  uint8_t reg = 0xFA;  // Register to read raw temperature data
  // Read 3 bytes of raw data
  int ret = bcm2835_i2c_read_register_rs(&reg, buf, 3);
  if (ret != BCM2835_I2C_REASON_OK) {
    printf("I2C read failed, error code: %d\n", ret);
    exit(1);
  }
  // printf("Raw Temperature Data: 0x%02X 0x%02X 0x%02X\n",
  //        buf[0], buf[1], buf[2]);
}

// Function to combine the 3 bytes into a single 20-bit raw temperature value
int32_t combine_raw_temperature(uint8_t *buf) {
  if (buf == NULL) {
    printf("Error: Null buffer passed to combine_raw_temperature\n");
    exit(1);  // Exit if buffer is NULL
  }

  // Combine the 3 bytes to form a 20-bit temperature value
  int32_t raw_temp = (buf[0]  << 12) | (buf[1] << 4) | (buf[2] >> 4);
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

// Function to read the raw pressure data (3 bytes)
void read_raw_pressure(uint8_t *buf) {
  uint8_t reg = 0xF7;  // Register to read raw temperature data
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
int32_t combine_raw_pressure(uint8_t *buf) {
  if (buf == NULL) {
    printf("Error: Null buffer passed to combine_raw_temperature\n");
    exit(1);  // Exit if buffer is NULL
  }

  // Combine the 3 bytes to form a 20-bit temperature value
  int32_t raw_pressure = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);
  return raw_pressure;
}

// Calibration constants
// uint16_t T1, T2, T3;                    // Temperature calibration
// uint16_t P1                             // Pressure calibration
// int16_t P2, P3, P4, P5, P6, P7, P8, P9  // Pressure calibration
// int16_t H2, H4, H5, H6;                 // Humidity calibration
// uint8_t H1, H3;                         // Humidity calibration

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

  // Pressure
  P1 = (uint16_t)(calib1[7] << 8 | calib1[6]);
  P2 = (int16_t)(calib1[9] << 8 | calib1[8]);
  P3 = (int16_t)(calib1[11] << 8 | calib1[10]);
  P4 = (int16_t)(calib1[13] << 8 | calib1[12]);
  P5 = (int16_t)(calib1[15] << 8 | calib1[14]);
  P6 = (int16_t)(calib1[17] << 8 | calib1[16]);
  P7 = (int16_t)(calib1[19] << 8 | calib1[18]);
  P8 = (int16_t)(calib1[21] << 8 | calib1[20]);
  P9 = (int16_t)(calib1[23] << 8 | calib1[22]);

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

// Adapted from the sensor datasheet documentation
// Returns pressure in Pa as double. Example: 96386.2 (Pa) = 963.862 hPa
double bme280_compensate_P_double(int32_t adc_P)
{
  double var1, var2, p;
  var1 = ((double)t_fine/2.0) - 64000.0;
  var2 = var1 * var1 * ((double)P6) / 32768.0;
  var2 = var2 + var1 * ((double)P5) * 2.0;
  var2 = (var2/4.0)+(((double)P4) * 65536.0);
  var1 = (((double)P3) * var1 * var1 / 524288.0 +
         ((double)P2) * var1) / 524288.0;
  var1 = (1.0 + var1 / 32768.0)*((double)P1);
  if (var1 == 0.0) {
   return 0; // avoid exception caused by division by zero
  }
  p = 1048576.0 - (double)adc_P;
  p = (p - (var2 / 4096.0)) * 6250.0 / var1;
  var1 = ((double)P9) * p * p / 2147483648.0;
  var2 = p * ((double)P8) / 32768.0;
  p = p + (var1 + var2 + ((double)P7)) / 16.0;
  return p;
}


// Function which opens the serial ports and set up parameters
int open_serial_port(const char *device) {
  int fd = open(device, O_RDWR | O_NOCTTY);
  if(fd < 0) {
    perror("Unable to open serial port");
    return -1;
  }

  struct termios options;
  tcgetattr(fd, &options);

  cfsetispeed(&options, B9600);
  cfsetospeed(&options, B9600);

  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_oflag &= ~OPOST;

  tcsetattr(fd, TCSANOW, &options);

  return fd;
}

// function which reads the CO2-value
int read_co2(int fd, unsigned char *buffer, int buffer_size){
  int n = write(fd, co2_command, sizeof(co2_command));
  if(n < 0){
    perror("Write failed");
    return -1;
  }

  usleep(1000000); // wait 1 second

  n = read(fd, buffer, buffer_size);
  if(n < 0){
    perror("Read failed");
    return -1;
  }

  return n;
}

// function to extract the CO2 value
unsigned int parse_co2(unsigned char *response){
  unsigned int co2 = (response[2] << 8) | response[3];
  return co2;
}

uint16_t read_mcp3008(uint8_t channel) {
  char buf[3];

  buf[0] = 0x01; // Start bit
  buf[1] = (0x08 | channel) << 4;
  buf[2] = 0x00;

  bcm2835_spi_transfern(buf, 3);

  uint16_t value = ((buf[1] & 0x03) << 8) | buf[2];
  return value;
}

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
  printf("I2C initialized, slave address set to 0x%X\n", BME280_ADDR);
}


int main(){
  uint8_t raw_data[3];  // Buffer to store raw temperature data
  uint8_t raw_hum_data[2];  // Buffer to store raw humidity data

  int fd = open_serial_port("/dev/serial0");
  if(fd == -1) {
    return 1;
  }

  int mCount = 0;
  unsigned char buffer[9]; // The sensor answer has 9 Bytes


  char buffer2[100];
  int fd3;
  FILE *file_ptr;

  fd3 = open("results.txt", O_RDWR);
  if(fd3 < 0){
    perror("Failed to open result file");
    return 1;
  }
  file_ptr = fdopen(fd3, "a");
  if(file_ptr == NULL) {
    perror("Failed to convert file descriptor to FILE*");
    close(fd3);
    return 1;
  }

  init_i2c();
  read_calibration();
  uint8_t ctrl_hum[2]  = {0xF2, 0x01};  // humidity oversampling x1
  uint8_t ctrl_meas[2] = {0xF4, 0x27};  // temp x1, press x1, normal mode
  uint8_t config[2]    = {0xF5, 0xA0};  // standby 1000ms 
  
  bcm2835_i2c_write(ctrl_hum, 2);
  bcm2835_i2c_write(ctrl_meas, 2);
  bcm2835_i2c_write(config, 2);

  // Initialize the BCM2835-Bibliothek and SPI
  // if(!bcm2835_init()){
  //   printf("BCM2835-Bibliothek konnte nicht initialisiert werden!\n");
  //   return 1;
  // }

  // Initialize SPI
  bcm2835_spi_begin();
  bcm2835_spi_set_speed_hz(1000000);  // SPI speed set to 1 MHz
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);  // Mode0 (CPOL=0, CPHA=0)

  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

  signal(SIGINT, handleSigint);

  while(1){
    mCount++;
    time_t start_time = time(NULL);
    struct tm *tm_info = localtime(&start_time);

    char time_buffer[26];
    strftime(time_buffer,sizeof(time_buffer),"%Y-%m-%d %H:%M:%S", tm_info);

    int len = read_co2(fd, buffer, sizeof(buffer));
    if(len < 0){
      perror("CO2 read failed");
      continue;
    }

    unsigned int co2 = parse_co2(buffer);
    fprintf(file_ptr, "-----------------------------------------------------\n");
    fprintf(file_ptr, "%d. c(CO2): %u ppm %s\n", mCount, co2, time_buffer);
    fflush(file_ptr);
    sleep(1);

    // Read raw temperature data
    read_raw_temperature(raw_data);

    // Combine the 3 bytes into a single 20-bit raw temperature value
    int32_t raw_temp = combine_raw_temperature(raw_data);
    float temp_value = convert_to_celsius(raw_temp);
    printf("Temperature: %.2f°C\n", temp_value);
  
    // Read raw humidity data
    uint16_t raw_hum = read_raw_humidity();
    // Print raw humidity
    // printf("Raw Humidity: 0x%02X 0x%02X\n", raw_hum >> 8, raw_hum & 0xFF);

    // Compensate humidity
    double humidity = bme280_compensate_H_double(raw_hum);
    printf("Humidity: %.2f%%\n", humidity);
  
    read_raw_pressure(raw_data);
    int32_t raw_pressure = combine_raw_pressure(raw_data);
    double pressure = bme280_compensate_P_double(raw_pressure)/100.0;
    printf("Pressure: %.2f hPa\n", pressure);
  
    fprintf(file_ptr, "Temperature: %.2f°C\n", temp_value);
    fprintf(file_ptr, "Humidity: %.2f%%\n", humidity);
    fprintf(file_ptr, "Pressure: %.2f hPa\n", pressure);
    // fprintf(file_ptr, "%.2f  %.2f %.2f\n", temp_value, humidity, pressure);

    uint16_t mq135_value = read_mcp3008(0); // CH0
    uint16_t mq7_value = read_mcp3008(1);   // CH1
    printf("MQ135 Value: %d\n", mq135_value);
    printf("MQ7 Value: %d\n", mq7_value);
    fflush(stdout);
    fprintf(file_ptr, "MQ135 Value: %d\n", mq135_value);
    fprintf(file_ptr, "MQ7 Value: %d\n", mq7_value);
    fflush(file_ptr);
    bcm2835_delay(5000);
  }
  bcm2835_spi_end();
  bcm2835_close();
  fclose(file_ptr);
  close(fd);
  return 0;
}
