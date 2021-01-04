/*
 * ElevatorThread.cpp
 *
 *  Created on: 20. 12. 2020
 *      Author: admin
 */

#include <ElevatorThread.h>

bool Elevator_Thread::isAckDone(){
	if(newAckFlag){
		newAckFlag = false;
		return true;
	} else
		return false;
}

bool Elevator_Thread::isPacketPrepared(){
	if(packetPrepared){
		packetPrepared = false;
		return true;
	} else
		return false;
}

bool Elevator_Thread::Run(){
	PT_BEGIN();

	deactivate_Emergency_break();
	PT_YIELD();
	Reset_watch_dog_timer()	;
	PT_YIELD();
	/*Defaultne nastavenie*/
	/*Vypnutie lediek ak náhodou sú zapnuté*/
	turn_off_led_diod(0x10);
	turn_off_led_diod(0x11);
	turn_off_led_diod(0x12);
	turn_off_led_diod(0x13);
	turn_off_led_diod(0x14);
	PT_YIELD();
	turn_off_led_diod(0x20);
	turn_off_led_diod(0x21);
	turn_off_led_diod(0x22);
	turn_off_led_diod(0x23);
	turn_off_led_diod(0x24);

	PT_YIELD();

	{
		uint8_t data[8] = "ReSTART";
	set_information_display(0x00, data);

	}
	PT_YIELD();

    /* zistenie aktuálnej pozície výťahu a podľa nej rozbehnúť výťah*/
	find_actual_elevator_position();
	PT_YIELD();
	PT_WAIT_UNTIL(isPacketPrepared());
			atQueues_of_packets->Packets_from_queue.pop_into(working_Packet);
	pom = true;
	{
		uint16_t position_of_elevator= working_Packet.ch_data[0]+ working_Packet.ch_data[1]*16*16;
		lock_elevator_door();
		if(position_of_elevator <460){
			Elevator_settings(1, 85);
			pom = false;
		} else if(position_of_elevator > 540){
			Elevator_settings(0, 85);
			pom = false;
		}

	}


	/* Ak sa výťah rozbehne treba kontrolovať senzory aby sa zastavil výťah*/
	if(!pom){
		for(i = 0; i < 30 ; i++){
			PT_WAIT_UNTIL(isPacketPrepared()); // pridu dáta zo senzora
			atQueues_of_packets->Packets_from_queue.pop_into(working_Packet);
			if((working_Packet.ch_data[0] == 0x01)  && (working_Packet.ch_adr_sender == 0xE2)){
				motor_stop();
				PT_YIELD();
				//PT_WAIT_UNTIL(isPacketPrepared());
				break;
			}
		}
	}

	unlock_elevator_door();
	PT_YIELD();
	current_state_id = 2;
	//PT_WAIT_UNTIL(isPacketPrepared()); // asi hlúpsť tu čakať. nieje načo
	start(false);
    guard = true;
	// Defaultný stav sa nastavuje v konštruktore elevatorThread. kde sa state chart nastavuje


     /* Nastavenie aktuálnych súradníc stojacého výťahu */
    find_actual_elevator_position();
    	PT_WAIT_UNTIL(isPacketPrepared());
    			atQueues_of_packets->Packets_from_queue.pop_into(working_Packet);
    not_Moving_coordinates = working_Packet.ch_data[0]+ working_Packet.ch_data[1]*16*16;
    elevator_is_moving = false;

    /* RIADENIE VÝŤAHOVEJ APLIKÁCIE */
	while(true){

		PT_WAIT_UNTIL(isPacketPrepared());

		if(!(atQueues_of_packets->Packets_from_queue.empty())){ // ak je nejaký paket na spracovanie.
			 atQueues_of_packets->Packets_from_queue.pop_into(working_Packet); // vytiahne paket ktorý spracuje

			  /* Spracovanie povelu z výťahu (na jeho základe zmena stavu a chodu výťahu)*/
				switch(working_Packet.ch_adr_sender){

				/* Tlačidlá z vonku kabíny*/

				   case 0xC0:
				   {

					   from_inside = false;
					   process_event(Events::GO_TO_FL_P);
				   }
				   break;
				   case 0xC1:
				   {
					   from_inside = false;
					   process_event(Events::GO_TO_FL_1);

				   }
				   break;
				   case 0xC2:
				   {
					   from_inside = false;
					   process_event(Events::GO_TO_FL_2);

				   }
				   break;

				   case 0xC3:
				   {
					   from_inside = false;
					   process_event(Events::GO_TO_FL_3);

				   }

				   break;

				   case 0xC4:
				   {
					   from_inside = false;
					   process_event(Events::GO_TO_FL_4);
				   }
				   break;

				/* Tlačidlá v kabíne*/

				   case 0xB0:{
					   from_inside = true;
					   process_event(Events::GO_TO_FL_P);}
				   break;
				   case 0xB1:{
					   from_inside = true;
					   process_event(Events::GO_TO_FL_1);
				   }

				   break;
				   case 0xB2:
					   from_inside = true;
					   process_event(Events::GO_TO_FL_2);
				   break;

				   case 0xB3:
					   from_inside = true;
					   process_event(Events::GO_TO_FL_3);
				   break;

				   case 0xB4:
					   from_inside = true;
					   process_event(Events::GO_TO_FL_4);
				   break;

				/* Senzory poschodí*/

				   case 0xE0:{

					   if(where_to_Go[0] == 1){


						   current_state_id = 0;
						   motor_stop();
						   unlock_elevator_door();
						   //PT_YIELD();
						   elevator_is_moving = false;
						   turn_off_led_diod(0x10);
						   turn_off_led_diod(0x20);
						   where_to_Go[0] = 0;
						   //PT_YIELD();
						  // set_information_where_to_go_to_terminal();
						 }
						   find_actual_elevator_position();



					   }
				   break;
				   case 0xE1:
				   {

					   if(where_to_Go[1] == 1){
						   current_state_id = 1;
						    motor_stop();
						    unlock_elevator_door();
						   elevator_is_moving = false;
						   turn_off_led_diod(0x11);
						   turn_off_led_diod(0x21);
						   where_to_Go[1] = 0;
						   //PT_YIELD();
						 //  set_information_where_to_go_to_terminal();
						   find_actual_elevator_position();
						   break;
					   }

				   }
				   break;
				   case 0xE2:
				   {

					   if(where_to_Go[2] == 1){
						   current_state_id = 2;
						   motor_stop();
						   unlock_elevator_door();
						   elevator_is_moving = false;
						   turn_off_led_diod(0x12);
						   turn_off_led_diod(0x22);
						   where_to_Go[2] = 0;
						  // PT_YIELD();
						   //set_information_where_to_go_to_terminal();
						   find_actual_elevator_position();

					   }
				   }
				   break;

				   case 0xE3:

					   if(where_to_Go[3] == 1){
						   current_state_id = 3;
						   motor_stop();
						   unlock_elevator_door();
						   elevator_is_moving = false;
						   turn_off_led_diod(0x13);
						   turn_off_led_diod(0x23);
						   where_to_Go[3] = 0;
						  // PT_YIELD();
						  // set_information_where_to_go_to_terminal();
						   find_actual_elevator_position();
					   }
				   break;

				   case 0xE4:


					   if(where_to_Go[4] == 1){
						   current_state_id = 4;
						   motor_stop();
						   unlock_elevator_door();
						   elevator_is_moving = false;
						   turn_off_led_diod(0x14);
						   turn_off_led_diod(0x24);
						   where_to_Go[4] = 0;
						  // PT_YIELD();
						   //set_information_where_to_go_to_terminal();
						   find_actual_elevator_position();
					   }

				   break;

				 /* Dorazové senzory hore dole */
				   case 0xE5:
				   {
					  // uint8_t buf[45] = "POZOR VÝŤAH JE NA KONCI\n";
					  // send_information_to_d0_terminal(information);
				   }
			       break;

				   case 0xDF:
				   {

				   }


				  	 // uint8_t buf[45] = "POZOR VÝŤAH JE NA KONCI\n";
				  	 // send_information_to_d0_terminal(information);

				   break;

				   /* Dáta prijaté z terminálu*/

				   case 0xD0:

					   /*if(working_Packet.ch_lenght_of_data == 7){
						  char buff[8] = "restart" ;
						  bool same = true;
						  for(int i = 0; i < 7 ; i++){
							if(working_Packet.ch_data[i] != buff[i])
								same = false;
						  }
						  if(same){
							  queue_of_pakets_to_send.Packets_from_queue.empty();
							  Restart();
						  }
					   }*/
					   from_inside = true;
					   switch(working_Packet.ch_data[0]){
					   case '0':
						   process_event(Events::GO_TO_FL_P);
						   break;
					   case '1':
						   process_event(Events::GO_TO_FL_1);
					   		break;
					   case '2':
						   process_event(Events::GO_TO_FL_2);
					   	   break;
					   case '3':
						   process_event(Events::GO_TO_FL_3);
					   	   break;
					   case '4':
						   process_event(Events::GO_TO_FL_4);
					   	   break;
					   case 'E':
					   	   deactivate_Emergency_break();
					   	   activateBreak = false;
					   	   break;
					   default:
						   break;
					   }

				   break;

				   case 0xF1:{

					   if(working_Packet.ch_lenght_of_data == 2) {
						   not_Moving_coordinates = working_Packet.ch_data[0]+ working_Packet.ch_data[1]*16*16;
						   receive_current_speed_from_ElevatorF1();
					   }
					   if(working_Packet.ch_lenght_of_data == 4) {
						   uint8_t buf[4];
						   buf[0] = working_Packet.ch_data[0];
						   buf[1] = working_Packet.ch_data[1];
						   buf[2] = working_Packet.ch_data[2];
						   buf[3] = working_Packet.ch_data[3];
						   float Speed_of_Motor = 0;
						   uint8_t * pom2 =(uint8_t*)&Speed_of_Motor;
						   pom2[0] = buf[0];
						   pom2[1] = buf[1];
						   pom2[2] = buf[2];
						   pom2[3] = buf[3];
						   if(Speed_of_Motor > 0)
							   elevator_is_moving = true;
						   else if (Speed_of_Motor == 0){
							   elevator_is_moving = false;
						   if(elevator_is_moving == false){

						   if(not_Moving_coordinates>700){	   }

							   set_information_where_to_go_to_terminal();

							   uint8_t floor = 0;

							   if(not_Moving_coordinates < 200){
								   floor = 0 ;
							   }else if(not_Moving_coordinates < 400){
								   floor = 1 ;
							   }else if(not_Moving_coordinates < 600){
								   floor = 2 ;
							   }else if(not_Moving_coordinates < 800){
								   floor = 3 ;
							   }else{
								   floor = 4 ;
							   }

							   uint8_t bellow = 100,up = 100;
							   for(int i = 0 ; i < 5; i++){
								   if(where_to_Go[i] == 1 ){
									  if(i < floor){
										  if(i > bellow || bellow == 100)
										  bellow = i;
									  }
									  if(i > floor){
										  if(i < up)
										  up = i;
									  }
								  }
						      }

							    if(bellow == 100 || up == 100){

							   if((where_to_Go[0] == 1) ){
								   goToFloorP();
								   break;
							   }

							   if((where_to_Go[1] == 1)){
								   goToFloor1();
								   break;
							   }
						       if(where_to_Go[2] == 1){
								   goToFloor2();
								   break;
							   }

						       if(where_to_Go[3] == 1){
						    	   goToFloor3();
						    	   break;
						       }

						       if(where_to_Go[4] == 1){
						     	   goToFloor4();
						     	   break;
						       }
							   } else {

								   if((not_Moving_coordinates - (200*bellow+100)) < ((200*up+100) - not_Moving_coordinates) ){
									floor = bellow ;
								   }else{
									floor = up;
								   }

								   switch(floor){
								   case 0:
									   goToFloorP();
									 break;
								   case 1:
									   goToFloor1();
								  	 break;
								   case 2:
									   goToFloor2();
								  	 break;
								   case 3:
									   goToFloor3();
								  	 break;
								   case 4:
									   goToFloor4();
								  	 break;
								   default:
									   break;
								   }

							   }


						   }
							}
						   }
					}
				   break;
				   default:
				   break;
				}
		}
	}
	PT_END();
}

