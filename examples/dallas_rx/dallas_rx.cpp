#include <RF24Network.h>
#include <RF24.h>
#include <cstdlib>
#include <iostream>

//
// Hardware configuration
//

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
  //
  // Setup and configure rf radio
  //
  radio.begin();
//  radio.setPALevel(RF24_PA_HIGH);
//  radio.setPayloadSize(8);
//  radio.printDetails();
  network.begin(/*channel*/90, /*node_address*/this_node);
}

int main(int argc, char** argv)
{
  setup();
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
    }  
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
