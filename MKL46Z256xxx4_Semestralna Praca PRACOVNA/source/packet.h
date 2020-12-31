/*
 * packet.h
 *
 *  Created on: 20. 12. 2020
 *      Author: admin
 */

#ifndef PACKET_H_
#define PACKET_H_
#define DATA_MAX_LENGHT 100
#include "etl/queue.h"

typedef struct Packet{
	uint8_t ch_type_A0_A1;
	uint8_t ch_adr_receiver;
	uint8_t ch_adr_sender;
	uint8_t ch_lenght_of_data;
	uint8_t ch_data[DATA_MAX_LENGHT];
	uint8_t ch_CRC8;

}packet;

typedef struct Packet_queue{
	etl::queue<packet,20> Packets_from_queue;
}packet_queue;



#endif /* PACKET_H_ */
