/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define SOPT5_UART0RXSRC_UART_RX 0x00u /*!<@brief UART0 Receive Data Source Select: UART_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART0 Transmit Data Source Select: UART0_TX pin */

/*! @name PORTD5 (number 98), LED_GREEN
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_GREEN_FGPIO FGPIOD             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_LED_GREEN_GPIO GPIOD               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LED_GREEN_GPIO_PIN_MASK (1U << 5U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LED_GREEN_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LED_GREEN_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LED_GREEN_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                          /* @} */

/*! @name PORTE29 (number 26), LED_RED
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_RED_FGPIO FGPIOE              /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_LED_RED_GPIO GPIOE                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LED_RED_GPIO_PIN_MASK (1U << 29U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LED_RED_PORT PORTE                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LED_RED_PIN 29U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LED_RED_PIN_MASK (1U << 29U)      /*!<@brief PORT pin mask */
                                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
