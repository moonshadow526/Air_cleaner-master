/******************** (C) COPYRIGHT 2013 EADingCore ********************
 * File Name          	: aes_crypt.h
 * Author            	: zhujun
 * Version            	: V1.0
 * Date               	: 9/4/2013
 * Description        	: AES加密相关
 ********************************************************************************/
#ifndef _AES_CRYPT_H_
#define _AES_CRYPT_H_

#define AES_DEBUG 1

#if AES_DEBUG
#define AES_DEBUG_MSG _printf
#else  
#define AES_DEBUG_MSG NO_AES_DEBUG
#endif

int AES_Encrypt(unsigned char* inputs, int len, unsigned char* outputs);  //AES加密
int AES_Decrypt(unsigned char* inputs, int len, unsigned char* outputs);  //AES解密

void NO_AES_DEBUG(const char* msg, ...);
#endif