void Elevator_Thread::goToFloorP(){
	  if(not_Moving_coordinates > 100){
		  elevator_is_moving = true;
	  lock_elevator_door();
	  Elevator_settings(0, 85);
	   uint8_t pom[2] = "P";
		set_information_display(0x02,pom);
	  }
}

void Elevator_Thread::goToFloor1(){

	 if(not_Moving_coordinates < 200){
		 elevator_is_moving = true;
		 lock_elevator_door();
		 Elevator_settings(1, 85);
		   uint8_t pom[2] = "1";
				set_information_display(0x01,pom);
	 } else {
	 lock_elevator_door();
	 Elevator_settings(0, 85);
	 	 uint8_t pom[2] = "1";
	 		set_information_display(0x02,pom);
	 }
}

void Elevator_Thread::goToFloor2(){
	if(not_Moving_coordinates < 400){
		elevator_is_moving = true;
		lock_elevator_door();
	    Elevator_settings(1, 85);
	    uint8_t pom[2] = "2";
	    	set_information_display(0x01,pom);
		}else{
		 lock_elevator_door();
		Elevator_settings(0, 85);
		uint8_t pom[2] = "2";
			set_information_display(0x02,pom);
	}
}

void Elevator_Thread::goToFloor3(){
	 if(not_Moving_coordinates < 600){
		 elevator_is_moving = true;
		lock_elevator_door();
		Elevator_settings(1, 85);
		uint8_t pom[2] = "3";
			set_information_display(0x01,pom);
	 }else{
		lock_elevator_door();
		 Elevator_settings(0, 85);
		 uint8_t pom[2] = "3";
		 	set_information_display(0x02,pom);
	 }
}

