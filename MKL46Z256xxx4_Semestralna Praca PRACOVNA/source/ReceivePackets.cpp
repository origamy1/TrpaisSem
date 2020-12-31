/*
 * ReceivePackets.cpp
 *
 *  Created on: 20. 12. 2020
 *      Author: admin
 */

#include <ReceivePackets.h>







bool ReceivePackets::Run(){
	PT_BEGIN();

		while (true) {
					PT_WAIT_UNTIL(areNewDataReady());
					//ch = newChar;
					//LPSCI_DisableInterrupts(UART0,  kLPSCI_RxDataRegFullInterruptEnable);
					queues_of_packets.Packets_from_queue.pop_into(working_packet);
					ch = working_packet.ch_type_A0_A1;
					//LPSCI_EnableInterrupts(UART0,kLPSCI_RxDataRegFullInterruptEnable);
					switch (ch) {
					/* ak dorazili dáta*/
						case 0xA0:

							if(is_CRC_alright(&working_packet)){
								prepare_ack_packet_for_send(working_packet.ch_adr_sender);
						//LPSCI_DisableInterrupts(UART0,  kLPSCI_RxDataRegFullInterruptEnable);
							queues_of_packets.Packets_from_queue.push(working_packet);
						//LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullFlag);
							packetPrepared = true;
							}
							break;

					/* ak dorazilo potvrdenie acknowledge*/
						case 0xA1:

							if(is_CRC_alright(&working_packet)){
								newAckFlag = true;
								last_ack_from = working_packet.ch_adr_sender;
							}
							break;
						default:

							break;
				 }
		}

	PT_END();
}
