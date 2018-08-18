#include "includes.h"

extern uint8_t timer2off_cleaner;

static struct Json_Prase
{
	char *timerflag;
	char *oldtimerflag;
	char *s_decid;
	char *s_seqid;
	int time;
} sjson_prase;

static struct Json_Prase sjson_prase = {NULL,NULL,NULL,NULL,0};

char * makeJson(char* orderword,int type ,char* jdevid, char* quenid ,int code, char* status, char recflg)
{
	cJSON * pJsonRoot = NULL;
	cJSON * pJsonData;
	char * p;

	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
      //error happend here
		return NULL;
	}
	cJSON_AddStringToObject(pJsonRoot, "api", orderword);
	cJSON_AddNumberToObject(pJsonRoot, "type", type);
	cJSON_AddStringToObject(pJsonRoot, "device_sn", jdevid);
	cJSON_AddStringToObject(pJsonRoot, "seqid", quenid);

	pJsonData = NULL;
	pJsonData = cJSON_CreateObject();
	if(NULL == pJsonData)
	{
		// create object faild, exit
		cJSON_Delete(pJsonRoot);
		return NULL;
	}
	cJSON_AddItemToObject(pJsonRoot, "data", pJsonData);
	if (recflg)
	{
		cJSON_AddNumberToObject(pJsonData, "err", code);
		cJSON_AddStringToObject(pJsonData, "msg", status);
	}
	p = cJSON_PrintUnformatted(pJsonRoot);
	if(NULL == p)
	{
      //convert json list to string faild, exit
      //because sub json pSubJson han been add to pJsonRoot, so just delete pJsonRoot, if you also delete pSubJson, it will coredump, and error is : double free
		cJSON_Delete(pJsonRoot);
		return NULL;
	}
	cJSON_Delete(pJsonRoot);
	return p;
}

u8 parseJson(char * pMsg)
{
	
    cJSON * pSub;	
	cJSON * pJson;
	cJSON * pSubData;
	cJSON * pJsonData;
	
	sjson_prase.s_decid = NULL;
	sjson_prase.s_seqid = NULL;
    if(NULL == pMsg)
    {
        return 0;
    }
    pJson = cJSON_Parse(pMsg);
    if(NULL == pJson)                                                                                         
    {
        // parse faild, return
      return 0;
    }

    // get string from json		
	pSub = cJSON_GetObjectItem(pJson, "api");
    if(NULL == pSub)
    {
        //get object named "hello" faild
		cJSON_Delete(pJson);
		return 0;
    }
    printf("obj_1 : %s\n", pSub->valuestring);

	if(!strcmp(pSub->valuestring ,"health"))
	{
		pSub = cJSON_GetObjectItem(pJson, "seqid");
		if(NULL == pSub)
		{
        //get object named "hello" faild
			cJSON_Delete(pJson);
			return 0;
		}
//		sjson_prase.s_seqid = pSub->valuestring;
		pSub = cJSON_GetObjectItem(pJson, "device_sn");
		if(NULL == pSub)
		{
        //get object named "hello" faild
			cJSON_Delete(pJson);
			return 0;
		}
//		sjson_prase.s_decid = pSub->valuestring;
		printf("decid : %s\n", sjson_prase.s_decid);
		cJSON_Delete(pJson);
		return 1;
	}
		
	if(!strcmp(pSub->valuestring ,"subscribe"))
	{
		
		pSub = cJSON_GetObjectItem(pJson, "seqid");
		if(NULL == pSub)
		{
        //get object named "hello" faild
			cJSON_Delete(pJson);
			return 0;
		}
		if(NULL == (sjson_prase.s_seqid = (char *)malloc(strlen(pSub->valuestring)+1)))
		{
			return NULL;
		}
		memset(sjson_prase.s_seqid,0,sizeof((char *)sjson_prase.s_seqid));
		memcpy(sjson_prase.s_seqid,pSub->valuestring,strlen(pSub->valuestring)+1);
	//	sjson_prase.s_seqid = pSub->valuestring;
		printf("s_seqid : %s\n", sjson_prase.s_seqid);
		pSub = cJSON_GetObjectItem(pJson, "device_sn");
		if(NULL == pSub)
		{
        //get object named "hello" faild
			cJSON_Delete(pJson);
			return 0;
		}
		if(NULL == (sjson_prase.s_decid = (char*)malloc(strlen(pSub->valuestring)+1)))
		{
			return NULL;
		}
		memset(sjson_prase.s_decid,0,sizeof((char *)sjson_prase.s_decid));
	//	sjson_prase.s_decid = pSub->valuestring;
		memcpy(sjson_prase.s_decid, pSub->valuestring, strlen(pSub->valuestring)+1);
		printf("s_decid : %s\n",sjson_prase.s_decid);
		pSubData = cJSON_GetObjectItem(pJson, "data");
		if(NULL == pSubData)
		{
        //get object named "hello" faild
			cJSON_Delete(pJson);
			return 0;
		}
		pJsonData = cJSON_GetObjectItem(pSubData, "use_time");
		if(NULL == pJsonData)
		{
        //get object named "hello" faild
			cJSON_Delete(pJson);
			return 0;
		}
		sjson_prase.time = pJsonData->valueint;
		printf("use_time : %d\n", pJsonData->valueint);
		
		pJsonData = cJSON_GetObjectItem(pSubData, "order_sn");
		if(NULL == pJsonData)
		{
        //get object named "hello" faild
			cJSON_Delete(pJson);
			return 0;
		}
		if(sjson_prase.timerflag==NULL&&sjson_prase.oldtimerflag==NULL)
		{
			printf("\n====================malloc timerflag========================\n");
			if(NULL == (sjson_prase.timerflag = (char *)malloc(strlen(pJsonData->valuestring)+1)))
			{
				return NULL;
			}
			memset(sjson_prase.timerflag,0,sizeof((char *)sjson_prase.timerflag));
			
			if(NULL == (sjson_prase.oldtimerflag = (char *)malloc(strlen(pJsonData->valuestring)+1)))
			{
				return NULL;
			}
			memset(sjson_prase.oldtimerflag,0,sizeof((char *)sjson_prase.oldtimerflag));
		}
		memcpy(sjson_prase.timerflag,pJsonData->valuestring,strlen(pJsonData->valuestring)+1);
		printf("=====timeflag : %s\n", sjson_prase.timerflag);
		printf("=====oldtimerflag : %s\n", sjson_prase.oldtimerflag);
		printf("alarm time re%d\n",strcmp(sjson_prase.oldtimerflag,sjson_prase.timerflag));
		if(0 == strcmp(sjson_prase.oldtimerflag,sjson_prase.timerflag))
		{
			printf("alarm time re\n");
			sjson_prase.time = 0;
		}
		cJSON_Delete(pJson);
		printf("=====timeflag : %s\n", sjson_prase.timerflag);
		return 2;
	}
	
//	free(pMsg);
    cJSON_Delete(pJson);
	printf ("=========Leave parse!!!=============\n");
	return 0;
}


