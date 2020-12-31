/*
 * SendPackets.h
 *
 *  Created on: 20. 12. 2020
 *      Author: admin
 */

#ifndef SENDPACKETS_H_
#define SENDPACKETS_H_
#include "UartThread.h"

class SendPackets : public UartThread {
public:
	SendPackets(SwTimer::callback_f func,packet_queue* Paqueue_packets_to_send) :UartThread(func){
		queue_of_pakets_to_send = Paqueue_packets_to_send;
	};
	virtual bool Run();

	packet_queue* queue_of_pakets_to_send;

private:
	packet packet_to_send;
	bool areDataToSend(){

			if (data_To_send_count ){
				data_To_send_count-- ;
				return true;
			} else
				return false;
		}
	packet working_Packet;

	void sendPacket(packet* paPacket){

		paPacket->ch_CRC8 = count_CRC_of_packet(paPacket);

		uint8_t buf[5+ paPacket->ch_lenght_of_data+1];
		buf[0] = paPacket->ch_type_A0_A1;
		buf[1] = paPacket->ch_adr_receiver;
		buf[2] = paPacket->ch_adr_sender;
		buf[3] = paPacket->ch_lenght_of_data ;
		for(int i = 0; i < paPacket->ch_lenght_of_data ; i++){
				buf[4+i] = paPacket->ch_data[i];
		}
	    buf[4 + paPacket->ch_lenght_of_data] =  count_CRC_of_packet(paPacket);
	    buf[5 + paPacket->ch_lenght_of_data] = '\0';
		LPSCI_WriteBlocking(UART0, buf, 5 + paPacket->ch_lenght_of_data);
	   /* uint8_t* pom = buf;

	    while(*pom != 0){
	    	LPSCI_WriteByte(UART0, *pom);
	    	pom++;
	    }*/
		packet_received_to =  paPacket->ch_adr_receiver;

	}
};

#endif /* SENDPACKETS_H_ */
