# FreeRC

Welcome to the Arduino based remote flight controller FreeRC. This repo provides source code for an RC
transmitter (with an Arduino, PS4 controller, Radio transmitter module) and for a receiver 
(Arduino, Radio receiver module, Servos, ...).

<img src="https://user-images.githubusercontent.com/24190530/31602041-160d7a88-b25c-11e7-8fe2-89360db4aa30.JPG" width="50%" align="left" style="float: left">
<img src="https://user-images.githubusercontent.com/24190530/31602042-16490490-b25c-11e7-90cb-1749337c5cb8.JPG" width="50%" align="left" style="float: left">
<span style="width: 100%; clear: both"></span>

## Hardware Requirements

| Transmitter           |      | Receiver                      |      |
|-----------------------|------|-------------------------------|------|
| Arduino AtMega 2560   |  11€ | Arduino Nano                  |   5€ |
| USB Host Shield v2.0  |   8€ | 4 Servo Motors (e.g. SG90)    |   8€ |
| NRF24L01+ TX/RX       |   5€ | NRF24L01+ TX/RX               |   5€ |
| PS4 Controller        |  30€ | LiPo Accu                     |  20€ |
| Adafruit 2.8" TFT LCD |  13€ | Turnigy Plush 18A, or similar |  20€ |
| LiIon Powerbank       |  15€ | 3-Phase Brushless Motor       |  20€ |

Which makes a total of 160€ for the complete RC electronics. But you'll also need equipment for soldering and voltage and current measuring, a lots of wires, optionally an op-amp for plane voltage reading and an acc and/or gyro.
Also a GPS module could be implemented in long future terms.


## USB Host Shield modification

In order to use the LCDTFT Shield on top of the USB Host Shield rev 2.0, there are a few things to be done before. As both shields share the same pins 
(Digital 9 and 10) it make sense to change the USB Host shield pins. The tft shield occupies pins D5-D13, so the USB host shield can use e.g. D1 and D2.
Therefore I rewired the SS (original D10) to D2 and the INT (originally D9) to D1. There have to be a few changes in the UsbCore.h file in the USB Host Shield Library.
Go to line 43 and change it  to 
```c
typedef MAX3421e<P2, P1> MAX3421E; // Official Arduinos (UNO, Duemilanove, Mega ...
```
Now your Usb Host Shield will work as well, if you rewired it correctly.

<img src="https://user-images.githubusercontent.com/24190530/31602043-16850d96-b25c-11e7-8e06-8b6389cc83ee.JPG" width="300" style="float: left">

## Installation

The *_nano.ino file is the logic for the Arduino Nano and the *_mega.ino file is the source code
for controlling the Arduino AtMega 2560 (would work with some modifications with an Uno R3 as well, but there are too few pins for USB shield and Display). The pin configuration for
the NRF24L01 modules looks as follows

| Description | Nano | AtMega 2560 |
|-------------|------|-------------|
| VCC         | 5V   | 5V          |
| GND         | GND  | GND         |
| SCK         | 13   | 52          |
| MOSI        | 11   | 50          |
| MISO        | 12   | 51          |
| CE          | 7    | 53          |
| CSN         | 8    | 49          |

Notice that the CE and CSN pins require just a digital pin, it doesn't matter which ones you are choosing. The configuration
of an Uno R3 should be similar to the Nano one, but no guarantee for that.

The servo pin configuration is set to

| Motor | Roll-aileron left | Roll-aileron right | Pitch elevator | Side rudder |
| ---   | ---               | ---                | ---            | ---         |
| 2     | 3                 | 4                  | 5              | 6           |

# References

- http://shieldlist.org/adafruit/tft-2.8-touch-lcd
- http://shieldlist.org/circuitsathome/usbhost-v2
- https://github.com/felis/USB_Host_Shield_2.0#interface-modifications
- https://github.com/JoaoLopesF/SPFD5408
