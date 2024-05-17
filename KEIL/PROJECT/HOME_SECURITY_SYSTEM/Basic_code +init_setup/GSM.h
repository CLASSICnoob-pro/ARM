#ifndef __GSM_H__
#define __GSM_H__

#include"lpc21xx.h"
#include"types.h"
#include"delay.h"
#include"uart0.h"
#include"stdlib.h"

//#define NUM "+917987648081"

void GSM_SMS(u8* MSG);
void GSM_OTP(u8* otp);

#endif
