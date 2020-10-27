/* 
 gurov was here, use this code, or don't, whatever, I don't care. If you see a giant bug with a billion legs, please let me know so it can be squashed

*/

#ifndef INCLUDE_HX711_H
#define INCLUDE_HX711_H

#include <stdio.h>
#include "gb_common.h"
#include <sched.h>
#include <string.h>
#include <stdlib.h>

#define CLOCK_PIN	16
#define DATA_PIN	20
#define N_SAMPLES	64
#define SPREAD		10

#define SCK_ON  (GPIO_SET0 = (1 << CLOCK_PIN))
#define SCK_OFF (GPIO_CLR0 = (1 << CLOCK_PIN))
#define DT_R    (GPIO_IN0  & (1 << DATA_PIN))

void           reset_converter(void);
unsigned long  read_cnt(long offset, int argc);
void           set_gain(int r);
void           setHighPri (void);


#ifdef __cplusplus
extern "C" {
#endif 

long Init(int argc, char **argv) ;
void DeInit(void) ;
long GetData(void) ;


#ifdef __cplusplus
}
#endif


#endif
