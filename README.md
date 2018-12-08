# vado-identity-shower-interface

Arduino program to interact with the Vado Identity IDE-147A-C/P / SENSORI Identity IDE-147C-C/P

## Projects

There are 3 projects in this repo:

* psuedo_mixer
* rs485_read
* simple_comms

The main one I'll be maintaining is the psuedo_mixer project as that is the one that I need but I've included the rest of them in case they're useful to someone else.

### psuedo_mixer

This is the main project that replicates the functionality of the vado mixer.
It allows the Vado controller to be connected to an arduino for using custom hardware to act as the mixer.

### rs485_reader

This is a very simple project for working with, and simply reading data from, rs 485 modules.

### simple_comms

This is a simple project that transmits the handshake message that the Vado mixer would at the appropriate times and then reads the response from the controller and outputs the raw data to the serial monitor.

## Useful Links

The discussion about how I decoded the comms protocol can be found here - https://forum.arduino.cc/index.php?topic=564171

Useful information on the RS-485 communication standard - https://en.wikipedia.org/wiki/RS-485

How to set up an arduino as an oscilloscope (it's not perfect but it will definitely get you started) - https://www.instructables.com/id/Oscilloscope-Arduino-Processing/

How to determine an unknown baud rate - https://www.kumari.net/index.php/random/37-determing-unknown-baud-rate

Sigrok tool used to display / decode data from a logic analyser - https://sigrok.org/wiki/Protocol_decoder:Uart