void Elevator_Thread::goToFloor4(){
	elevator_is_moving = true;
	 if(not_Moving_coordinates < 800){
		lock_elevator_door();
		Elevator_settings(1, 85);
		uint8_t pom[2] = "4";
			set_information_display(0x01,pom);
	 }
}

const etl::array<Elevator_Thread::transition, 20> Elevator_Thread::transitionTable = {

		/* From P_FLOOR*/
		Elevator_Thread::transition(
				States::P_FLOOR,   //Current state
				Events::GO_TO_FL_1, // Event
				States::FLOOR_1,  // New state
				&Elevator_Thread::OnTranTo_Floor_1,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::P_FLOOR,   //Current state
				Events::GO_TO_FL_2, // Event
				States::FLOOR_2,  // New state
				&Elevator_Thread::OnTranTo_Floor_2,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::P_FLOOR,   //Current state
				Events::GO_TO_FL_3, // Event
				States::FLOOR_3,  // New state
				&Elevator_Thread::OnTranTo_Floor_3,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::P_FLOOR,   //Current state
				Events::GO_TO_FL_4, // Event
				States::FLOOR_4,  // New state
				&Elevator_Thread::OnTranTo_Floor_4,
				&Elevator_Thread::Guard),


		/* From FLOOR_1*/

		Elevator_Thread::transition(
				States::FLOOR_1,   //Current state
				Events::GO_TO_FL_2, // Event
				States::FLOOR_2,  // New state
				&Elevator_Thread::OnTranTo_Floor_2,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_1,   //Current state
				Events::GO_TO_FL_3, // Event
				States::FLOOR_3,  // New state
				&Elevator_Thread::OnTranTo_Floor_3,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_1,   //Current state
				Events::GO_TO_FL_4, // Event
				States::FLOOR_4,  // New state
				&Elevator_Thread::OnTranTo_Floor_4,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_1,   //Current state
				Events::GO_TO_FL_P, // Event
				States::P_FLOOR,  // New state
				&Elevator_Thread::OnTranTo_Floor_P,
				&Elevator_Thread::Guard),

		/* From FlOOR_2*/

		Elevator_Thread::transition(
				States::FLOOR_2,   //Current state
				Events::GO_TO_FL_1, // Event
				States::FLOOR_1,  // New state
				&Elevator_Thread::OnTranTo_Floor_1,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_2,   //Current state
				Events::GO_TO_FL_P, // Event
				States::P_FLOOR,  // New state
				&Elevator_Thread::OnTranTo_Floor_P,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_2,   //Current state
				Events::GO_TO_FL_3, // Event
				States::FLOOR_3,  // New state
				&Elevator_Thread::OnTranTo_Floor_3,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_2,   //Current state
				Events::GO_TO_FL_4, // Event
				States::FLOOR_4,  // New state
				&Elevator_Thread::OnTranTo_Floor_4,
				&Elevator_Thread::Guard),

		/* From FLOOR_3*/

		Elevator_Thread::transition(
				States::FLOOR_3,   //Current state
				Events::GO_TO_FL_1, // Event
				States::FLOOR_1,  // New state
				&Elevator_Thread::OnTranTo_Floor_1,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_3,   //Current state
				Events::GO_TO_FL_2, // Event
				States::FLOOR_2,  // New state
				&Elevator_Thread::OnTranTo_Floor_2,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_3,   //Current state
				Events::GO_TO_FL_P, // Event
				States::P_FLOOR,  // New state
				&Elevator_Thread::OnTranTo_Floor_P,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_3,   //Current state
				Events::GO_TO_FL_4, // Event
				States::FLOOR_4,  // New state
				&Elevator_Thread::OnTranTo_Floor_4,
				&Elevator_Thread::Guard),


		/* From FLOOR_4*/

		Elevator_Thread::transition(
				States::FLOOR_4,   //Current state
				Events::GO_TO_FL_1, // Event
				States::FLOOR_1,  // New state
				&Elevator_Thread::OnTranTo_Floor_1,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_4,   //Current state
				Events::GO_TO_FL_2, // Event
				States::FLOOR_2,  // New state
				&Elevator_Thread::OnTranTo_Floor_2,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_4,   //Current state
				Events::GO_TO_FL_3, // Event
				States::FLOOR_3,  // New state
				&Elevator_Thread::OnTranTo_Floor_3,
				&Elevator_Thread::Guard),
		Elevator_Thread::transition(
				States::FLOOR_4,   //Current state
				Events::GO_TO_FL_P, // Event
				States::P_FLOOR,  // New state
				&Elevator_Thread::OnTranTo_Floor_P,
				&Elevator_Thread::Guard)
};


const etl::array<Elevator_Thread::state, 5> Elevator_Thread::stateTable={

		Elevator_Thread::state(
				States::P_FLOOR,
				&Elevator_Thread::OnEnter_P_Floor,
				&Elevator_Thread::OnExit_P_Floor)
		,Elevator_Thread::state(
				States::FLOOR_1,
				&Elevator_Thread::OnEnter_Floor_1,
				&Elevator_Thread::OnExit_Floor_1)
		,Elevator_Thread::state(
				States::FLOOR_2,
				&Elevator_Thread::OnEnter_Floor_2,
				&Elevator_Thread::OnExit_Floor_2)
		,Elevator_Thread::state(
				States::FLOOR_3,
				&Elevator_Thread::OnEnter_Floor_3,
				&Elevator_Thread::OnExit_Floor_3)
		,Elevator_Thread::state(
				States::FLOOR_4,
				&Elevator_Thread::OnEnter_Floor_4,
				&Elevator_Thread::OnExit_Floor_4)

};






