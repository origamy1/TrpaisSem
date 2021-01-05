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
							//swtimer.startTimer(5);
							//do{
						    //b++;
							sendPacketSend();
							//PT_WAIT_UNTIL(Was_Ack_delivered() || swtimer.isExpired() );

							//}while(swtimer.isExpired() && (b < 2));
					        //if(newAckFlag == 1)
					        	//bool pom = 1;

						}

			}
	PT_END();


}

void SendPackets::sendPacketSend(){
	swtimer.restartTimer();
	LPSCI_WriteBlocking(UART0, buf, 5 + buf[3]);
}
void SendPackets::sendPacket(packet* paPacket){

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

bool SendPackets::areDataToSend(){

	if (data_To_send_count ){
		data_To_send_count-- ;
		return true;
	} else
		return false;
}

