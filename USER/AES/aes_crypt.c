/******************** (C) COPYRIGHT 2013 EADingCore ********************
 * File Name          	: aes_crypt.c
 * Author            	: zhujun
 * Version            	: V1.0
 * Date               	: 9/4/2013
 * Description        	: AES加密相关，加密采用128位AES/CBC/pkcs5padding,
 *                       加密结果用base64重新编码。
 ********************************************************************************/
#include <stm32f10x.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "aes_crypt.h"
#include "aes.h"
//#include "uart.h"
static unsigned char aeskey[16] = { 'e', 'a', 'd', 'i', 'n', 'g', 'c', 'o', 'r', 'e', '1', '4', '0', '1', '1', '4' }; /*密钥，必须16位*/
static unsigned char Vector[16] = { 'c', '1', '5', 'e', 'e', '9', '8', '1', 'a', '6', 'c', 'd', '8', '1', '0', '7' }; /*密钥偏移量，必须16位*/

static void Pkcs5_Encode(unsigned char* indata, int inlen, unsigned char* outdata, int* outlen);
static void Pkcs5_Decode(unsigned char* indata, int inlen, unsigned char* outdata, int* outlen);
static void _base64_encode(const unsigned char *src, int len, int *out_len, unsigned char* out);
static unsigned char * _base64_decode(const unsigned char *src, int len, int *out_len);

/****************************************************************************************
 ** 函数名称: AES_Init
 ** 功能描述: 开启AES硬件处理器时钟，STM32F205无此功能
 ** 参数    : 
 ** 返回值  : 
 ** 作者    : zhujun
 ** 日期    : 2013-09-04        
 ****************************************************************************************/
/*void AES_Init(void)
{
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_CRYP, ENABLE);	// AES 时钟;
}*/

/****************************************************************************************
 ** 函数名称: AES_Encrypt
 ** 功能描述: AES加密数据
 ** 参数    : inputs，输入数据,应为带有边界的地址空间
 ** 			     len，源数据长度
 **           outputs，输出数据
 ** 返回值  : 编码后数据长度
 ** 作者    : zhujun
 ** 日期    : 2013-09-04        
 ****************************************************************************************/
/****************************************************************************************
 **变更记录
 **         : 2013-09-08  zhujun 
 **           更换MCU为stm32f205时，无硬件加密处理器，AES需要软件实现。采用OpenSSL提供的开源代码。
 ****************************************************************************************/

int AES_Encrypt(unsigned char* inputs, int len, unsigned char* outputs)
{
    AES_KEY key;
    int out_len;
    unsigned char *res = outputs;
    unsigned char *res_pkcs = NULL, *res_aes = NULL;
    unsigned char i, iv[16];
    for(i = 0; i < 16; i++)                      //OpenSSL的AES-CBC使用时会更改原始的偏移量，这里将偏移量复制到新的变量
        iv[i] = Vector[i];
   
    
    if((0 == inputs) || (len<=0))
    {
        outputs = 0;
        out_len = -1;
        printf("AES_Encrypt  Inputs Data is NULL\r\n");
        goto EXIT;
    }
    
    /*pkcs5padding*/
    res_pkcs = malloc(len + 32);
    if(0 == res_pkcs)
    {
        outputs = 0;
        out_len = -1;
        printf("res_pkcs malloc error!\r\n");
        goto EXIT;
    }
    Pkcs5_Encode(inputs, len, res_pkcs, &out_len);
    if(0 == res_pkcs)
    {
        outputs = 0;
        out_len = -1;
        printf("pkcs5_Encode error!\r\n");
        goto EXIT;
    }
    
    /*aes_cbc encoding*/
    res_aes = malloc(out_len + 32);
    if(0 == res_aes)
    {
        outputs = 0;
        out_len = -1;
        printf("res_aes malloc error!\r\n");
        goto EXIT;
    }
    AES_set_encrypt_key(aeskey, 128, &key);
    //CRYP_AES_CBC(MODE_ENCRYPT, Vector, aeskey, 128, res_pkcs, out_len, res_aes);       //stm32f215
    AES_cbc_encrypt(res_pkcs, res_aes, out_len, &key, iv, AES_ENCRYPT);                  //OpenSSl

    /*base64 encoding*/
     _base64_encode(&res_aes[0], out_len, &out_len, &res[0]);
    if(0 == res)
    {
        outputs = 0;
        out_len = -1;
        printf("base64 error!\r\n");
        goto EXIT;
    }
    
EXIT:  
    free(res_pkcs);
    free(res_aes);
    res_pkcs = NULL;
    res_aes= NULL;
    return out_len;
}

