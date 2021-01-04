/*
 * ReceivePackets.h
 *
 *  Created on: 20. 12. 2020
 *      Author: admin
 */

#ifndef RECEIVEPACKETS_H_
#define RECEIVEPACKETS_H_
#include "UartThread.h"

 //! Trieda na príjem paketov a zaradenie na spracovanie do elevatorThread.cpp.
  /*! Táto trieda zisťuje o aký prijatý paket ide. Ak je to Ack paket tak nastaví príznak že prišiel ack Paket takže nieje treba odosielať
   * Rekaputilovane paket výťahu.  Ak je to dátový paket so štartbajtom 0xA0 tak sa uloží do qeue, ktorý používa ElevatorThread.cpp */
class ReceivePackets: public UartThread  {
public:
	ReceivePackets(SwTimer::callback_f func) :UartThread(func){

	};


	virtual bool Run();
};

#endif /* RECEIVEPACKETS_H_ */
