#define TRIG RPI_V2_GPIO_P1_40  // GPIO21
#define ECHO_PIN RPI_V2_GPIO_P1_38  // GPIO20
#define LED_PIN RPI_V2_GPIO_P1_37  // GPIO26

void ultrasonic_init(void);
float ultrasonic_get_distance(void);
