/*
 * UartThread.h
 *
 *  Created on: 11. 12. 2019
 *      Author: martin
 */

#ifndef UARTTHREAD_H_
#define UARTTHREAD_H_
#include <common.hpp>
#include "fsl_debug_console.h"
#include "Protothread.h"
#include "SwTimer.h"
#include "packet.h"
#include "fsl_lpsci.h"

#define ACK_PACKET_SIZE 5
#define MAX_INFORMATION_SIZE 250





class UartThread: public Protothread {
public:
	UartThread(SwTimer::callback_f func):swtimer(func){
		ack_packet_data = (uint8_t*) malloc(sizeof(uint8_t) * (ACK_PACKET_SIZE));
	}
	~ UartThread(){

	}
	virtual bool Run();
	char ch;
	 packet_queue queues_of_packets;
	 packet_queue queue_of_pakets_to_send;
void set_atributess_of_packet(uint8_t paA0_A1,uint8_t adr_rec, uint8_t adr_send, uint8_t lenght, uint8_t CRC, uint8_t* data);
void send_information_to_d0_terminal(const uint8_t* information);
void prepare_ack_packet_for_send(uint8_t receiver);
void send_Reset_watchDog();
void find_actual_elevator_position();
void activate_Emergency_break();
private:

	uint8_t * ack_packet_data;

protected:
	SwTimer swtimer;
	int i;
    uint8_t	CRC_Count;
	uint8_t packet_received_to;
    bool areNewDataReady();
    bool areDataToSend();
    bool Was_Ack_delivered();
	 packet working_packet;
	 packet working_ack_packet;

	 /* paket obsahuje CRC. a zároveň sa z neho vypočíta nové CRC ktoré musí sedieť s pôvodným v pakete*/
	 bool is_CRC_alright(const packet* paPacket);
	 uint8_t count_CRC_of_packet(const packet* paPacket);

	 uint8_t which_limit_switch_is_turned_on();

	 void createPacket_for_send(const uint8_t paType_A0_A1,const uint8_t paReceiver,const uint8_t paSize_of_data,const uint8_t* paData);
 	 //void send_packet(const uint8_t paType_A0_A1,const uint8_t paReceiver,const uint8_t paSize_of_data,const uint8_t* paData);
 	 void turn_on_led_diod(const uint8_t led_adress_receiver);
 	 void turn_off_led_diod(const uint8_t led_adress_receiver);
 	 void set_information_display(const uint_fast8_t direction, const uint8_t* data_to_display );
 	 void lock_elevator_door();
 	 void unlock_elevator_door();
 	 void motor_stop();
 	 void Elevator_settings(const bool direction_up, const int8_t speed_100_pl_min);
 	 void receive_current_speed_from_ElevatorF1();

 	 void deactivate_Emergency_break();

 	 void Reset_watch_dog_timer();
 	 //todo
 	 void print_packet_to_elevator_terminal();
 	 // prerobiť pakety aby sa dali postupne vyťahovať .

};

#endif /* UARTTHREAD_H_ */
