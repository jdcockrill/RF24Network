# Network Layer for nRF24L01(+) radios

Based on the work of @stanleyseow and @maniacbug
https://github.com/stanleyseow/RF24
Please see the full documentation at http://maniacbug.github.com/RF24Network/index.html 

## Why did I fork?

Well, the intention was to be able to create a network of temperature sensors (Dallas DS18B20's) and to use a Raspberry Pi as a base-station that marshalled comms up to Xively.com.

Whilst it might've been easier just to create another example based on farconada's work (https://github.com/farconada/RF24Network), I rather fancied separating the RF24 driver library out from the network overlay provided by RF24Network and have them compile and work as separate libraries, so that's what I've tried to do. The Makefile within the project has a dependency on the librf24-bcm library being available (see https://github.com/jdcockrill/RF24), but otherwise compiles into a separate library.

## You said something about sensors, Xively, etc

Yes I did. In keeping with the original structure, see the 'examples' subdirectory. Within that you should find a couple of examples I've added. Specifically a ```dallas_rx``` example which is pretty much a re-hash of ```helloworld_rx``` with a slightly different message payload.

```xively_update``` then extends the theme to use the libxively library to push data to the Xively.com service (formerly COSM and Pachube). The library can be found here: https://github.com/xively/libxively/

Good luck. 
