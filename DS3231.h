/**
 * @author  Lukas Herudek
 * @email   lukas.herudek@gmail.com
 * @version v1.0
 * @ide     Atmel Studio 6.2
 * @license GNU GPL v3
 * @brief   RTC with DS3231 for AVR
 * @verbatim
   ----------------------------------------------------------------------
    Copyright (C) Lukas Herudek, 2018
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU General Public License for more details.
	
	<http://www.gnu.org/licenses/>
@endverbatim
 */

#ifndef DS3231_H
#define DS3231_H

#define DS3231_ADDRESS			0b11010000
#define DS3231_WRITE_ADDRESS	0b11010000
#define DS3231_READ_ADDRESS		0b11010001

#define TIME			0x00
#define DATE			0x03
#define CONTROL			0x0E
#define STATUS			0x0F
#define AGING_OFFSET	0x10
#define TEMP			0x11

static volatile struct
{
	volatile unsigned char Hour;
	volatile unsigned char Minute;
	volatile unsigned char Second;
	volatile unsigned char Year;
	volatile unsigned char Month;
	volatile unsigned char Date;
	volatile unsigned char DayOfWeek;
}Time;

static volatile struct
{
	volatile unsigned char Hour;
	volatile unsigned char Minute;
	volatile unsigned char Second;
	volatile unsigned char Year;
	volatile unsigned char Month;
	volatile unsigned char Date;
	volatile unsigned char DayOfWeek;
}TimeBCD;



void RTCinit();
void RTCreadTime();
void RTCreadTimeBCD();
void RTCconvertTimeFromBCD();
void RTCconvertTimeToBCD();
void RTCwriteTimeBCD(unsigned char hours, unsigned char minutes, unsigned char seconds);
void RTCwriteDateBCD(unsigned char DayOfWeek, unsigned char Date, unsigned char Month, unsigned char Year);
void RTCwriteTimeDateBCD(unsigned char hours, unsigned char minutes, unsigned char seconds, unsigned char DayOfWeek, unsigned char Date, unsigned char Month, unsigned char Year);
void RTCwriteTime(unsigned char hours, unsigned char minutes, unsigned char seconds);
void RTCwriteDate(unsigned char DayOfWeek, unsigned char Date, unsigned char Month, unsigned char Year);
void RTCwriteTimeDate(unsigned char hours, unsigned char minutes, unsigned char seconds, unsigned char DayOfWeek, unsigned char Date, unsigned char Month, unsigned char Year);
float RTCreadTemperature();

/*The change in ppm per LSB is different at different temperatures.
The frequency vs. temperature curve is shifted
by the values used in this register. At +25°C, one LSB
typically provides about 0.1ppm change in frequency.*/
void RTCwriteTimeCorrection(unsigned char correction);

#endif