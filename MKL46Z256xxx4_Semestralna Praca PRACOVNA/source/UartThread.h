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

	    //! Nastavenie atributov paketu.
	    /*! Nastavenie paketu typu paket a jeho vloženie do qeue. */
void set_atributess_of_packet(uint8_t paA0_A1,uint8_t adr_rec, uint8_t adr_send, uint8_t lenght, uint8_t CRC, uint8_t* data);
//! Vytvorenie paketu ktorý sa odošle na D0 terminál.
/*! Parameter information predstavuje dáta, ktoré sa vypíšu na tomto terminále výťahu*/
void send_information_to_d0_terminal(const uint8_t* information);

//! Odoslanie acknowledge paketu.
/*! Odoslanie acknowledge paketu na adresu Receiver. */
void prepare_ack_packet_for_send(uint8_t receiver);

void send_Reset_watchDog(); /*!< Prichystanie paketu resetovania WatchDogu. */


void find_actual_elevator_position(); /*!< Prichystanie paketu vyžiadania aktuálnej pozície výťahu */

void activate_Emergency_break(); /*!< Prichystanie paketu bezpečnostného zabrzdenia výťahu. */
private:

	uint8_t * ack_packet_data;

protected:
	SwTimer swtimer;
	int i;
    uint8_t	CRC_Count;
	uint8_t packet_received_to;


    bool areNewDataReady(); /*!< Zisťuje či dorazili nové dáta ak hej potom ich treba spracovať */
    bool areDataToSend(); /*!< Zisťuje či sú prichystané dáta na odoslanie. Ak hej snaží sa ich odoslať */
    bool Was_Ack_delivered(); /*!< Zisťuje či dorazil ack paket. Ak nie snaží sa odoslať ešte raz svoj paket na ktorý nedostal odozvu */
	 packet working_packet;
	 packet working_ack_packet;


	    //! Výpočet správnosti CRC.
	    /*! Paket obsahuje CRC. a zároveň sa z neho vypočíta nové CRC, ktoré musí sedieť s pôvodným v pakete
	     *
	     *
	     * */
	 bool is_CRC_alright(const packet* paPacket);

	    //! Výpočet CRC paketu.
	    /*! Vypočíta sa CRC paketu z adries a dát a vráti sa v návratovej hodnote. */
	 uint8_t count_CRC_of_packet(const packet* paPacket);

	    //! Vytvorenie paketu na odoslanie.
	    /*! Vytvorí sa paket na odoslanie a vloží sa do qeue a pripočíta sa flag data_To_send_count++
	     *  Vďaka, ktorému súbor SendPackets.cpp vie, že má prichystaný paket na odoslanie */
	 void createPacket_for_send(const uint8_t paType_A0_A1,const uint8_t paReceiver,const uint8_t paSize_of_data,const uint8_t* paData);

	    //! Vzopnutie led diody.
	    /*! Prichystá sa paket na vzopnutie led diódy na adrese led_adress_receiver */
 	 void turn_on_led_diod(const uint8_t led_adress_receiver);
	    //! Vypnutie led diody.
	    /*! Prichystá sa paket na vypnutie led diódy na adrese led_adress_receiver */
 	 void turn_off_led_diod(const uint8_t led_adress_receiver);

	    //! Informačný panel výťahu.
	    /*! Nastaví sa na informačný panel výťahu smer a nejaký text väčšinou číslo poschodia kde výťah mieri,
	     *  alebo informacia, že sa resetuje výťah */
 	 void set_information_display(const uint_fast8_t direction, const uint8_t* data_to_display );

	    //! Zamknutie dverí
	    /*! Prichystá sa paket na zamknutie dverí výťahu*/
 	 void lock_elevator_door();
	    //! Odomknutie dverí
	    /*! Prichystá sa paket na odomknutie dverí výťahu*/
 	 void unlock_elevator_door();
	    //! Vypnutie motora
	    /*! Prichystá sa paket na vypnutie motora výťahu*/
 	 void motor_stop();
	    //! Nastavenie rýchlosti
	    /*! Nastaví sa smer direcftion_up a rýchlosť speed_100_pl_min  rozbehne sa výťah*/
 	 void Elevator_settings(const bool direction_up, const int8_t speed_100_pl_min);

	    //! zistenie aktuálnej rýchlosti výťahu
	    /*! Zistí sa aktuálna rýchlosť výťahu*/
 	 void receive_current_speed_from_ElevatorF1();

	    //! Deaktivácia emergency brzdy
	    /*! Deaktivuje sa emergency brzda*/
 	 void deactivate_Emergency_break();
	    //! Reset Časovača watch dogu
	    /*! Po tomto resetovaní sa dá deaktivovať */
 	 void Reset_watch_dog_timer();
 	 //todo

	    //! Funkcia na vypísanie paketu na terminal výťahu
	    /*! Funkcia na vypísanie paketu na terminal výťahu*/
 	 void print_packet_to_elevator_terminal();

};

#endif /* UARTTHREAD_H_ */
