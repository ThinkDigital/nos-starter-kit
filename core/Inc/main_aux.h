/*
 * main_aux.h
 *
 *  Created on: Nov 13, 2018
 *      Author: jsilva
 */

#ifndef INC_MAIN_AUX_H_
#define INC_MAIN_AUX_H_



#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


#endif /* INC_MAIN_AUX_H_ */
