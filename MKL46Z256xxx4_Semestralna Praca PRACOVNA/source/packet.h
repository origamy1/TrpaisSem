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


 //! Štruktúra uchovávajúca celý paket.
 /*! Uchováva štart bajt, bajt adresy prímateľa, bajt adresy odosielateľa, bajt veľkostí dát, potenciálnych 255 bajtov dát, a CRC 8 */
typedef struct Packet{
	uint8_t ch_type_A0_A1;
	uint8_t ch_adr_receiver;
	uint8_t ch_adr_sender;
	uint8_t ch_lenght_of_data;
	uint8_t ch_data[DATA_MAX_LENGHT];
	uint8_t ch_CRC8;

}packet;

//! štruktúra Packet_qeue.
/*! Uchováva queue 20 paketov. Je to zásobník paketov z knižnice Etl.cpp  kde sa uchovávajú vždy pracovné pakety.
 *  buď tie ktoré sa prijali, alebo tie ktoré sú nachystané na odoslanie. Je možné že to pretečie ak je privela povelov naraz
 *  Dá sa to ošetriť zväčsením tohoto Qeue. Ale potom je treba dynamicky alokovať pakety.
 *   */
typedef struct Packet_queue{
	etl::queue<packet,20> Packets_from_queue;
}packet_queue;



#endif /* PACKET_H_ */
