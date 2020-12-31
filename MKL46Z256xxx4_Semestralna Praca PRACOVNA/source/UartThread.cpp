/*
 * UartThread.cpp
 *
 *  Created on: 11. 12. 2019
 *      Author: martin
 */

#include <UartThread.h>

unsigned char crc_array[256] =
{
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
    0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
    0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
    0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
    0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
    0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
    0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
    0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
    0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
    0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
    0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
    0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
    0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
    0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
    0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
    0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
    0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};

bool UartThread::Run() {
	PT_BEGIN();

	while(true){


	}

	PT_END();
}


bool UartThread::areNewDataReady() {
if (newDataFlag>0) {
	newDataFlag --;
	return true;
} else
	return false;
}

void UartThread::set_atributess_of_packet(uint8_t paA0_A1,uint8_t adr_rec, uint8_t adr_send, uint8_t lenght, uint8_t CRC, uint8_t* data){
	packet pom;
	pom.ch_adr_receiver = adr_rec;
	pom.ch_adr_sender = adr_send;
	pom.ch_lenght_of_data = lenght;
	pom.ch_CRC8 = CRC;
	pom.ch_type_A0_A1  = paA0_A1;
	for(int i = 0; i < lenght; i++){
		pom.ch_data[i] = data[i];
	}
	queues_of_packets.Packets_from_queue.push(pom);
}

void UartThread::send_information_to_d0_terminal(const uint8_t* information){
	createPacket_for_send(0xA0, 0xD0, strlen((char*)information), information);
}


/* paket obsahuje CRC. a zároveň sa z neho vypočíta nové CRC ktoré musí sedieť s pôvodným v pakete*/
bool UartThread::is_CRC_alright(const packet* paPacket){
	uint8_t CRC = count_CRC_of_packet(paPacket);
	 return CRC == paPacket->ch_CRC8 ? true : false;
}

uint8_t UartThread::count_CRC_of_packet(const packet* paPacket){

		int CRC_Count = 0;
		CRC_Count = crc_array[paPacket->ch_adr_receiver ^ CRC_Count];
		CRC_Count = crc_array[paPacket->ch_adr_sender ^ CRC_Count];

		for(int i = 0; i <  paPacket->ch_lenght_of_data; i++){
			CRC_Count = crc_array[paPacket->ch_data[i] ^ CRC_Count];
		}
		return CRC_Count;
}



uint8_t UartThread::which_limit_switch_is_turned_on(){



}

void UartThread::find_actual_elevator_position(){
	uint8_t data = 0x03 ; // vyžiadanie 16 bitového čísla
	createPacket_for_send(0xA0, 0xF1, 1,&data);
}

void UartThread::prepare_ack_packet_for_send(const uint8_t receiver){


	   working_ack_packet.ch_adr_receiver = receiver;
	   working_ack_packet.ch_adr_sender = 0; // odosielateľ je počítač s adresou 0
	   working_ack_packet.ch_lenght_of_data = 0;
	   working_ack_packet.ch_type_A0_A1 = 0xA1;
	   working_ack_packet.ch_CRC8 =  count_CRC_of_packet(&working_ack_packet);;  // CRC sa vypočíta pred odoslaním samo
	   	uint8_t buf[5];
	   	buf[0] = 0xA1;
	   	buf[1] = receiver;
	   	buf[2] =0;
	   	buf[3] =0 ;
	   	buf[4] = working_ack_packet.ch_CRC8  ;

	   		LPSCI_WriteBlocking(UART0, buf, 5 );
	   		packet_received_to =  receiver;

	//createPacket_for_send(0xA1, receiver, 0, 0);
	//send_packet(0xA1, receiver, 0, 0);// možno by bol dobré poslať ten ack paket hneď. mimo logík vlákien
}

void UartThread::createPacket_for_send(const uint8_t paType_A0_A1,const uint8_t paReceiver,const uint8_t paSize_of_data,const uint8_t* paData){
	working_packet.ch_CRC8 = 0;  // CRC sa vypočíta pred odoslaním samo
	working_packet.ch_adr_receiver = paReceiver;
	working_packet.ch_adr_sender = 0; // odosielateľ je počítač s adresou 0
	working_packet.ch_lenght_of_data = paSize_of_data;
	working_packet.ch_type_A0_A1 = paType_A0_A1;
	for(int i = 0; i < paSize_of_data; i++){
		working_packet.ch_data[i] = paData[i];
	}
	queue_of_pakets_to_send.Packets_from_queue.push(working_packet);
	data_To_send_count++;
}


void UartThread::turn_on_led_diod(const uint8_t led_adress_receiver){
	uint8_t turn_on = 0x01;
	createPacket_for_send(0xA0, led_adress_receiver, 1 , &turn_on);
	packet_received_to = led_adress_receiver;
}

void UartThread::turn_off_led_diod(const uint8_t led_adress_receiver){
	uint8_t turn_off = 0x00;
	createPacket_for_send(0xA0, led_adress_receiver, 1 , &turn_off);
	packet_received_to = led_adress_receiver;
}

void UartThread::set_information_display(const uint_fast8_t direction ,const uint8_t* data_to_display){
	 uint8_t buff[MAX_INFORMATION_SIZE+4];
	 buff[0] = direction;
	 strcpy(((char*)buff)+1,(char*) data_to_display);
	 createPacket_for_send(0xA0, 0x30, strlen((char*)data_to_display)+1, buff);
	 packet_received_to = 0x30;
}

void UartThread::lock_elevator_door(){
	uint8_t lock_el = 0x01;
	createPacket_for_send(0xA0, 0xF0, 1, &lock_el);
}

void UartThread::unlock_elevator_door(){
	uint8_t lock_el = 0x00;
	createPacket_for_send(0xA0, 0xF0, 1, &lock_el);
}

void UartThread::motor_stop(){
	uint8_t stop = 0x1;
	createPacket_for_send(0xA0, 0xF1, 1, &stop);
}

void UartThread::Elevator_settings(const bool direction_up, const int8_t speed_100_pl_min){
	uint8_t movement[5] = {0};
	movement[0]= 0x2;
	if(direction_up == true)
		movement[1]= speed_100_pl_min;
	else{
		movement[1]= -speed_100_pl_min;
		movement[2]= 0xFF;
		movement[3]= 0xFF;
		movement[4]= 0xFF;//-speed_100_pl_min;//speed_100_pl_min;
	}


	createPacket_for_send(0xA0, 0xF1, 5, movement);
}

void UartThread::receive_current_speed_from_ElevatorF1(){
	uint8_t send_speed = 0x04;
	createPacket_for_send(0xA0, 0xF1, 1, &send_speed);
}



void UartThread::send_Reset_watchDog(){
	uint8_t reset_watchdog = 0x00;
	createPacket_for_send(0xA0, 0xFE, 1, &reset_watchdog);
}

void UartThread::deactivate_Emergency_break(){
	uint8_t deactivate = 0x00;
	createPacket_for_send(0xA0, 0xf, 1, &deactivate);
}

void UartThread::activate_Emergency_break(){
	uint8_t deactivate = 0x01;
	createPacket_for_send(0xA0, 0xf, 1, &deactivate);
}

void UartThread::Reset_watch_dog_timer(){
	uint8_t reset_timer = 0x01;
		createPacket_for_send(0xA0, 0xfe, 1, &reset_timer);

}









