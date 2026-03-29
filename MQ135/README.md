# Using the MQ135 Gas sensor with MCP-ADC

![Circuit](circuit.jpg)

Circuit detail:

![Circuit detail](detail.jpg)

## Setup

The following picture shows the circuit connections:

```
                              _________
                             |         |
      Sensor-AO-Pin -  Pin 1 | •       | Pin 16  -  3.3V Pin
                       Pin 2 |         | Pin 15  -  3.3V Pin
                       Pin 3 |  MCP    | Pin 14  -  Ground
                       Pin 4 |  3008   | Pin 13  -  Ground
                       Pin 5 |         | Pin 12  -  MISO (GPIO9)
                       Pin 6 |         | Pin 11  -  MOSI (GPIO10)
                       Pin 7 |         | Pin 10  -  CE0 (GPIO8)
                       Pin 8 |_________| Pin 9   -  Ground

             _________
            |         | VCC -  3.3V Pin
            |  MQ     | GND -  Ground
            |  135    | DO
            |         | AO  -  PIN 1 Sensor
            |_________| 

```

You can find a lot of details and good explanations by visiting this page which
I found very useful to build the circuit:

[How to use MCP3008] (https://www.techcoil.com/blog/how-to-use-mcp3008-i-p-chip-to-help-your-raspberry-pi-read-digital-data-from-analog-sensors/)


## Compile and run

Install the bcm2835 library first and compile the source file with 'make'.
Run the sensor reading program as root by typing 'sudo ./mq135'.

## Try it out

The program will print the ADC value which can be used to compare
air quality in different situations. I recommend
combining this measurement with a MH-Z19 Sensor
to obtain the actual CO2 concentration. The contribution
of gases such as Benzene, alcohol or smoke can be estimated
from the ADC value.
Here, the expected output in a closed room shows the increase when
exhaled air is blown on the sensor:

```
ADC Value: 376
ADC Value: 376
ADC Value: 375
ADC Value: 375
ADC Value: 385
ADC Value: 400
ADC Value: 414
ADC Value: 422
ADC Value: 425
ADC Value: 422
^C
```
