/*
 * debug.h
 *
 *  Created on: 20 nov. 2018
 *      Author: jctejero
 */

#ifndef IOT_UTILS_DEBUGLOG_H_
#define IOT_UTILS_DEBUGLOG_H_

/*********************************************************************
*                          Macros Definitions                        *
**********************************************************************/
//#define RELEASE  // Comment to enable debug output
//#include "Arduino.h"

#define DBG_OUTPUT_PORT Serial

//#define	RELEASE

#ifndef RELEASE

	#if defined(ESP866)
		#define DEBUGLOG(...) DBG_OUTPUT_PORT.printf(__VA_ARGS__)
		#define DEBUGLOG_PR(...) DBG_OUTPUT_PORT.printf_P(__VA_ARGS__)
		#define DEBUGLOG_P(fmt, ...) DBG_OUTPUT_PORT.printf_P(PSTR(fmt), ##__VA_ARGS__)
	#elif defined (__AVR_ATmega32C1__) || defined(__AVR_ATmega64C1__) || defined(__AVR_ATmega16M1__) || defined(__AVR_ATmega32M1__) || defined(__AVR_ATmega64M1__) || defined(__AVR_ATmega328P__)
		//#define DEBUGLOG(...)
		#define DEBUGLOG(...)	{char buf[100];sprintf(buf, __VA_ARGS__);DBG_OUTPUT_PORT.print(buf);}
		#define DEBUGLOGLN(...)	{char buf[100];sprintf(buf, __VA_ARGS__);DBG_OUTPUT_PORT.println(buf);}
		#define DEBUGLOG_P(...)	{char buf[100];sprintf(buf, __VA_ARGS__);DBG_OUTPUT_PORT.print(F(buf));}

		#define DEBUG(...)   	DBG_OUTPUT_PORT.print(__VA_ARGS__)
		#define DEBUGLN(...)    DBG_OUTPUT_PORT.println(__VA_ARGS__)
		#define DEBUGF(...)     DBG_OUTPUT_PORT.print(F(__VA_ARGS__))
		#define DEBUGLNF(...)   DBG_OUTPUT_PORT.println(F(__VA_ARGS__)) //Printing text using the F macro
	#endif
/*
#define PROGMEM_P __attribute__((section(".irom.text.template")))
#define DEBUGLOG_P(fmt, ...) \
{ static const char pfmt[] PROGMEM_P = fmt; DBG_OUTPUT_PORT.printf_P(pfmt, ## __VA_ARGS__); }
*/

#else
#define DEBUGLOG(...)
#define DEBUGLOG_PR(...)
#define DEBUGLOG_P(...)
#endif




#endif /* IOT_UTILS_DEBUGLOG_H_ */