char * makeArray(int iSize)
{
    cJSON * root =  cJSON_CreateArray();    
    int i;	
		char * out;	
	
    if(NULL == root)
    {
        printf("create json array faild\n");
        return NULL;
    }
    i = 0;
    
    for(i = 0; i < iSize; i++)
    {
        cJSON_AddNumberToObject(root, "hehe", i);
    }
    out = cJSON_Print(root);
    cJSON_Delete(root);

    return out;
}


void parseArray(char * pJson)
{
    cJSON * root;
	cJSON * pSub;
    int iSize;
	int iValue;	
	int iCnt;	
	
    if(NULL == pJson)
    {                                                                                                
        return ;
    }
    root = NULL;
    if((root = cJSON_Parse(pJson)) == NULL)
    {
        return ;
    }
    iSize = cJSON_GetArraySize(root);
    for(iCnt = 0; iCnt < iSize; iCnt++)
    {
        pSub = cJSON_GetArrayItem(root, iCnt);
        if(NULL == pSub)
        {
            continue;
        }
        iValue = pSub->valueint;
        printf("value[%2d] : [%d]\n", iCnt, iValue);
    }
    cJSON_Delete(root);
    return;
}


uint8_t Data_Parser(void)
{
	int waittime = 200;
	u8 dataflag;
	static int timeval,beeptimer;
	int i,tempCountValue;//,time=0;
	u8 jsondecodeText[400] = {0};
//	char decid[12] = {0};
//	char seqid[20] = {0};
//	int time = 0;
	
	char *phbeat = NULL;
	char *popen = NULL;
	if (USART3_RX_STA!=0 && (USART3_RX_STA & 0x8000)!=1)
		delay_ms(15);
	if(USART3_RX_STA!=0 && (USART3_RX_STA & 0x8000)!=1)
	{
		while(--waittime>0)
		{
			if (USART3_RX_STA!=0 && (USART3_RX_STA & 0x8000)!=1)
				TIM_SetCounter(TIM4,0);
			else
				break;
			delay_ms(5);
		}
	}

	if(Usart_recive_cmpl == 1)
	{	
		printf("\n\n===================recive data is================\n\n%s \n",USART3_RX_BUF);
		if(strstr((const char*)USART3_RX_BUF,"CLOSED")&&strlen((char *)USART3_RX_BUF) == 6)
		{
			set_status_flag(POWERON);
		}
		AES_Decrypt(USART3_RX_BUF , (strlen((char *)USART3_RX_BUF)) , jsondecodeText);
		printf("\n\n=========recive decode data is=====================\n\n %s \n",jsondecodeText);
		dataflag = parseJson(jsondecodeText);
		
		printf("decode flag is %d\n",dataflag);
		if(0 == dataflag)
		{
			USART3_RX_STA = 0;
			Usart_recive_cmpl = 0;
			memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
//			return 0;
		}
		if(1 == dataflag)
		{

		}
		
		if(2 == dataflag)
		{
			printf("Enter set alarm!!!\n");
			printf("time is %d\n",sjson_prase.time);
			tempCountValue = RTC_GetCounter();
			if(0==RELAYOUT&&sjson_prase.time!=0)
			{
				printf("NOW: %d\n",tempCountValue);
				RELAYOUT = 1;
				RELAYBEEP = 1;
				beeptimer = tempCountValue;
				RTC_Alarm_Set(tempCountValue+sjson_prase.time*60);
				printf("ALARM: %d\n",tempCountValue+sjson_prase.time*60);
				timeval = tempCountValue+sjson_prase.time*60;
				sjson_prase.time = 0;
			}
			else if(1==RELAYOUT&&sjson_prase.time!=0)
			{
				RELAYBEEP = 1;
				beeptimer = RTC_GetCounter();
				timeval+=sjson_prase.time*60;
				RTC_Alarm_Set(timeval);
				printf("ALARM: %d\n",timeval);
			}
			memcpy(sjson_prase.oldtimerflag,sjson_prase.timerflag,strlen(sjson_prase.timerflag)+1);
			printf("=====oldtimerflag : %s\n", sjson_prase.oldtimerflag);
			printf("=====s_seqid : %s\n", sjson_prase.s_seqid);
			popen = make_msg("subscribe",2,sjson_prase.s_decid,sjson_prase.s_seqid,0,"success",1);
			m26_send_msg(popen);
			if(sjson_prase.s_decid!=NULL)
				free(sjson_prase.s_decid);
			if(sjson_prase.s_decid!=NULL)
				free(sjson_prase.s_seqid);
			printf("Leave set alarm\n");
		}
		 
		

		USART3_RX_STA = 0;
		memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
	}
	
	Usart_recive_cmpl = 0;
	if(timer2off_cleaner)
	{
		RELAYOUT = 0;
		timer2off_cleaner = 0;
		timeval = 0;
	}
	
	USART3_RX_STA = 0;
	Usart_recive_cmpl = 0;
	
	if((RTC_GetCounter() - beeptimer ) > 2)
	{
		RELAYBEEP = 0;
	}

	return 0;
}