/****************************************************************************************
 ** 函数名称: AES_Decrypt
 ** 功能描述: AES解密数据
 ** 参数    : inputs，输入数据，应为带有边界的地址空间
 ** 		  len，源数据长度
 **           outputs，输出数据
 ** 返回值  : 编码后数据长度
 ** 作者    : zhujun
 ** 日期    : 2013-09-04
 ****************************************************************************************/
/****************************************************************************************
 **变更记录
 **         : 2013-09-08  zhujun 
 **           更换MCU为stm32f205时，无硬件加密处理器，AES需要软件实现。采用OpenSSL提供的开源源码。
 ****************************************************************************************/
int AES_Decrypt(unsigned char* inputs, int len, unsigned char* outputs)
{
    AES_KEY key;
    int outlen;
    unsigned char* res_base64 = NULL;
    unsigned char* res_aes = NULL;
    unsigned char i, iv[16];
    for(i = 0; i < 16; i++)                  //OpenSSL的AES-CBC使用时会更改原始的偏移量，这里将偏移量复制到新的变量
        iv[i] = Vector[i];
    
    if((0 == inputs) || (len<=0))
    {
        outputs = 0;
        outlen = -1;
       //AES_DEBUG_MSG("AES_Decrypt  Inputs Data is NULL\r\n");
        goto EXIT;
    }
    
    /*base64 decoding*/
    res_base64 = _base64_decode(inputs, len, &outlen); 
    if(0 == res_base64)
    {
        outputs = 0;
        outlen = -1;
        //AES_DEBUG_MSG("Base64 Format Error!\r");
        goto EXIT;
    }
    
    /*aes_cbc decodng*/
    res_aes = malloc(outlen + 32);
    if(0 == res_aes)
    {
        outputs = 0;
        outlen = -1;
        //AES_DEBUG_MSG("res_aes malloc error!\r");
        goto EXIT;
    }
    memset(res_aes, 0, outlen + 32);
    //CRYP_AES_CBC(MODE_DECRYPT, Vector, aeskey, 128, res_base64, outlen, res_aes);         //stm32f215
    AES_set_decrypt_key(aeskey, 128, &key);                                                 //OpenSSl
    AES_cbc_encrypt(res_base64, res_aes, outlen, &key, iv, AES_DECRYPT);
    
    /*pkcs5ping decoding*/
    Pkcs5_Decode(res_aes, strlen((const char*) res_aes), outputs, &outlen);
    if(outlen == -1)
    {
        //AES_DEBUG_MSG("Pkcs5_Decode error!\r");
        outputs = 0;
        outlen = -1;
        goto EXIT;
    }
    
EXIT: 
    free(res_base64);
    free(res_aes);
    res_base64 = NULL;
    res_aes = NULL;
    return outlen;
}

/****************************************************************************************
 ** 函数名称: Pkcs5_Encode
 ** 功能描述: pkcs5padding补码
 ** 参数    : indata，输入数据
 ** 		  inlen，输入数据长度
 **           outdata，输出数据
 **           outlen，输出数据长度的指针
 ** 返回值  :
 ** 作者    : zhujun
 ** 日期    : 2013-09-04
 ****************************************************************************************/
static void Pkcs5_Encode(unsigned char* indata, int inlen, unsigned char* outdata, int* outlen)
{
    int i;
    unsigned char *data = outdata;

    for(i = 0; i < inlen; i++)
    {
        data[i] = indata[i];
    }
    if(inlen % 16 == 0)
    {
        for(i = 0; i < 16; i++)
        {
            data[inlen + i] = 16;
        }
        data[inlen + 16] = '\0';
        *outlen = inlen + 16;
    }
    else
    {
        for(i = 0; i < (16 - inlen % 16); i++)
        {
            data[inlen + i] = 16 - inlen % 16;
        }
        data[inlen + i] = '\0';
        *outlen = inlen + i;
    }
}

