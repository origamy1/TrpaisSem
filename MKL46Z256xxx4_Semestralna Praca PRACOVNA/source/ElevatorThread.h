/*
 * ElevatorThread.h
 *
 *  Created on: 20. 12. 2020
 *      Author: admin
 */

#ifndef ELEVATORTHREAD_H_
#define ELEVATORTHREAD_H_
#include "UartThread.h"
#include "packet.h"
#include <fsl_debug_console.h>
#include "etl/state_chart.h"
#include "etl/array.h"
#include "common.hpp"



struct Events {
	enum {
		GO_TO_FL_P,GO_TO_FL_1,GO_TO_FL_2,GO_TO_FL_3,GO_TO_FL_4	};
};

struct States {
	enum {
		P_FLOOR,FLOOR_1,FLOOR_2,FLOOR_3,FLOOR_4
	};
};

class Elevator_Thread: public UartThread, public etl::state_chart<Elevator_Thread> {
public:


	Elevator_Thread(packet_queue* PaAueues_of_packets,packet_queue* PaQueue_of_packets_to_Send, SwTimer::callback_f func):etl::state_chart<Elevator_Thread>(*this, transitionTable.data(),
			transitionTable.data() + transitionTable.size(),
			stateTable.data(), stateTable.data() + stateTable.size(),
			States::P_FLOOR),UartThread(func){
			turned_senzor = 0;
		atQueues_of_packets = PaAueues_of_packets;
		//queue_of_pakets_to_send = PaQueue_of_packets_to_Send;

	};

	//packet_queue* queue_of_pakets_to_send;

	virtual bool Run();



	void OnTranTo_Floor_P() {



		 PRINTF("%s\n", __func__);
		 //lock_elevator_door();
		 if(from_inside == false){
			 turn_on_led_diod(0x10);
		 } else {
			 turn_on_led_diod(0x20);
		 }
		 where_to_Go[0] = 1;

		 if(elevator_is_moving == false){
			 lock_elevator_door();
			 Elevator_settings(0, 85);

			   uint8_t pom[2] = "P";
			   set_information_display(0x02,pom);
		 }else{
			 find_actual_elevator_position();
			 printf("\n Výťah sa hýbe neni možno privolať\n");
		 }

		 //buff[3]= "0";
         //set_information_display(0x02, data_to_display)
		 //if(current_state_id != States::P_FLOOR){
		  //	 Elevator_settings(0, 85); // pusti motor dole ak je nad P_FLOOR
		 //}

		 //PT_YIELD();

	}

	void OnTranTo_Floor_1() {
		//lock_elevator_door();
		PRINTF("%s\n", __func__);
		if(from_inside == false){
		    turn_on_led_diod(0x11);
		} else {
			turn_on_led_diod(0x21);
		}
		where_to_Go[1] = 1;

		 if(elevator_is_moving == false){
			 lock_elevator_door();
			 if(not_Moving_coordinates < 250){
				Elevator_settings(1, 85);
				uint8_t pom[2] = "1";
				set_information_display(0x01,pom);
			 }
			 else{
				Elevator_settings(0, 85);
				uint8_t pom[2] = "1";
				set_information_display(0x02,pom);
			 }
		 }else {
			 find_actual_elevator_position();
			 printf("\n Výťah sa hýbe neni možno privolať\n");
		 }

	}
	void OnTranTo_Floor_2() {
		//lock_elevator_door();
		PRINTF("%s\n", __func__);

		if(from_inside == false){
		    turn_on_led_diod(0x12);
		} else {
			turn_on_led_diod(0x22);
		}

	    where_to_Go[2] = 1;

	    if(elevator_is_moving == false){
	   		if(not_Moving_coordinates < 500){
	   			Elevator_settings(1, 85);
	   			uint8_t pom[2] = "2";
	   			set_information_display(0x01,pom);
	   		}else{
	   			Elevator_settings(0, 85);
	   			uint8_t pom[2] = "2";
	   			set_information_display(0x02,pom);
	   		}
	   	}else {
			 find_actual_elevator_position();
			 printf("\n Výťah sa hýbe neni možno privolať\n");
	   	}

	}

