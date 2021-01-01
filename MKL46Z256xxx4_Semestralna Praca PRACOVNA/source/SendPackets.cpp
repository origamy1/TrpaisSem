/*
 * SendPackets.cpp
 *
 *  Created on: 20. 12. 2020
 *      Author: admin
 */

#include <SendPackets.h>




bool SendPackets::Run(){
	PT_BEGIN();

			while (true) {
				// wait for sync byte
						PT_WAIT_UNTIL(areDataToSend());

						queue_of_pakets_to_send->Packets_from_queue.pop_into(working_Packet);
						ch = working_Packet.ch_type_A0_A1;
						if (ch == 0xA0 || ch == 0xA1) {
							//if (ch == 0xA1) printftf("\n odoslal sa acknowledge\n");
							sendPacket(&working_Packet);
							b= 0;
							swtimer.startTimer(6);
							do{
						    b++;
							sendPacketSend();
							PT_WAIT_UNTIL(Was_Ack_delivered() || swtimer.isExpired() );

							}while(swtimer.isExpired() && (b < 8));
					        //if(newAckFlag == 1)
					        	//bool pom = 1;

						}

			}
	PT_END();


}
