// Wokwi Custom Chip - For docs and examples see:
// https://docs.wokwi.com/chips-api/getting-started
//
// SPDX-License-Identifier: MIT
// Copyright 2023 Angel Velasquez

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// LEN calculation macro
#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0])) 

// Microsecond Timer Rate
#define SECONDS 1000000

// Sample GPS Data in NMEA Format
const char gps_tx_data[][80] = {
    "$GPGGA,172914.049,2327.985,S,05150.410,W,1,12,1.0,0.0,M,0.0,M,,*60\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172914.049,A,2327.985,S,05150.410,W,009.7,025.9,060622,000.0,W*74\r\n",
    "$GPGGA,172915.049,2327.982,S,05150.409,W,1,12,1.0,0.0,M,0.0,M,,*6E\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172915.049,A,2327.982,S,05150.409,W,009.7,025.9,060622,000.0,W*7A\r\n",
    "$GPGGA,172916.049,2327.980,S,05150.408,W,1,12,1.0,0.0,M,0.0,M,,*6E\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172916.049,A,2327.980,S,05150.408,W,009.7,025.9,060622,000.0,W*7A\r\n",
    "$GPGGA,172917.049,2327.977,S,05150.406,W,1,12,1.0,0.0,M,0.0,M,,*69\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172917.049,A,2327.977,S,05150.406,W,009.7,025.9,060622,000.0,W*7D\r\n",
    "$GPGGA,172918.049,2327.975,S,05150.405,W,1,12,1.0,0.0,M,0.0,M,,*67\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172918.049,A,2327.975,S,05150.405,W,009.7,025.9,060622,000.0,W*73\r\n",
    "$GPGGA,172919.049,2327.973,S,05150.404,W,1,12,1.0,0.0,M,0.0,M,,*61\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172919.049,A,2327.973,S,05150.404,W,009.7,025.9,060622,000.0,W*75\r\n",
    "$GPGGA,172920.049,2327.970,S,05150.403,W,1,12,1.0,0.0,M,0.0,M,,*6F\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172920.049,A,2327.970,S,05150.403,W,009.7,025.9,060622,000.0,W*7B\r\n",
    "$GPGGA,172921.049,2327.968,S,05150.402,W,1,12,1.0,0.0,M,0.0,M,,*66\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172921.049,A,2327.968,S,05150.402,W,009.7,025.9,060622,000.0,W*72\r\n",
    "$GPGGA,172922.049,2327.965,S,05150.401,W,1,12,1.0,0.0,M,0.0,M,,*6B\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172922.049,A,2327.965,S,05150.401,W,009.7,025.9,060622,000.0,W*7F\r\n",
    "$GPGGA,172923.049,2327.963,S,05150.399,W,1,12,1.0,0.0,M,0.0,M,,*6A\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172923.049,A,2327.963,S,05150.399,W,009.7,025.9,060622,000.0,W*7E\r\n",
    "$GPGGA,172924.049,2327.960,S,05150.398,W,1,12,1.0,0.0,M,0.0,M,,*6F\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172924.049,A,2327.960,S,05150.398,W,009.7,294.1,060622,000.0,W*7B\r\n",
    "$GPGGA,172925.049,2327.959,S,05150.401,W,1,12,1.0,0.0,M,0.0,M,,*63\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172925.049,A,2327.959,S,05150.401,W,009.7,294.1,060622,000.0,W*77\r\n",
    "$GPGGA,172926.049,2327.958,S,05150.403,W,1,12,1.0,0.0,M,0.0,M,,*63\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172926.049,A,2327.958,S,05150.403,W,009.7,294.1,060622,000.0,W*77\r\n",
    "$GPGGA,172927.049,2327.957,S,05150.406,W,1,12,1.0,0.0,M,0.0,M,,*68\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172927.049,A,2327.957,S,05150.406,W,009.7,205.5,060622,000.0,W*70\r\n",
    "$GPGGA,172928.049,2327.959,S,05150.407,W,1,12,1.0,0.0,M,0.0,M,,*68\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172928.049,A,2327.959,S,05150.407,W,009.7,205.5,060622,000.0,W*70\r\n",
    "$GPGGA,172929.049,2327.962,S,05150.408,W,1,12,1.0,0.0,M,0.0,M,,*6E\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172929.049,A,2327.962,S,05150.408,W,009.7,205.5,060622,000.0,W*76\r\n",
    "$GPGGA,172930.049,2327.964,S,05150.410,W,1,12,1.0,0.0,M,0.0,M,,*69\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172930.049,A,2327.964,S,05150.410,W,009.7,205.5,060622,000.0,W*71\r\n",
    "$GPGGA,172931.049,2327.967,S,05150.411,W,1,12,1.0,0.0,M,0.0,M,,*6A\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172931.049,A,2327.967,S,05150.411,W,009.7,205.5,060622,000.0,W*72\r\n",
    "$GPGGA,172932.049,2327.969,S,05150.412,W,1,12,1.0,0.0,M,0.0,M,,*64\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172932.049,A,2327.969,S,05150.412,W,009.7,205.5,060622,000.0,W*7C\r\n",
    "$GPGGA,172933.049,2327.971,S,05150.413,W,1,12,1.0,0.0,M,0.0,M,,*6D\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172933.049,A,2327.971,S,05150.413,W,009.7,205.5,060622,000.0,W*75\r\n",
    "$GPGGA,172934.049,2327.974,S,05150.414,W,1,12,1.0,0.0,M,0.0,M,,*68\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172934.049,A,2327.974,S,05150.414,W,009.7,205.5,060622,000.0,W*70\r\n",
    "$GPGGA,172935.049,2327.976,S,05150.415,W,1,12,1.0,0.0,M,0.0,M,,*6A\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172935.049,A,2327.976,S,05150.415,W,009.7,205.5,060622,000.0,W*72\r\n",
    "$GPGGA,172936.049,2327.979,S,05150.417,W,1,12,1.0,0.0,M,0.0,M,,*64\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172936.049,A,2327.979,S,05150.417,W,009.7,205.5,060622,000.0,W*7C\r\n",
    "$GPGGA,172937.049,2327.981,S,05150.418,W,1,12,1.0,0.0,M,0.0,M,,*6D\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172937.049,A,2327.981,S,05150.418,W,009.7,117.1,060622,000.0,W*71\r\n",
    "$GPGGA,172938.049,2327.983,S,05150.415,W,1,12,1.0,0.0,M,0.0,M,,*6D\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172938.049,A,2327.983,S,05150.415,W,009.7,117.1,060622,000.0,W*71\r\n",
    "$GPGGA,172939.049,2327.984,S,05150.413,W,1,12,1.0,0.0,M,0.0,M,,*6D\r\n",
    "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n",
    "$GPRMC,172939.049,A,2327.984,S,05150.413,W,009.7,117.1,060622,000.0,W*71\r\n",
};