	void OnTranTo_Floor_3() {
		 //lock_elevator_door();
		PRINTF("%s\n", __func__);

		if(from_inside == false){
		    turn_on_led_diod(0x13);
		} else {
			turn_on_led_diod(0x23);
		}
     	where_to_Go[3] = 1;

     	if(elevator_is_moving == false){
     		 lock_elevator_door();
     		if(not_Moving_coordinates < 750){
     		 	Elevator_settings(1, 85);
     		 	uint8_t pom[2] = "3";
     		 	set_information_display(0x01,pom);
     		}else{
     			Elevator_settings(0, 85);
     			uint8_t pom[2] = "3";
     			set_information_display(0x02,pom);
     		}
     	}else {
			 find_actual_elevator_position();
			 printf("\n Výťah sa hýbe neni možno privolať\n");
     	}
	}

	void OnTranTo_Floor_4() {

		PRINTF("%s\n", __func__);

		if(from_inside == false){
			turn_on_led_diod(0x14);
		} else {
			turn_on_led_diod(0x24);
		}
		where_to_Go[4] = 1;


		if(elevator_is_moving == false){
			 lock_elevator_door();
			 Elevator_settings(1, 85);
			 uint8_t pom[2] = "4";
			 set_information_display(0x01,pom);
		}else {
			 find_actual_elevator_position();
			 printf("\n Výťah sa hýbe neni možno privolať\n");
		}

	}



	void OnEnter_P_Floor() {
		PRINTF("%s\n", __func__);
	}

	void OnExit_P_Floor() {
		PRINTF("%s\n", __func__);
	}

	void OnEnter_Floor_1() {
		PRINTF("%s\n", __func__);
	}

	void OnExit_Floor_1() {
		PRINTF("%s\n", __func__);
	}
	void OnEnter_Floor_2() {
		PRINTF("%s\n", __func__);
	}

	void OnExit_Floor_2() {
		PRINTF("%s\n", __func__);
	}
	void OnEnter_Floor_3() {
		PRINTF("%s\n", __func__);
	}

	void OnExit_Floor_3() {
		PRINTF("%s\n", __func__);
	}
	void OnEnter_Floor_4() {
		PRINTF("%s\n", __func__);
	}

	void OnExit_Floor_4() {
		PRINTF("%s\n", __func__);
	}
	bool Guard() {
			PRINTF("%s\n", __func__);
			return guard;
	}

private:
	volatile bool where_to_Go[5] = {0}; // pole povelov od tlačidiel (kde treba ešte ísť)
		volatile uint8_t turned_senzor;
	    bool guard;
	    bool from_inside = false;
	    bool elevator_is_moving = false;
	    bool elevator_is_moving_up = true;
	    uint16_t not_Moving_coordinates = 0;
		bool S1 = false;
		bool S2 = false;
		bool pom = false;
		static const etl::array<Elevator_Thread::transition, 20> transitionTable;
		static const etl::array<Elevator_Thread::state, 5> stateTable;
	packet working_Packet;
	packet_queue* atQueues_of_packets;

	bool isPacketPrepared();
	bool isAckDone();

	void set_information_where_to_go_to_terminal(){


		if((where_to_Go[0] == 1 ) || (where_to_Go[1] == 1 ) )
		{
				uint8_t information[33] = "\nGo to  _floor: \n Go to _floor: ";
				information[7] = '0';
				information[15] = where_to_Go[0] + '0';
				//information[20] = 0;
				information[23] = '1';
				information[31] = where_to_Go[1] + '0';
				send_information_to_d0_terminal(information);
		}
		if((where_to_Go[2] == 1 ) || (where_to_Go[3] == 1 ) )
		{
				uint8_t information[33] = "\nGo to  _floor: \n Go to _floor: ";
				information[7] = '2';
				information[15] = where_to_Go[2] + '0';
				information[23] = '3';
				information[31] = where_to_Go[3] + '0';
				send_information_to_d0_terminal(information);
		}
		if((where_to_Go[4] == 1 ))
		{       uint8_t information[21] = "\nGo to  _floor: \n";
				information[7] = '4';
				information[15] = where_to_Go[4] + '0';
				send_information_to_d0_terminal(information);
		}





	}

};

#endif /* ELEVATORTHREAD_H_ */
