#include "xively.h"
#include "xi_helpers.h"
#include "xi_err.h"
#include <RF24Network.h>
#include <RF24.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
Stuff for xively
*/
#define XI_FEED_ID YOUR_FEED_ID // set Xively Feed ID (numerical, no quoutes
#define XI_API_KEY "YOUR_API_KEY" // set Xively API key (double-quoted string) 

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);

// Network uses that radio
RF24Network network(radio);
// Address of our node
const uint16_t this_node = 0;

// Structure of our payload
struct payload_t
{
  uint16_t node_id;
  uint8_t d_temperature;
  uint8_t p_temperature;
};

void setup(void)
{
  printf("\n\rRF24/DallasNRFNet_rx/\n\r");
  // Setup and configure rf radio
  radio.begin();
  network.begin(/*channel*/90, /*node_address*/this_node);
}

int main(int argc, char** argv)
{
  setup();

  xi_feed_t feed;
  memset( &feed, 0, sizeof( xi_feed_t ));

  feed.feed_id = XI_FEED_ID;
  feed.datastream_count = 1;

  xi_context_t* xi_context
        = xi_create_context( XI_HTTP, XI_API_KEY, feed.feed_id );

  if( xi_context == NULL )
  {
    return -1;
  }

  xi_datapoint_t current_temp;
  xi_set_value_f32( &current_temp, 11.1 );
  { // get actual timestamp
    time_t timer = 0;
    time( &timer );
    current_temp.timestamp.timestamp = timer;
  }
  xi_datastream_update( xi_context, XI_FEED_ID, "mobile_temperature", &current_temp );

  while(1)
  {
    network.update();
    // if there is data ready
    while ( network.available() )
    {
      RF24NetworkHeader header;
      payload_t payload;
      network.read(header, &payload, sizeof(payload));
      printf("Received reading: ");
      printf("%d", payload.d_temperature);
      printf(".%d", payload.p_temperature);
      printf(" from node: ");
      printf("%d", payload.node_id);
      printf("\n");

      float d = payload.d_temperature + (float)payload.p_temperature / 100; //slight presumption that the sender is only sending 2 decimal places.
      xi_set_value_f32( &current_temp, d );
      
      { // get actual timestamp
        time_t timer = 0;
        time( &timer );
        current_temp.timestamp.timestamp = timer;
      }
      xi_datastream_update( xi_context, XI_FEED_ID, "mobile_temperature", &current_temp );
    }  
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
