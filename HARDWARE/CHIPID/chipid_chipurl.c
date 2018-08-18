#include "chipid_chipurl.h"
#include "sys.h"

u32 ChipUniqueID[3];

void Get_ChipID(void)
{

#if 1
	
	ChipUniqueID[2] = *(__IO u32*)(0X1FFFF7E8);  // 
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // 

#else 

    u8 temp[12];   

    u32 temp0,temp1,temp2;

    temp0=*(__IO u32*)(0x1FFFF7E8);    //???????????(96?)

    temp1=*(__IO u32*)(0x1FFFF7EC);

    temp2=*(__IO u32*)(0x1FFFF7F0);

    temp[0] = (u8)(temp0 & 0x000000FF);

    temp[1] = (u8)((temp0 & 0x0000FF00)>>8);

    temp[2] = (u8)((temp0 & 0x00FF0000)>>16);

    temp[3] = (u8)((temp0 & 0xFF000000)>>24);

    temp[4] = (u8)(temp1 & 0x000000FF);

    temp[5] = (u8)((temp1 & 0x0000FF00)>>8);

    temp[6] = (u8)((temp1 & 0x00FF0000)>>16);

    temp[7] = (u8)((temp1 & 0xFF000000)>>24);

    temp[8] = (u8)(temp2 & 0x000000FF);

    temp[9] = (u8)((temp2 & 0x0000FF00)>>8);

    temp[10] = (u8)((temp2 & 0x00FF0000)>>16);

    temp[11] = (u8)((temp2 & 0xFF000000)>>24);

    printf("######## STM32??ID?: %.2X%.2X%.2X%.2X-%.2X%.2X%.2X%.2X-%.2X%.2X%.2X%.2X rn",

  temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7],temp[8],temp[9],temp[10],temp [11]); //???????ID

#endif

}