// Payload
typedef struct {
  uart_dev_t  uart0;
  uint32_t    gps_tx_index;
} chip_state_t;

// Event Handler Pre-declare
static void chip_timer_event (void *user_data);

// Custom Chip Initialization
void chip_init(void) {

  // Disable Debug Log Buffering
  setvbuf(stdout, NULL, _IOLBF, 1024);

  // Allocate space for Payload
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  const uart_config_t uart_config = {
    .tx         = pin_init("TX", INPUT_PULLUP),
    .rx         = pin_init("RX", INPUT),
    .baud_rate  = 9600,
    .user_data  = chip
  };

  // Setup UART
  chip->uart0 = uart_init(&uart_config);

  // Setup TX (Simulated TX)
  chip->gps_tx_index = 0;

  // Setup Timer
  const timer_config_t timer_config = {
    .callback = chip_timer_event,
    .user_data = chip,
  };

  timer_t timer = timer_init(&timer_config);

  // Start Timer
  timer_start(timer, SECONDS, true);

  printf("NEO6M GPS initialized\n");
}

// Timer Event Handler Implementation
void chip_timer_event(void *user_data) {
  chip_state_t *chip = (chip_state_t*) user_data;

  printf("chip_timer_event\n");

  // Read GPS Data Item from Sample
  const char * message = gps_tx_data[chip->gps_tx_index++];
  printf("message: %s",message);

  // Send GPS Data to UART
  uart_write(chip->uart0, (uint8_t *) message, strlen(message));

  // Restart Index if no more Sample Data
  if(chip->gps_tx_index >= LEN(gps_tx_data)) {
    chip->gps_tx_index = 0;
  }

}