/*
 * SendPackets.h
 *
 *  Created on: 20. 12. 2020
 *      Author: admin
 */

#ifndef SENDPACKETS_H_
#define SENDPACKETS_H_
#include "UartThread.h"


 //! Trieda na odoslanie paketov.
  /*! Táto trieda odosiela pakety prichystané v qeue, ktorého adresu má queue_of_pakets_to_send . Ak je čo odoslať to odošle*/
class SendPackets : public UartThread {
public:


	SendPackets(SwTimer::callback_f func,packet_queue* Paqueue_packets_to_send) :UartThread(func){
		queue_of_pakets_to_send = Paqueue_packets_to_send;
	};
	virtual bool Run();

	packet_queue* queue_of_pakets_to_send;

private:
	uint8_t buf[300]; /*!< WriteBlocking funkcia využíva uint_8t typ. Tak celý paket sa vloží do buffra a potom odošle z buffra */
	packet packet_to_send;
	int i,b;

    //! Zistenie či sú nachystané nejaké dáta na odoslanie.
    /*! Ak sú data na odoslanie tak vráti true. inak false a zároveň sa odpočíta jedno číslo lebo paket sa práve spracuje */
	bool areDataToSend(){

			if (data_To_send_count ){
				data_To_send_count-- ;
				return true;
			} else
				return false;
		}
	packet working_Packet;

    //! Nachystanie paketu na odoslanie
    /*! Paket z typu packet sa vloží do buffra buf a vypočíta sa jeho CRC*/
	void sendPacket(packet* paPacket){

		paPacket->ch_CRC8 = count_CRC_of_packet(paPacket);

		//uint8_t buf[5+ paPacket->ch_lenght_of_data+1];
		buf[0] = paPacket->ch_type_A0_A1;
		buf[1] = paPacket->ch_adr_receiver;
		buf[2] = paPacket->ch_adr_sender;
		buf[3] = paPacket->ch_lenght_of_data ;
		for( i = 0; i < paPacket->ch_lenght_of_data ; i++){
				buf[4+i] = paPacket->ch_data[i];
		}
	    buf[4 + paPacket->ch_lenght_of_data] =  count_CRC_of_packet(paPacket);
	    buf[5 + paPacket->ch_lenght_of_data] = '\0';

	    packet_received_to =  buf[1];
		/* uint8_t* pom = buf;

	    while(*pom != 0){
	    	LPSCI_WriteByte(UART0, *pom);
	    	pom++;
	    }*/
		//packet_received_to =  paPacket->ch_adr_receiver;

	}



	void sendPacketSend(){ /*!< Odošle sa pripravený paket z buffra. */
		  swtimer.restartTimer();
		  LPSCI_WriteBlocking(UART0, buf, 5 + buf[3]);
	}

};

#endif /* SENDPACKETS_H_ */
