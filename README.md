# Embedded Systems Firmware Stack     [![C/C++ CI](https://github.com/loay-ashraf/ATmega16-32-Library-Set/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/loay-ashraf/ESFS/actions/workflows/c-cpp.yml)
Firmware stack for embedded devices based on layered architecture with full integration of FreeRTOS kernel.

## Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
You'll need one of the following tools installed on your machine:
- Atmel Studio 7 - https://www.microchip.com/mplab/avr-support/atmel-studio-7
- Eclipse Embedded CDT with AVR plugin - https://eclipse-embed-cdt.github.io | http://avr-eclipse.sourceforge.net/wiki/index.php/Plugin_Download

You'll also need git to be installed on your machine.

### Installing
You'll need to do the following:
- clone the repo to your local machine using: `git clone https://github.com/loay-ashraf/ESFS.git`
- create new project in your IDE.
- add your project root directory to compiler include path (in project toolchain settings).
- add the three main folders from cloned repo folder to the project: `app`, `hal`, `rtos`. 
- scale down the layers according to the size of your application by removing unwanted modules (if you are using RTOS, modify the following file: `rtos_config.h`) - see [Project Contents](#projectcontents).
- remove unwanted modules simply by removing their folders in the rtos, ecu and mcu layer - see [Project Layout](#projectlayout).
- after removing unwanted modules from the project, rebuild the project to check that no errors are thrown by the preprocessor or the linker.

Notice: if using Eclipse IDE, you must add the following line in linker's "other arguments" field: `-Wl,--gc-sections`

## <a name="projectcontents"></a>Project Contents
The project contains the following software layers:
- RTOS layer
- Service Layer
- Hardware Abstraction Layer (HAL) which is then divided into: mcu layer and ecu layer

The RTOS layer contains:
- FreeRTOS kernel V10.2.1
- complmentary APIs like tasks, coroutines, queue, etc.

The Service layer contains:
- common functions like `map`
- common header files like `std_types.h`

The ecu layer contains:
- Character LCD API/driver - HD44780 controller
- Graphical LCD API/driver - ST7920 controller
- Keypad matrix API/driver
- RTCC API/driver - DS1307/DS3231 controller
- Servo API/driver

The mcu layer contains:
- softreset API - software reset using watchdog timer
- pwmin API - frequency and pulse width measurment
- dac API - PWM output using timers' fast pwm mode
- ac driver - analog comparator
- adc driver - analog digital converter
- gpio driver - digital I/O with external interrupts
- spi driver - serial peripheral interface
- timer driver - 8-bit timers TIMER0 and TIMER2
- timer16 driver - 16-bit timers TIMER1
- twi driver - two wire interface (I2C bus)
- usart driver - serial interface (USB,RS232)
- wdt driver - watchdog timer

## <a name="projectlayout"></a>Project Layout
```
└───ESFS
    ├───app
    │   ├───config
    │   └───examples
    │       ├───include
    │       └───source
    ├───hal
    │   ├───ecu
    │   │   ├───distance
    │   │   │   └───ultrasonic
    │   │   │       ├───api
    │   │   │       ├───common
    │   │   │       └───driver
    │   │   │           └───HC-SR04
    │   │   ├───glcd
    │   │   │   ├───api
    │   │   │   ├───common
    │   │   │   └───driver
    │   │   │       └───ST7920
    │   │   ├───keypad
    │   │   ├───lcd
    │   │   │   ├───api
    │   │   │   ├───common
    │   │   │   └───driver
    │   │   │       └───HD44780
    │   │   ├───rtcc
    │   │   │   ├───api
    │   │   │   ├───common
    │   │   │   └───driver
    │   │   │       └───DS3231
    │   │   └───servo
    │   └───mcu
    │       ├───boot
    │       ├───hw
    │       │   ├───api
    │       │   │   ├───dac
    │       │   │   ├───pwmin
    │       │   │   └───softreset
    │       │   └───driver
    │       │       ├───ac
    │       │       ├───adc
    │       │       ├───gpio
    │       │       ├───spi
    │       │       ├───timer
    │       │       ├───timer16
    │       │       ├───twi
    │       │       ├───usart
    │       │       └───wdt
    │       ├───io
    │       └───sys
    ├───rtos
    │   ├───config
    │   ├───include
    │   └───source
    └───service
        ├───include
        └───src
```
## Built With
- Atmel Studio 7 - The official AVR IDE (for the AVR parts of the code)
- Eclipse Embedded CDT - 2021 - 03
- avr-gcc V5.4 - GNU AVR C compiler
- Make - auto-generated makefile

## Tested With
ATmega16 running at 16MHZ (external crystal oscillator)

## Authors
Loay Ashraf - <loay.ashraf.96@gmail.com>

## License
This project is licensed under the MIT License - see the LICENSE.md file for details

## Acknowledgments
Credits goes to: 

The FreeRTOS project - https://www.freertos.org/