/****************************************************************************************
 ** 函数名称: Pkcs5_Decode
 ** 功能描述: pkcs5padding获得原码
 ** 参数    : indata，输入数据
 ** 		  inlen，输入数据长度
 **           outdata，输出数据
 **           outlen，输出数据长度的指针
 ** 返回值  :
 ** 作者    : zhujun
 ** 日期    : 2013-09-04
 ****************************************************************************************/
static void Pkcs5_Decode(unsigned char* indata, int inlen, unsigned char* outdata, int* outlen)
{
    int i, count = indata[inlen - 1];
    unsigned char* data = outdata;
    if((count > 16) || (count > inlen))
    {
        outdata = 0;
        *outlen = -1;
        return;
    }
    for(i = 0; i < inlen - count; i++)
    {
        data[i] = indata[i];
    }
    data[inlen - count] = 0;
    *outlen = inlen - count;
}

/*BASE64表*/
static const unsigned char base64_table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/****************************************************************************************
 ** 函数名称: _base64_encode
 ** 功能描述: BASE编码
 ** 参数    : src，输入数据
 ** 			     len，输入数据长度
 **           out_len，输出数据长度的指针
 ** 返回值  : 输出数据
 ** 作者    : zhujun
 ** 日期    : 2013-09-04
 ****************************************************************************************/
static void _base64_encode(const unsigned char *src, int len, int *out_len, unsigned char* out)
{
    unsigned char /**out,*/ *pos=0;
    const unsigned char *end=0, *in=0;
    size_t olen;

    olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
    olen += olen / 72; /* line feeds */
    olen++; /* nul termination */
    if(out == NULL)
        return;

    end = src + len;
    in = src;
    pos = out;
    while(end - in >= 3)
    {
        *pos++ = base64_table[in[0] >> 2];
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = base64_table[in[2] & 0x3f];
        in += 3;
    }

    if(end - in)
    {
        *pos++ = base64_table[in[0] >> 2];
        if(end - in == 1)
        {
            *pos++ = base64_table[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
        else
        {
            *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
            *pos++ = base64_table[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
    }

    *pos = '\0';
    if(out_len)
        *out_len = pos - out;
    
    return;
}

/****************************************************************************************
 ** 函数名称: _base64_encode
 ** 功能描述: BASE反编码
 ** 参数    : src，输入数据
 ** 			     len，输入数据长度
 **           out_len，输出数据长度的指针
 ** 返回值  : 输出数据
 ** 作者    : zhujun
 ** 日期    : 2013-09-04
 ****************************************************************************************/
unsigned char * _base64_decode(const unsigned char *src, int len, int *out_len)
{
    unsigned char dtable[256], *out, *pos, in[4], block[4], tmp;
    size_t i, count;

    memset(dtable, 0x80, 256);
    for(i = 0; i < sizeof(base64_table); i++)
        dtable[base64_table[i]] = i;
    dtable['='] = 0;

    count = 0;
    for(i = 0; i < len; i++)
    {
        if(dtable[src[i]] != 0x80)
            count++;
    }

    if(count % 4)
        return NULL;

    pos = out = malloc(count);
    if(out == NULL)
        return NULL;

    count = 0;
    for(i = 0; i < len; i++)
    {
        tmp = dtable[src[i]];
        if(tmp == 0x80)
            continue;

        in[count] = src[i];
        block[count] = tmp;
        count++;
        if(count == 4)
        {
            *pos++ = (block[0] << 2) | (block[1] >> 4);
            *pos++ = (block[1] << 4) | (block[2] >> 2);
            *pos++ = (block[2] << 6) | block[3];
            count = 0;
        }
    }

    if(pos > out)
    {
        if(in[2] == '=')
            pos -= 2;
        else if(in[3] == '=')
            pos--;
    }

    *out_len = pos - out;
    return out;
}

void NO_AES_DEBUG(const char* msg, ...)
{
    return;
}
/*-@FILE END@-*/
