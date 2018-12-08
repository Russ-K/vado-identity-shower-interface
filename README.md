# vado-identity-shower-interface

Arduino program to interact with the Vado Identity IDE-147A-C/P / SENSORI Identity IDE-147C-C/P

The projects herein assume the use of an arduino mega as it allows for communication with the vado unit while also being able to write to the serial monitor

## Projects

There are 3 projects in this repo:

* [pseudo_mixer](pseudo_mixer/README.md)
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

## Tools Used

* [Arduino Mega][arduino_mega]
* [Logic Analyser][logic_analyser]
* [Final RS-485 module][final_rs485]
* [Original RS-485 module][original_rs485]

  [arduino_mega]: https://store.arduino.cc/arduino-mega-2560-rev3
  [logic_analyser]: https://www.ebay.co.uk/sch/i.html?_nkw=24mhz+8+channel+usb+logic+analyzer+saleae+8ch+logic+analyzer
  [final_rs485]: https://www.amazon.co.uk/gp/product/B07B667STP
  [original_rs485]: https://www.amazon.co.uk/gp/product/B06XHHWLMW

## Other Useful Links

The discussion about how I decoded the comms protocol can be found at the [Arduino Forum][arduino_forum]

Useful information on the RS-485 communication standard - [Wikipedia][wiki_rs485]

How to set up an arduino as an oscilloscope (it's not perfect but it will definitely get you started) - [Instructables][instructables_link]

How to determine an unknown baud rate - [kumari.net][kumari]

Sigrok tool used to display / decode data from a logic analyser - [Sigrok][sigrok_link]

[arduino_forum]: https://forum.arduino.cc/index.php?topic=564171
[wiki_rs485]: https://en.wikipedia.org/wiki/RS-485
[instructables_link]: https://www.instructables.com/id/Oscilloscope-Arduino-Processing/
[kumari]: https://www.kumari.net/index.php/random/37-determing-unknown-baud-rate
[sigrok_link]: https://sigrok.org/wiki/Protocol_decoder:Uart
