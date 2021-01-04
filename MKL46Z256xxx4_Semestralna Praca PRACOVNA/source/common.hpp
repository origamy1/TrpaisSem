/*
 * main.hpp
 *
 *  Created on: 4.1.2021
 *      Author: Ladislav Imrišek
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_
#include <stdint.h>

uint64_t getRunTimeInMs();
extern volatile char newChar;
extern volatile uint8_t newDataFlag; //! Príznak počtu aktuálne prijatých dát, ktoré sa môžu ešte spracovávať.
/*! Každý nový prijatý paket sa sem pripočíta a potom sa spracováva v súbore ReceivePackets.cpp. Kde sa čaká pomocou
 *  funkcie PT_WAIT_UNTIL(areNewDataReady()); či už sa môže paket spracovávať */
extern volatile bool packetPrepared; //! Príznak pripravenia paketu pre ďalšie spracovanie
/*! V súbore ReceivePackets.cpp sa pripraví ďalší paket na spracovanie. Ak je pripravený označí sa ako true.
 *  Súbor ElevatorThread.cpp vlákno čaká na jeho pripravenosť pomocou PT_WAIT_UNTIL(isPacketPrepared());.
 * */
extern unsigned char crc_array[256]; /*!< Pole CRC maxim-8 konštánt */
extern volatile bool newAckFlag ; //! Príznak, že dorazil acknowledge paket.
/*! Keď sa v súbore ReceivePackets.cpp zistí, že dorazil acknowledge paket tak sa označí tento príznak.
 *  Následne súbor SendPackets.cpp, ktorý vysiela pakety po uarte vie, pomocou funkcie PT_WAIT_UNTIL(Was_Ack_delivered() || swtimer.isExpired() );  že
 *  Acknowledge paket dorazil. Ak nedorazil, alebo mu to dlho trvalo, sa môže sa pokúsiť vyslať paket znova.
 */
extern volatile uint8_t data_To_send_count ; //! Príznak koľko paketov treba momentálne odoslať.
/*! Počas spracovávania paketov a požiadaviek od výťahu v súbore ElevatorThread.cpp sa do tejto premennej postupne pridávajú požiadavky
 *  (pakety). Vždy keď je paket pripravený na odoslanie sa navýši táto premenná.
 *  Táto premenná sa nastavuje v celom kóde len v metóde "createPacket_for_send" triedy "UartThread". Túto metódu volá vo väčšine prípadov len v súbore
 *  ElevatorThread.cpp kde sa pripravujú pakety na odoslanie podľa požiadaviek od prijatých paketov.
 *  Pomocou tejto funkcie PT_WAIT_UNTIL(areDataToSend()); sa v Súbore SendPackets.cpp zisťuje či už sú nejaké pakety na odoslanie pripravené. Ak áno odošlú sa.
 * */
extern volatile uint8_t last_ack_from; /*!< Kontrolný príznak, značí odosielateľa posledného prijatého acknowledge paketu. */
#endif /* COMMON_HPP_ */
extern uint8_t *packet_data;
extern bool activateBreak; //! Príznak či bola brzda aktivovaná
/*!
 *  Pri detekcií voľného pádu v Maine sa nastaví na True po aktivácií.
 *  Na false sa deaktivuje pri odblokovaní z terminálu výťahu v súbore ElevátorThread.c  keď príde paket z 0xD0:
 *  Ak bolo napísané v odosielacom poli 'E'.
 * */






