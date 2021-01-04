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


//! Štruktúra udalostí.
/*! Tieto udalosti sa používajú v stavovom automate výťahu. Podľa nich vie do ktorého stavu sa má výťah dostať */
struct Events {
	enum {
		GO_TO_FL_P,GO_TO_FL_1,GO_TO_FL_2,GO_TO_FL_3,GO_TO_FL_4	};
};

//! Štruktúra stavov.
/*! Obsahuje jednotlive stavy výťahu. (podľa návrhu závisí koľko ich v programe bude)*/
struct States {
	enum {
		P_FLOOR,FLOOR_1,FLOOR_2,FLOOR_3,FLOOR_4
	};
};

//! Trieda výťahu.
/*!  Dedí zo etl::state_chart pretože používa stavový automat.
*   Dedí tiež z UartThrad pretože využíva nejaké metódy z tej triedy  */
class Elevator_Thread: public UartThread, public etl::state_chart<Elevator_Thread> {
public:

	//! Konštruktor triedy Elevator_Thread .
	/*! Inicializuje konštruktor UartThread, ktorý využíva timer. A konštruktor etl::state_chart
	 *  Inicializuje atribút atQueues_of_packets kde sa nachádza adresa na zásobnik paketov qeue, ktoré sa príjmaju v súbore
	 *  ReceivePackets.cpp */
	Elevator_Thread(packet_queue* PaAueues_of_packets,packet_queue* PaQueue_of_packets_to_Send, SwTimer::callback_f func):etl::state_chart<Elevator_Thread>(*this, transitionTable.data(),
			transitionTable.data() + transitionTable.size(),
			stateTable.data(), stateTable.data() + stateTable.size(),
			States::P_FLOOR),UartThread(func){
			turned_senzor = 0;
		atQueues_of_packets = PaAueues_of_packets;
		//queue_of_pakets_to_send = PaQueue_of_packets_to_Send;

	};

	//packet_queue* queue_of_pakets_to_send;

	//! Virtuálny štart protovlákna.
	/*! Je nutné implementovať túto metódu. Pretože trieda UartThread, z ktorej sa dedí
	 *  Dedí z triedy Protothread.h ktorá ju potrebuje pre jej funkčnosť
	 *
	 *  V tejto metóde beží protovlákno triedy ElevatorThread. V tomto protovlákne beží celá logika výťahu. Ale Prímanie paketov a Odosielanie paketov
	 *  Sa rieši inde. Tu sa pakety iba dešifrujú a podľa ich požiadaviek sa prichystajú nové pakety na odoslanie.
	 *  . */
	virtual bool Run();



	//! Metóda, ktorú volá event stavového automatu GO_TO_FL_P .
	/*! Rozsvietí sa správna signalizačná dióda výťahu buď vnútorná alebo vonkajšia podľa toho kde bolo stlačené tlačidlo
	 *  Nastaví sa príznak where_to_Go[0] = 1; že výťah má ísť na nulté poschodie. Podľa tohto príznaku keď výťah stojí to sa zisťuje v
	 *  ElevatorThreads.cpp v case 0xF1 . Pomocou ktorého sa zisťuje rýchlosť výťahu. Ak výťah stojí a má nastavený jeden z príznakov
	 *  where_to_Go[x] tak tam ide. Na základe súradníc sa zistí, kde to má najvýhodnejšie ísť.
	 *
	 *  Ak sa motor nehýbe, tak sa zavrú dvere a rozbehne sa smerom na ktorý treba ísť podľa aktuálneho poschodia, alebo súradníc.
	 *
	 *  Odošle sa tu aj na informačný display táto voľba .
	 *  */
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
			 elevator_is_moving = true;
			 lock_elevator_door();
			 Elevator_settings(0, 85);