//send json manager
u8* make_msg (char* orderword,int type ,char* mdevid, char* quenid ,int code, char* status, char recflg)
{
	int i=0;
	char sepid[60] = {0};
	int quenrand;
	u8 *expressText = NULL;
//	u8 *cipherText = NULL;
	u8 cipherText[600];
//	u8 *jsontext = NULL;

//	if(NULL == (cipherText = (u8*)malloc(sizeof(char)*400)))
//	{
//		return NULL;
//	}
	
	srand(RTC_GetCounter());    //gSystickMsecCount?????Systick????????
	quenrand = rand()%32767;
	printf("rand %d\n",quenrand);
	memset(cipherText,0,sizeof(cipherText));

	sprintf(sepid,"%s%d","11f1e864-4a12-49b4-a19a-ce",quenrand);
	
	if(!strcmp(quenid," "))
	{
		quenid = sepid;
		printf("quenid %s\n",quenid);
	}
	printf("=========make json================\n");
	
	expressText = makeJson(orderword,type,mdevid,quenid,code,status,recflg);
	printf("=========make json end================\n");
	if (expressText == NULL)
	{
		printf("make json NULL\n");
		return NULL;
	}
		
	printf("\n==============================\n");	
	printf("json text is %s\n", expressText);
	AES_Encrypt(expressText  , strlen((char *)expressText) , cipherText);	//use aesKey encrypt
	free(expressText);
	printf("\n==============================\n");	
	printf("AES_Encrypt data is:   %s\r\n",cipherText);
	 
//	if(NULL == (expressText = (u8*)malloc(sizeof(char)*400)))
//	{
//		return NULL;
//	}
//	memset(expressText ,0 ,strlen((char *)expressText));
//	AES_Decrypt(cipherText , (strlen((char *)cipherText)) , expressText);//use aesKey decrypt
//	printf("\n\n=================nsend aes  data is===============\n\n   %s\r\n\n",expressText);
//	free(expressText);

	return cipherText;
}

