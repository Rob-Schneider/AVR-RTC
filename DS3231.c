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
 
 
#include "I2C.h" 
#include "DS3231.h"


void RTCinit()
{
	I2Cinit();
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(CONTROL);
	I2Cwrite(0b00000000);//Oscillator ON, 1 Hz output pin
	I2Cstop();
}


void RTCreadTime()
{
	unsigned char data;
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(TIME);
	
	I2Cstart();
	I2Cwrite(DS3231_READ_ADDRESS);
	data = I2CreadACK();
	Time.Second = ((data&0x70)>>4)*10 + (data&0x0F);
	data = I2CreadACK();
	Time.Minute = ((data&0x70)>>4)*10 + (data&0x0F);
	data = I2CreadACK();
	Time.Hour = ((data&0x30)>>4)*10 + (data&0x0F);
	
	Time.DayOfWeek = I2CreadACK();
	
	data = I2CreadACK();
	Time.Date = ((data&0x30)>>4)*10 + (data&0x0F);
	data = I2CreadACK();
	Time.Month = ((data&0x10)>>4)*10 + (data&0x0F);
	data = I2CreadNACK();
	Time.Year = ((data&0xF0)>>4)*10 + (data&0x0F);
	I2Cstop();
}

void RTCreadTimeBCD()
{
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(TIME);
	
	I2Cstart();
	I2Cwrite(DS3231_READ_ADDRESS);
	TimeBCD.Second = I2CreadACK();
	TimeBCD.Minute = I2CreadACK();
	TimeBCD.Hour   = I2CreadACK()&0x3F;
	
	TimeBCD.DayOfWeek = I2CreadACK();
	
	TimeBCD.Date  = I2CreadACK();
	TimeBCD.Month = I2CreadACK()&0x1F;
	TimeBCD.Year  = I2CreadNACK();
	I2Cstop();
}

void RTCconvertTimeFromBCD()
{
	Time.Second = ((TimeBCD.Second&0x70)>>4)*10 + (TimeBCD.Second&0x0F);
	Time.Minute = ((TimeBCD.Minute&0x70)>>4)*10 + (TimeBCD.Minute&0x0F);
	Time.Hour = ((TimeBCD.Hour&0x30)>>4)*10 + (TimeBCD.Hour&0x0F);
	
	Time.DayOfWeek = TimeBCD.DayOfWeek;
	Time.Date = ((TimeBCD.Date&0x30)>>4)*10 + (TimeBCD.Date&0x0F);
	Time.Month = ((TimeBCD.Month&0x10)>>4)*10 + (TimeBCD.Month&0x0F);
	Time.Year = ((TimeBCD.Year&0xF0)>>4)*10 + (TimeBCD.Year&0x0F);
}

void RTCconvertTimeToBCD()
{
	TimeBCD.Second = ((Time.Second/10)<<4) | (Time.Second%10);
	TimeBCD.Minute = ((Time.Minute/10)<<4) | (Time.Minute%10);
	TimeBCD.Hour = ((Time.Hour/10)<<4) | (Time.Hour%10);
	TimeBCD.DayOfWeek = Time.DayOfWeek;
	TimeBCD.Date = ((Time.Date/10)<<4) | (Time.Date%10);
	TimeBCD.Month = ((Time.Month/10)<<4) | (Time.Month%10);
	TimeBCD.Year = ((Time.Year/10)<<4) | (Time.Year%10);
}


void RTCwriteTimeBCD(unsigned char hours, unsigned char minutes, unsigned char seconds)
{
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(TIME);
	I2Cwrite(seconds);
	I2Cwrite(minutes);
	I2Cwrite(hours);
	I2Cstop();
}

void RTCwriteDateBCD(unsigned char DayOfWeek, unsigned char Date, unsigned char Month, unsigned char Year)
{
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(Date);
	I2Cwrite(DayOfWeek);
	I2Cwrite(Date);
	I2Cwrite(Month);
	I2Cwrite(Year);
	I2Cstop();
}

void RTCwriteTimeDateBCD(unsigned char hours, unsigned char minutes, unsigned char seconds, unsigned char DayOfWeek, unsigned char Date, unsigned char Month, unsigned char Year)
{
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(TIME);
	I2Cwrite(seconds);
	I2Cwrite(minutes);
	I2Cwrite(hours);
	I2Cwrite(DayOfWeek);
	I2Cwrite(Date);
	I2Cwrite(Month);
	I2Cwrite(Year);
	I2Cstop();
}

void RTCwriteTime(unsigned char hours, unsigned char minutes, unsigned char seconds)
{
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(TIME);
	I2Cwrite(((seconds/10)<<4) | (seconds%10));
	I2Cwrite(((minutes/10)<<4) | (minutes%10));
	I2Cwrite(((hours/10)<<4) | (hours%10));
	I2Cstop();
}

void RTCwriteDate(unsigned char DayOfWeek, unsigned char Date, unsigned char Month, unsigned char Year)
{
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(Date);
	I2Cwrite(DayOfWeek);
	I2Cwrite(((Date/10)<<4) | (Date%10));
	I2Cwrite(((Month/10)<<4) | (Month%10));
	I2Cwrite(((Year/10)<<4) | (Year%10));
	I2Cstop();
}

void RTCwriteTimeDate(unsigned char hours, unsigned char minutes, unsigned char seconds, unsigned char DayOfWeek, unsigned char Date, unsigned char Month, unsigned char Year)
{
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(TIME);
	I2Cwrite(((seconds/10)<<4) | (seconds%10));
	I2Cwrite(((minutes/10)<<4) | (minutes%10));
	I2Cwrite(((hours/10)<<4) | (hours%10));
	I2Cwrite(DayOfWeek);
	I2Cwrite(((Date/10)<<4) | (Date%10));
	I2Cwrite(((Month/10)<<4) | (Month%10));
	I2Cwrite(((Year/10)<<4) | (Year%10));
	I2Cstop();
}

float RTCreadTemperature()
{
	unsigned char tempH, tempL;
	
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(TEMP);
	
	I2Cstart();
	I2Cwrite(DS3231_READ_ADDRESS);
	tempH = I2CreadACK();
	tempL = I2CreadNACK();
	I2Cstop();
	
	return (float)(tempH+(tempL>>6)*0.25);
}


/*The change in ppm per LSB is different at different temperatures.
The frequency vs. temperature curve is shifted
by the values used in this register. At +25°C, one LSB
typically provides about 0.1ppm change in frequency.*/
void RTCwriteTimeCorrection(unsigned char correction) 
{
	I2Cstart();
	I2Cwrite(DS3231_WRITE_ADDRESS);
	I2Cwrite(AGING_OFFSET);
	I2Cwrite(correction);//The code is encoded in two’s complement, with bit 7 representing the sign bit.
	I2Cstop();
}


