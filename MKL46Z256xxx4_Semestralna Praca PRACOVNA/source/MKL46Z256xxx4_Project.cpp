/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MKL25Z128xxx4_ProtoThreads.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include "etl/delegate.h"
#include "etl/numeric.h"
#include "fsl_lpsci.h"
#include "SwTimer.h"
#include "UartThread.h"
#include "ElevatorThread.h"
#include "ReceivePackets.h"
#include "common.hpp"
#include "SendPackets.h"
#include "fsl_pit.h"
#include "fsl_gpio.h"

#define SYSTICK_MS 10U
#define SYSTICK_TICKS CLOCK_GetFreq(kCLOCK_CoreSysClk)/(1000/(SYSTICK_MS))

bool pitIsrFlag = false;
uint8_t count = 0;
volatile uint64_t glTicks = 0;
volatile uint8_t newDataFlag = 0;
volatile bool packetPrepared = false;
volatile bool newAckFlag = false;
volatile uint8_t last_ack_from = 0;
 //bool packet;
volatile char newChar;
volatile uint8_t data_To_send_count = 0;
uint8_t *packet_data =(uint8_t*) malloc(sizeof(uint8_t) * (DATA_MAX_LENGHT +5));


 SwTimer swtimer1(SwTimer::callback_f::create<getRunTimeInMs>());
 UartThread uartThread(SwTimer::callback_f::create<getRunTimeInMs>());
 ReceivePackets receivePackets(SwTimer::callback_f::create<getRunTimeInMs>());
 Elevator_Thread elevatorThread(&(receivePackets.queues_of_packets), &(uartThread.queue_of_pakets_to_send) ,SwTimer::callback_f::create<getRunTimeInMs>());
 SendPackets sendPackets(SwTimer::callback_f::create<getRunTimeInMs>(),&(elevatorThread.queue_of_pakets_to_send));

uint64_t getRunTimeInMs() {
	return glTicks * SYSTICK_MS;
}

extern "C" void PIT_IRQHandler(){ // prepisuje sa rovnaká funkcia WEAK. ktorá sa nahradí touto kompilátorom. pretože preddefinovaná je WEAK

	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	pitIsrFlag = true;
}

extern "C" void SysTick_Handler() {
	++glTicks;
}

extern "C" void UART0_IRQHandler() {

	if ((kLPSCI_RxDataRegFullFlag) & LPSCI_GetStatusFlags(UART0)) {
			newDataFlag ++;

			LPSCI_ReadBlocking(UART0, packet_data, 5);
			if(packet_data[3]>0) // prečítanie navyše dát
				LPSCI_ReadBlocking(UART0,&packet_data[5] , packet_data[3] )	;

			receivePackets.set_atributess_of_packet( packet_data[0], packet_data[1] ,  packet_data[2], packet_data[3], packet_data[4+packet_data[3]],  &packet_data[4] );

	}
}



int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

#if SDK_DEBUGCONSOLE
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	// Setup Systick
	SysTick_Config(SYSTICK_TICKS);
	EnableIRQ(SysTick_IRQn);

	/* Inicializacia UART0 (LPSCI)*/
	CLOCK_SetLpsci0Clock(0x1U); // Zapnutie hodin tu alebo cez GUI clock wizzard

	lpsci_config_t user_config;
	LPSCI_GetDefaultConfig(&user_config);
	user_config.baudRate_Bps = 230400U; // ako vo výťahu 115200U;
	user_config.enableRx = true;
	user_config.enableTx = true;
	LPSCI_Init(UART0, &user_config, CLOCK_GetPllFllSelClkFreq());
		/* Enable RX interrupt. */
	LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);
	EnableIRQ(UART0_IRQn);

	  /* Structure of initialize PIT */
	        pit_config_t pitConfig;
	        PIT_GetDefaultConfig(&pitConfig);
	        /* Init pit module */
	        PIT_Init(PIT, &pitConfig);
	        /* Set timer period for channel 0 */
	        PIT_SetTimerPeriod(PIT, kPIT_Chnl_0,
	        		USEC_TO_COUNT(500000U, CLOCK_GetBusClkFreq()));
	        /* Enable timer interrupts for channel 0 */
	        PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	        /* Enable at the NVIC */
	        EnableIRQ(PIT_IRQn); // povolenie prerušení
	   // Tu je nakonfigurovaný TIMER.


	printf("ProtoThreads demo\n\r");


	/* Enter an infinite loop, just incrementing a counter. */
	swtimer1.startTimer(1000);
	PIT_StartTimer(PIT, kPIT_Chnl_0);
	while (1) {
		receivePackets.Run();  // vlakno vyskočí vráti sa
		elevatorThread.Run();

		sendPackets.Run();

		if (true == pitIsrFlag){
			count++;
			PIT_StopTimer(PIT, kPIT_Chnl_0);
		    //PRINTF("\r\n Channel No.0 interrupt is occured !");
		    elevatorThread.send_Reset_watchDog();
		    if(count >= 6){
		    	elevatorThread.find_actual_elevator_position();
		    	count =0;
		    }
		    pitIsrFlag = false;
		    PIT_StartTimer(PIT, kPIT_Chnl_0);
		}

	}
	return 0;
}
