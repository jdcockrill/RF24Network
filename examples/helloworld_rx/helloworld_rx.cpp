/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Simplest possible example of using RF24Network,
 *
 * RECEIVER NODE
 * Listens for messages from the transmitter and prints them out.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <cstdlib>
#include <iostream>

// nRF24L01(+) radio attached using Getting Started board 
//RF24 radio(9,10,8000000);
RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;

// Address of the other node
const uint16_t other_node = 1;

// Structure of our payload
struct payload_t
{
  unsigned long ms;
  unsigned long counter;
};

int main(int argc, char** argv)
{
  printf("RF24Network/examples/helloworld_rx/\n");
 
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  while(1)
  {
    // Pump the network regularly
    network.update();

    // Is there anything ready for us?
    while ( network.available() )
    {
      // If so, grab it and print it out
      RF24NetworkHeader header;
      payload_t payload;
      network.read(header,&payload,sizeof(payload));
      printf("Received packet #");
      printf("%lu",payload.counter);
      printf(" at ");
      printf("%lu",payload.ms);
      printf("\n");
    }
  }
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