			   uint8_t pom[2] = "P";
			   set_information_display(0x02,pom);
		 }else{
			 find_actual_elevator_position();
			 printf("\n Výťah sa hýbe neni možno privolať\n");
		 }



	}


	//! Metóda, ktorú volá event stavového automatu GO_TO_FL_1 .
		/*! Rozsvietí sa správna signalizačná dióda výťahu buď vnútorná alebo vonkajšia podľa toho kde bolo stlačené tlačidlo
		 *  Nastaví sa príznak where_to_Go[1] = 1; že výťah má ísť na nulté poschodie. Podľa tohto príznaku keď výťah stojí to sa zisťuje v
		 *  ElevatorThreads.cpp v case 0xF1 . Pomocou ktorého sa zisťuje rýchlosť výťahu. Ak výťah stojí a má nastavený jeden z príznakov
		 *  where_to_Go[x] tak tam ide. Na základe súradníc sa zistí, kde to má najvýhodnejšie ísť.
		 *
		 *  Ak sa motor nehýbe, tak sa zavrú dvere a rozbehne sa smerom na ktorý treba ísť podľa aktuálneho poschodia, alebo súradníc.
		 *
		 *  Odošle sa tu aj na informačný display táto voľba .
		 *  */
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
			 elevator_is_moving = true;
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

	//! Metóda, ktorú volá event stavového automatu GO_TO_FL_2 .
		/*! Rozsvietí sa správna signalizačná dióda výťahu buď vnútorná alebo vonkajšia podľa toho kde bolo stlačené tlačidlo
		 *  Nastaví sa príznak where_to_Go[2] = 1; že výťah má ísť na nulté poschodie. Podľa tohto príznaku keď výťah stojí to sa zisťuje v
		 *  ElevatorThreads.cpp v case 0xF1 . Pomocou ktorého sa zisťuje rýchlosť výťahu. Ak výťah stojí a má nastavený jeden z príznakov
		 *  where_to_Go[x] tak tam ide. Na základe súradníc sa zistí, kde to má najvýhodnejšie ísť.
		 *
		 *  Ak sa motor nehýbe, tak sa zavrú dvere a rozbehne sa smerom na ktorý treba ísť podľa aktuálneho poschodia, alebo súradníc.
		 *
		 *  Odošle sa tu aj na informačný display táto voľba .
		 *  */
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
	    	elevator_is_moving = true;
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

	//! Metóda, ktorú volá event stavového automatu GO_TO_FL_3 .
		/*! Rozsvietí sa správna signalizačná dióda výťahu buď vnútorná alebo vonkajšia podľa toho kde bolo stlačené tlačidlo
		 *  Nastaví sa príznak where_to_Go[3] = 1; že výťah má ísť na nulté poschodie. Podľa tohto príznaku keď výťah stojí to sa zisťuje v
		 *  ElevatorThreads.cpp v case 0xF1 . Pomocou ktorého sa zisťuje rýchlosť výťahu. Ak výťah stojí a má nastavený jeden z príznakov
		 *  where_to_Go[x] tak tam ide. Na základe súradníc sa zistí, kde to má najvýhodnejšie ísť.
		 *
		 *  Ak sa motor nehýbe, tak sa zavrú dvere a rozbehne sa smerom na ktorý treba ísť podľa aktuálneho poschodia, alebo súradníc.
		 *
		 *  Odošle sa tu aj na informačný display táto voľba .
		 *  */
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
     		elevator_is_moving = true;
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


	//! Metóda, ktorú volá event stavového automatu GO_TO_FL_4 .
		/*! Rozsvietí sa správna signalizačná dióda výťahu buď vnútorná alebo vonkajšia podľa toho kde bolo stlačené tlačidlo
		 *  Nastaví sa príznak where_to_Go[4] = 1; že výťah má ísť na nulté poschodie. Podľa tohto príznaku keď výťah stojí to sa zisťuje v
		 *  ElevatorThreads.cpp v case 0xF1 . Pomocou ktorého sa zisťuje rýchlosť výťahu. Ak výťah stojí a má nastavený jeden z príznakov
		 *  where_to_Go[x] tak tam ide. Na základe súradníc sa zistí, kde to má najvýhodnejšie ísť.
		 *
		 *  Ak sa motor nehýbe, tak sa zavrú dvere a rozbehne sa smerom na ktorý treba ísť podľa aktuálneho poschodia, alebo súradníc.
		 *
		 *  Odošle sa tu aj na informačný display táto voľba .
		 *  */
	void OnTranTo_Floor_4() {

		PRINTF("%s\n", __func__);

		if(from_inside == false){
			turn_on_led_diod(0x14);
		} else {
			turn_on_led_diod(0x24);
		}
		where_to_Go[4] = 1;


		if(elevator_is_moving == false){
			elevator_is_moving = true;
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

	//! Funkcia, ktorá rozbehne výťah na poschodie P.
	/*! Zamkne dvere výťahu a podľa aktuálnych
	 *  súradníc rozbehne motor výťahu smerom dole. Nastaví spávny smer na informačný display a písmeno P*/
	void goToFloorP();
	//! Funkcia, ktorá rozbehne výťah na poschodie 1.
	/*! Zamkne dvere výťahu a podľa aktuálnych
	*  súradníc rozbehne motor výťahu smerom dole, alebo hore. Nastaví spávny smer na informačný display a číslo 1*/
	void goToFloor1();
	//! Funkcia, ktorá rozbehne výťah na poschodie 2.
	/*! Zamkne dvere výťahu a podľa aktuálnych
	*  súradníc rozbehne motor výťahu smerom dole, alebo hore. Nastaví spávny smer na informačný display a číslo 2*/
	void goToFloor2();
	//! Funkcia, ktorá rozbehne výťah na poschodie 3.
	/*! Zamkne dvere výťahu a podľa aktuálnych
	*  súradníc rozbehne motor výťahu smerom dole, alebo hore. Nastaví spávny smer na informačný display a číslo 3*/
	void goToFloor3();
	//! Funkcia, ktorá rozbehne výťah na poschodie 4.
	/*! Zamkne dvere výťahu a podľa aktuálnych
	*  súradníc rozbehne motor výťahu smerom dole, alebo hore. Nastaví spávny smer na informačný display a číslo 4*/
	void goToFloor4();


	volatile bool where_to_Go[5] = {0}; /*!< Pole zvolených poschodí, ktoré ešte neboli obslúžené */
		volatile uint8_t turned_senzor;
	    bool guard;
	    bool from_inside = false; /*!< Atribút uchováva informáciu, či bolo stlačené tlačidlo z Vonku výťahu, alebo z vnútra výťahu */
	    bool elevator_is_moving = false; /*!< Atribút ktorý hovorí, či sa výťah hýbe  */
	    bool elevator_is_moving_up = true;
	    uint16_t not_Moving_coordinates = 0; /*!< Atribút ktorý uchováva súradnice výťahu ak sa výťah nehýbe */
		bool S1 = false;
		bool S2 = false;
		bool pom = false;

	    //!  Tabuľka tranzicií stavov výťahu
	    /*!  Uchováva možné tranzície z rôznych stavov výťahu do rôznych stavov. Dá sa prejsť z každého do každého stavu
	     *   Okrem pôvodného. teda 5 stavov * 4 = 20 tranzícií*/
		static const etl::array<Elevator_Thread::transition, 20> transitionTable;
	    //! Tabuľka stavov výťahu
	    /*! Uchováva 5 stavov výťahu pre každé poschodie jeden stav */
		static const etl::array<Elevator_Thread::state, 5> stateTable;

	 //! Pracovný paket.
	 /*! S týmto paketom práve pracuje súbor elevatorThread.cpp */
	packet working_Packet;
    //! Adresa zásobníku paketov
    /*! Chystá sa v súbore ReceivePackets.cpp vždy keď v ňom niečo je súbor ElevatorThread.cpp si vyťiahne prijatý paket do
     *  working_Packet. */
	packet_queue* atQueues_of_packets;

    //! Zistenie či je paket pripravený.
    /*! Zisťuje sa či je paket pripravený */
	bool isPacketPrepared();

	bool isAckDone();


	  //! Odoslanie informácie ostávajúcich poschodí na terminál výťahu.
	  /*!   */
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
