/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    boards/addons/ginput/touch/ADS7843/ginput_lld_mouse_board_olimex_stm32_e407.h
 * @brief   GINPUT Touch low level driver source for the ADS7843 on an Olimex STM32E407.
 *
 * @note	This file contains a mix of hardware specific and operating system specific
 *			code. You will need to change it for your CPU and/or operating system.
 */

#ifndef _GINPUT_LLD_MOUSE_BOARD_H
#define _GINPUT_LLD_MOUSE_BOARD_H

// Resolution and Accuracy Settings
#define GMOUSE_ADS7843_PEN_CALIBRATE_ERROR		8
#define GMOUSE_ADS7843_PEN_CLICK_ERROR			6
#define GMOUSE_ADS7843_PEN_MOVE_ERROR			4
#define GMOUSE_ADS7843_FINGER_CALIBRATE_ERROR	14
#define GMOUSE_ADS7843_FINGER_CLICK_ERROR		18
#define GMOUSE_ADS7843_FINGER_MOVE_ERROR		14

static const SPIConfig spicfg = { 
    0,
	GPIOG, 
    10,
    /* SPI_CR1_BR_2 |*/ SPI_CR1_BR_1 | SPI_CR1_BR_0,
};

// How much extra data to allocate at the end of the GMouse structure for the board's use
#define GMOUSE_ADS7843_BOARD_DATA_SIZE			0

static gBool init_board(GMouse* m, unsigned driverinstance) {
	(void)		m;

	if (driverinstance)
		return gFalse;

	spiStart(&SPID2, &spicfg);
	return gTrue;
}

static GFXINLINE gBool getpin_pressed(GMouse* m) {
	(void)		m;

	return (!palReadPad(GPIOG, 0));
}

static GFXINLINE void aquire_bus(GMouse* m) {
	(void)		m;

	spiAcquireBus(&SPID2);
    //TOUCHSCREEN_SPI_PROLOGUE();
    palClearPad(GPIOG, 10);
}

static GFXINLINE void release_bus(GMouse* m) {
	(void)		m;

	palSetPad(GPIOG, 10);
	spiReleaseBus(&SPID2);
    //TOUCHSCREEN_SPI_EPILOGUE();
}

static GFXINLINE uint16_t read_value(GMouse* m, uint16_t port) {
    static uint8_t txbuf[3] = {0};
    static uint8_t rxbuf[3] = {0};
	(void)		m;

    txbuf[0] = port;
    spiExchange(&SPID2, 3, txbuf, rxbuf);
    return ((uint16_t)rxbuf[1] << 5) | (rxbuf[2] >> 3);
}

#endif /* _GINPUT_LLD_MOUSE_BOARD_H */
