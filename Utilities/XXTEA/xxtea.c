/**
  ******************************************************************************
  * @file    xxtea.c
  * @author  LoopEdison
  * @version V1.0
  * @date    12-December-2016
  * @brief   Utilities for XXTEA
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "xxtea.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DELTA   (0x9e3779b9)
#define MX      (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//==============================================================================
/**
  * @brief  xxtea function 
  * @param  *v: data buff uint32_t type
  * @param  n:  data length ;>0 encrypt; <0 decrypt
  * @param  key:  keys 
  * @retval 0: success
  *         1: error occurred
  */
void btea(uint32_t *v, int n, uint32_t const key[4]) {
	uint32_t y, z, sum;
	unsigned p, rounds, e;
	if (n > 1) {          /* Coding Part */
	  rounds = 6 + 52/n;
	  sum = 0;
	  z = v[n-1];
	  do {
	    sum += DELTA;
	    e = (sum >> 2) & 3;
	    for (p=0; p<n-1; p++) {
	      y = v[p+1]; 
	      z = v[p] += MX;
	    }
	    y = v[0];
	    z = v[n-1] += MX;
	  } while (--rounds);
	} else if (n < -1) {  /* Decoding Part */
	  n = -n;
	  rounds = 6 + 52/n;
	  sum = rounds*DELTA;
	  y = v[0];
	  do {
	    e = (sum >> 2) & 3;
	    for (p=n-1; p>0; p--) {
	      z = v[p-1];
	      y = v[p] -= MX;
	    }
	    z = v[n-1];
	    y = v[0] -= MX;
	    sum -= DELTA;
	  } while (--rounds);
	}
}

/************************ (C) COPYRIGHT LOOPEDISON *********END OF FILE********/
