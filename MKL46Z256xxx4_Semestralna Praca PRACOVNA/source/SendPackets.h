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
	bool areDataToSend();
	packet working_Packet;
    //! Nachystanie paketu na odoslanie
    /*! Paket z typu packet sa vloží do buffra buf a vypočíta sa jeho CRC*/
	void sendPacket(packet* paPacket);

	void sendPacketSend();	 /*!< Odošle sa pripravený paket z buffra. */

};

#endif /* SENDPACKETS_H_ */
