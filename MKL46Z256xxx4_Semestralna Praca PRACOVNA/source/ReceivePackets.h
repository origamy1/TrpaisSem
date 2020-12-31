/*
 * ReceivePackets.h
 *
 *  Created on: 20. 12. 2020
 *      Author: admin
 */

#ifndef RECEIVEPACKETS_H_
#define RECEIVEPACKETS_H_
#include "UartThread.h"


class ReceivePackets: public UartThread  {
public:
	ReceivePackets(SwTimer::callback_f func) :UartThread(func){

	};


	virtual bool Run();
};

#endif /* RECEIVEPACKETS_H_ */
