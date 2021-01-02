/*
 * main.hpp
 *
 *  Created on: 11. 12. 2019
 *      Author: martin
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_
#include <stdint.h>

uint64_t getRunTimeInMs();
extern volatile char newChar;
extern volatile uint8_t newDataFlag;
extern volatile bool packetPrepared;
extern unsigned char crc_array[256];
extern volatile bool newAckFlag ;
extern volatile uint8_t data_To_send_count ;
extern volatile uint8_t last_ack_from;
#endif /* COMMON_HPP_ */
extern uint8_t *packet_data;
