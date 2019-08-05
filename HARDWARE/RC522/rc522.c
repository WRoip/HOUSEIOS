#include "rc522.h"

/*ȫ�ֱ���*/
unsigned char CT[2];			//������
unsigned char SN[4];			//����
unsigned char RFID[16];			//���RFID 
unsigned char lxl_bit		= 0;
uint8_t		  KEY[6]		= {0xff,0xff,0xff,0xff,0xff,0xff};		//���룺Ĭ�ϳ�ʼΪ0xff
uint8_t		  AUDIO_OPEN[6]	= {0xAA, 0x07, 0x02, 0x00, 0x09, 0xBC};
//unsigned char RFID1[16]		= {0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x07,0x80,0x29,0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char status;			//�ƽ�Ѱ������ײ��ѡ�����鿨������
unsigned char s				= 0x08;
uint8_t RC522_state = 0;
uint8_t IC_OK = 0;

/******************* �Զ���ID�� ***************/
unsigned char total		= 0;
unsigned char card1_bit	= 0;
unsigned char card2_bit	= 0;
unsigned char card3_bit	= 0;
unsigned char card4_bit	= 0;
unsigned char lxl[4]	= {196,58,104,217};
unsigned char card_1[4]	= {249,102,49,141};
unsigned char card_2[4]	= {154,48,108,21};
unsigned char card_3[4]	= {10,143,113,21};
unsigned char card_4[4]	= {25,177,180,142};
/*********************************************/

extern void delay_us(u32);
extern void delay_ms(u32);
extern int	printf(const char *,...);

#define   RC522_DELAY()  delay_us( 20 )

 /*
  * ��  �ܣ���ʼ��RC522
  * ��  ������
  * ����ֵ����
  *
 */
void RC522_Init(void)
{
	RC522_Spi_Init();
	Set_RST(1);
	Set_CS(1);
	
	PcdReset ();
	
	M500PcdConfigISOType( 'A' );//���ù�����ʽ
}

void RC522_Handel(unsigned char sector ,unsigned char block ,unsigned char writeBuf[])
{
	uint8_t i=0;
    status = PcdRequest(PICC_REQALL,CT);//Ѱ��
    
    if(status==MI_OK)//Ѱ���ɹ�
    {
         status=MI_ERR;
         status = PcdAnticoll(SN);//����ײ	       
    }

    if (status==MI_OK)//����ײ�ɹ�
    {
        status=MI_ERR;
        if((SN[0]==lxl[0])&&(SN[1]==lxl[1])&&(SN[2]==lxl[2])&&(SN[3]==lxl[3])){
			lxl_bit=1;
			printf("\r\nThe User is:card_0\r\n");
        }else if((SN[0]==card_1[0])&&(SN[1]==card_1[1])&&(SN[2]==card_1[2])&&(SN[3]==card_1[3])){
            card1_bit=1;
            printf("\r\nThe User is:card_1\r\n");
            
        }else if((SN[0]==card_2[0])&&(SN[1]==card_2[1])&&(SN[2]==card_2[2])&&(SN[3]==card_2[3])){
            card2_bit=1;
            printf("\r\nThe User is:card_2\r\n");
        }else if((SN[0]==card_3[0])&&(SN[1]==card_3[1])&&(SN[2]==card_3[2])&&(SN[3]==card_3[3])){
            card3_bit=1;
            printf("\r\nThe User is:card_3\r\n");
        }else if((SN[0]==card_4[0])&&(SN[1]==card_4[1])&&(SN[2]==card_4[2])&&(SN[3]==card_4[3])){
            card4_bit=1;
            printf("\r\nThe User is:card_4\r\n");
        }else{
			printf("ID={%d,%d,%d,%d}\r\n", SN[0], SN[1], SN[2], SN[3]);
		}
        status =PcdSelect(SN);//ѡ��
    }
    else
    {
		//printf("����ײʧ��\r\n");
    }
	
    if(status==MI_OK)//ѡ���ɹ�
    {
        status=MI_ERR;
		status =PcdAuthState(KEYA,(sector*4+3),KEY,SN);//�鿨
        //status =PcdAuthState(KEYA,0x09,KEY,SN);//�鿨
     }
    if(status==MI_OK)//��֤�ɹ�
    {
		IC_OK = 1;
        status=MI_ERR;
        //status=PcdRead(s,RFID); //����
		status=PcdRead((sector*4+block), RFID);

    }

    if(status==MI_OK)//�����ɹ�
    {
        status=MI_ERR;
		printf("RFID=");
		for(i = 0;i < 16; i++){
			printf("%x, ",RFID[i]);
		}
		printf("\r\n");
        delay_ms(100);
    }	

}

void RC522_WriteCardData(unsigned char sector ,unsigned char block ,unsigned char writeBuf[])
{
	char status;
	 
	status = PcdRequest(PICC_REQALL,CT);//Ѱ��
	
	if(status==MI_OK)//Ѱ���ɹ�
    {
         status=MI_ERR;
         status = PcdAnticoll(SN);//����ײ	       
    }
	
	if (status==MI_OK)//����ײ�ɹ�
    {
        status=MI_ERR;
        status =PcdSelect(SN);//ѡ��
    }
	
	if(status==MI_OK)//ѡ���ɹ�
    {
        status=MI_ERR;
		status =PcdAuthState(KEYA,(sector*4+3),KEY,SN);//�鿨
     }
    if(status==MI_OK)//��֤�ɹ�
    {
		IC_OK = 1;
        status=MI_ERR;
        status = PcdWrite((sector*4+block), writeBuf);   // д������buf[0]-buf[16]д��1����0��
		//status=PcdRead((sector*4+block), RFID);

    }

    if(status==MI_OK)//�����ɹ�
    {
		IC_OK = 2;
        status=MI_ERR;
        delay_ms(100);
    }	
}

/*
 * ��������SPI_RC522_SendByte
 * ����  ����RC522����1 Byte ����
 * ����  ��byte��Ҫ���͵�����
 * ����  : RC522���ص�����
 * ����  ���ڲ�����
 */
void SPI_RC522_SendByte ( u8 byte )
{
    u8 counter;
	
    for(counter=0;counter<8;counter++)
    {     
		if ( byte & 0x80 ){
			Set_MOSI(1);
		}else{ 
			Set_MOSI(0);
		}

		RC522_DELAY();

		Set_SCK(0);

		RC522_DELAY();
		 
		Set_SCK(1);

		RC522_DELAY();
		 
		byte <<= 1; 	
    } 
}

/*
 * ��������SPI_RC522_ReadByte
 * ����  ����RC522����1 Byte ����
 * ����  ����
 * ����  : RC522���ص�����
 * ����  ���ڲ�����
 */
u8 SPI_RC522_ReadByte ( void )
{
	u8 counter;
	u8 SPI_Data;

	for(counter=0;counter<8;counter++)
	{
			SPI_Data <<= 1;
	 
			Set_SCK(0);

		    RC522_DELAY();
		
			if ( Red_MISO() == 1)
					SPI_Data |= 0x01;

			RC522_DELAY();

			Set_SCK(1);

			RC522_DELAY();
			
	}
	return SPI_Data;
}

/*
 * ��������SetBitMask
 * ����  ����RC522�Ĵ�����λ
 * ����  ��ucReg���Ĵ�����ַ
 *         ucMask����λֵ
 * ����  : ��
 * ����  ���ڲ�����
 */
void SetBitMask (uint8_t ucReg, uint8_t ucMask )  
{
    uint8_t ucTemp;
    ucTemp = ReadRawRC(ucReg );
    WriteRawRC(ucReg, ucTemp | ucMask);         // set bit mask

}

/*
 * ��������ClearBitMask
 * ����  ����RC522�Ĵ�����λ
 * ����  ��ucReg���Ĵ�����ַ
 *         ucMask����λֵ
 * ����  : ��
 * ����  ���ڲ�����
 */
void ClearBitMask (uint8_t ucReg, uint8_t ucMask)  
{
    uint8_t ucTemp;
    ucTemp = ReadRawRC(ucReg);
    WriteRawRC( ucReg, ucTemp & ( ~ ucMask) );  // clear bit mask
	
}

/*
 * ��������PcdAntennaOn
 * ����  ���������� 
 * ����  ����
 * ����  : ��
 * ����  ���ڲ�����
 */
void PcdAntennaOn (void)
{
    uint8_t uc;
	
    uc = ReadRawRC ( TxControlReg );
    if ( ! ( uc & 0x03 ) )
		SetBitMask(TxControlReg, 0x03);
	
}

/*
 * ��������ReadRawRC
 * ����  ����RC522�Ĵ���
 * ����  ��ucAddress���Ĵ�����ַ
 * ����  : �Ĵ����ĵ�ǰֵ
 * ����  ���ڲ�����
 */
uint8_t ReadRawRC (uint8_t ucAddress)
{
	uint8_t ucAddr, ucReturn;
	
	ucAddr = ( ( ucAddress << 1 ) & 0x7E ) | 0x80;
	Set_CS(0);
	
	SPI_RC522_SendByte( ucAddr );			//SPI_RC522_SendByte ( ucAddr );
	ucReturn = SPI_RC522_ReadByte();		//SPI_RC522_ReadByte ();
	Set_CS(1);

	return ucReturn;
}


/*
 * ��������WriteRawRC
 * ����  ��дRC522�Ĵ���
 * ����  ��ucAddress���Ĵ�����ַ
 *         ucValue��д��Ĵ�����ֵ
 * ����  : ��
 * ����  ���ڲ�����
 */
void WriteRawRC (uint8_t ucAddress, uint8_t ucValue )
{  
	uint8_t ucAddr;

	ucAddr = ( ucAddress << 1 ) & 0x7E;
	Set_CS(0);
	
	SPI_RC522_SendByte ( ucAddr );
	SPI_RC522_SendByte ( ucValue );
	
	Set_CS(1);
}

/*
 * ��������PcdRese
 * ����  ����λRC522 
 * ����  ����
 * ����  : ��
 * ����  ���ⲿ����
 */
void PcdReset (void )
{
    Set_RST(1);
    delay_us ( 1 );
    Set_RST(0);
    delay_us ( 1 );
    Set_RST(1);
    delay_us ( 1 );

    WriteRawRC(CommandReg, 0x0f );
	
    while( ReadRawRC(CommandReg ) & 0x10 ){
		if(RC522_state >= 0xff){
			return;
		}
		RC522_state++;
	}
	
    delay_us( 1 );

    WriteRawRC(ModeReg, 0x3D );				//���巢�ͺͽ��ճ���ģʽ ��Mifare��ͨѶ��CRC����Ϊ0x6363
    WriteRawRC(TReloadRegL, 30 );			//16λ��ʱ����λ    
    WriteRawRC(TReloadRegH, 0 );			//16λ��ʱ����λ
    WriteRawRC(TModeReg, 0x8D );			//�����ڲ���ʱ��������
    WriteRawRC(TPrescalerReg, 0x3E );		//���ö�ʱ����Ƶϵ��
    WriteRawRC(TxAutoReg, 0x40 );			//���Ʒ����ź�Ϊ100%ASK	
	
}

/*
 * ��������M500PcdConfigISOType
 * ����  ������RC522�Ĺ�����ʽ
 * ����  ��ucType��������ʽ
 * ����  : ��
 * ����  ���ⲿ����
 */
void M500PcdConfigISOType(uint8_t ucType)
{
	if ( ucType == 'A')                     //ISO14443_A
	{
		ClearBitMask ( Status2Reg, 0x08 );

		WriteRawRC(ModeReg, 0x3D );//3F
		WriteRawRC(RxSelReg, 0x86 );//84
		WriteRawRC(RFCfgReg, 0x7F );   //4F
		WriteRawRC(TReloadRegL, 30 );//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		WriteRawRC(TReloadRegH, 0 );
		WriteRawRC(TModeReg, 0x8D );
		WriteRawRC(TPrescalerReg, 0x3E );

		delay_us ( 2 );

		PcdAntennaOn ();//������
	}
}

/*
 * ��������PcdComMF522
 * ����  ��ͨ��RC522��ISO14443��ͨѶ
 * ����  ��ucCommand��RC522������
 *         pInData��ͨ��RC522���͵���Ƭ������
 *         ucInLenByte���������ݵ��ֽڳ���
 *         pOutData�����յ��Ŀ�Ƭ��������
 *         pOutLenBit���������ݵ�λ����
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ڲ�����
 */
char PcdComMF522 (uint8_t ucCommand, uint8_t * pInData, uint8_t ucInLenByte, uint8_t * pOutData, uint32_t * pOutLenBit )		
{
    char cStatus		= MI_ERR;
    uint8_t ucIrqEn		= 0x00;
    uint8_t ucWaitFor	= 0x00;
    uint8_t ucLastBits;
    uint8_t ucN;
    uint32_t ul;

    switch( ucCommand )
    {
		case PCD_AUTHENT:			//Mifare��֤
			ucIrqEn   = 0x12;		//���������ж�����ErrIEn  ���������ж�IdleIEn
			ucWaitFor = 0x10;		//��֤Ѱ���ȴ�ʱ�� ��ѯ�����жϱ�־λ
			break;
			
		case PCD_TRANSCEIVE:		//���շ��� ���ͽ���
			ucIrqEn   = 0x77;		//����TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
			ucWaitFor = 0x30;		//Ѱ���ȴ�ʱ�� ��ѯ�����жϱ�־λ�� �����жϱ�־λ
			break;
		 
		default:
			break;
	}
   
    WriteRawRC ( ComIEnReg, ucIrqEn | 0x80 );	//IRqInv��λ�ܽ�IRQ��Status1Reg��IRqλ��ֵ�෴ 
    ClearBitMask ( ComIrqReg, 0x80 );			//Set1��λ����ʱ��CommIRqReg������λ����
    WriteRawRC ( CommandReg, PCD_IDLE );		//д��������
    SetBitMask ( FIFOLevelReg, 0x80 );			//��λFlushBuffer����ڲ�FIFO�Ķ���дָ���Լ�ErrReg��BufferOvfl��־λ�����
    
    for(ul = 0; ul < ucInLenByte; ul ++ ){
		WriteRawRC ( FIFODataReg, pInData [ ul ] );    		//д���ݽ�FIFOdata
	}		
    WriteRawRC( CommandReg, ucCommand );					//д����
   
    if( ucCommand == PCD_TRANSCEIVE ){
		SetBitMask(BitFramingReg,0x80);  				//StartSend��λ�������ݷ��� ��λ���շ�����ʹ��ʱ����Ч
	}
    
    ul = 1000;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
    do 														//��֤ ��Ѱ���ȴ�ʱ��	
    {
		ucN = ReadRawRC ( ComIrqReg );							//��ѯ�¼��ж�
		ul --;
    }while( ( ul != 0 ) && ( ! ( ucN & 0x01 ) ) && ( ! ( ucN & ucWaitFor ) ) );		//�˳�����i=0,��ʱ���жϣ���д��������
		
    ClearBitMask ( BitFramingReg, 0x80 );					//��������StartSendλ
		
	if( ul != 0 ){
		if( ! (( ReadRawRC ( ErrorReg ) & 0x1B )) ){			//�������־�Ĵ���BufferOfI CollErr ParityErr ProtocolErr
			cStatus = MI_OK;
			
			if( ucN & ucIrqEn & 0x01 ){					//�Ƿ�����ʱ���ж�
			  cStatus = MI_NOTAGERR;
			}
				
			if( ucCommand == PCD_TRANSCEIVE )
			{
				ucN = ReadRawRC ( FIFOLevelReg );			//��FIFO�б�����ֽ���
				ucLastBits = ReadRawRC ( ControlReg ) & 0x07;	//�����յ����ֽڵ���Чλ��
				
				if( ucLastBits ){
					* pOutLenBit = ( ucN - 1 ) * 8 + ucLastBits;   	//N���ֽ�����ȥ1�����һ���ֽڣ�+���һλ��λ�� ��ȡ����������λ��
				}else{
					* pOutLenBit = ucN * 8;   					//�����յ����ֽ������ֽ���Ч
				}
				
				if( ucN == 0 ){
                    ucN = 1;
				}					
				
				if( ucN > MAXRLEN ){
					ucN = MAXRLEN;
				}
				
				for( ul = 0; ul < ucN; ul ++ ){
				  pOutData [ ul ] = ReadRawRC ( FIFODataReg );
				}					
			}		
        }else{
			cStatus = MI_ERR;   
		}
    }
   
   SetBitMask ( ControlReg, 0x80 );           // stop timer now
   WriteRawRC ( CommandReg, PCD_IDLE ); 
	
   return cStatus;

}

/*
 * ��������PcdRequest
 * ����  ��Ѱ��
 * ����  ��ucReq_code��Ѱ����ʽ
 *                     = 0x52��Ѱ��Ӧ�������з���14443A��׼�Ŀ�
 *                     = 0x26��Ѱδ��������״̬�Ŀ�
 *         pTagType����Ƭ���ʹ���
 *                   = 0x4400��Mifare_UltraLight
 *                   = 0x0400��Mifare_One(S50)
 *                   = 0x0200��Mifare_One(S70)
 *                   = 0x0800��Mifare_Pro(X))
 *                   = 0x4403��Mifare_DESFire
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdRequest(uint8_t ucReq_code, uint8_t * pTagType)
{
    char		cStatus;  
    uint8_t		ucComMF522Buf[ MAXRLEN ]; 
    uint32_t	ulLen;

    ClearBitMask ( Status2Reg, 0x08 );	//����ָʾMIFARECyptol��Ԫ��ͨ�Լ����п�������ͨ�ű����ܵ����
    WriteRawRC ( BitFramingReg, 0x07 );	//���͵����һ���ֽڵ� ��λ
    SetBitMask ( TxControlReg, 0x03 );	//TX1,TX2�ܽŵ�����źŴ��ݾ����͵��Ƶ�13.56�������ز��ź�

    ucComMF522Buf[0] = ucReq_code;		//���� ��Ƭ������

    cStatus = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &ulLen);	//Ѱ��  

    if ( ( cStatus == MI_OK ) && ( ulLen == 0x10 ) ){	//Ѱ���ɹ����ؿ�����    
       *pTagType = ucComMF522Buf[0];
       *(pTagType + 1) = ucComMF522Buf[1];
    }else{
     cStatus = MI_ERR;
	}
    return cStatus;
}

/*
 * ��������PcdAnticoll
 * ����  ������ײ
 * ����  ��pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdAnticoll(uint8_t * pSnr)
{
    char		cStatus;
    uint8_t		uc, ucSnr_check = 0;
    uint8_t		ucComMF522Buf [ MAXRLEN ]; 
	uint32_t	ulLen;

    ClearBitMask ( Status2Reg, 0x08 );		//��MFCryptol Onλ ֻ�гɹ�ִ��MFAuthent����󣬸�λ������λ
    WriteRawRC ( BitFramingReg, 0x00);		//�����Ĵ��� ֹͣ�շ�
    ClearBitMask ( CollReg, 0x80 );			//��ValuesAfterColl���н��յ�λ�ڳ�ͻ�����
   
    ucComMF522Buf[ 0 ] = 0x93;	//��Ƭ����ͻ����
    ucComMF522Buf[ 1 ] = 0x20;
   
    cStatus = PcdComMF522( PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &ulLen);//�뿨Ƭͨ��
	
    if ( cStatus == MI_OK)		//ͨ�ųɹ�
    {
		for ( uc = 0; uc < 4; uc ++ )
        {
            *( pSnr + uc )  = ucComMF522Buf[ uc ];			//����UID
            ucSnr_check ^= ucComMF522Buf[ uc ];
        }
			
        if ( ucSnr_check != ucComMF522Buf [ uc ] )
			cStatus = MI_ERR;   
    }
    
    SetBitMask ( CollReg, 0x80 );

    return cStatus;
	
}

/*
 * ��������CalulateCRC
 * ����  ����RC522����CRC16
 * ����  ��pIndata������CRC16������
 *         ucLen������CRC16�������ֽڳ���
 *         pOutData����ż�������ŵ��׵�ַ
 * ����  : ��
 * ����  ���ڲ�����
 */
void CalulateCRC (uint8_t * pIndata, uint8_t ucLen, uint8_t * pOutData )
{
    uint8_t uc, ucN;

    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
	
    for ( uc = 0; uc < ucLen; uc ++)
	    WriteRawRC ( FIFODataReg, * ( pIndata + uc ) );   

    WriteRawRC ( CommandReg, PCD_CALCCRC );
	
    uc = 0xFF;
	
    do 
    {
        ucN = ReadRawRC ( DivIrqReg );
        uc --;
    }while( ( uc != 0 ) && ! ( ucN & 0x04 ) );
		
    pOutData [ 0 ] = ReadRawRC ( CRCResultRegL );
    pOutData [ 1 ] = ReadRawRC ( CRCResultRegM );
	
}

/*
 * ��������PcdSelect
 * ����  ��ѡ����Ƭ
 * ����  ��pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdSelect(uint8_t * pSnr )
{
    char ucN;
    uint8_t uc;
	uint8_t ucComMF522Buf [ MAXRLEN ]; 
    uint32_t  ulLen;

    ucComMF522Buf [ 0 ] = PICC_ANTICOLL1;
    ucComMF522Buf [ 1 ] = 0x70;
    ucComMF522Buf [ 6 ] = 0;
	
    for ( uc = 0; uc < 4; uc ++ )
    {
    	ucComMF522Buf [ uc + 2 ] = * ( pSnr + uc );
    	ucComMF522Buf [ 6 ] ^= * ( pSnr + uc );
    }
		
    CalulateCRC ( ucComMF522Buf, 7, & ucComMF522Buf [ 7 ] );
  
    ClearBitMask ( Status2Reg, 0x08 );

    ucN = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, & ulLen );
    
    if ( ( ucN == MI_OK ) && ( ulLen == 0x18 ) )
      ucN = MI_OK;  
    else
      ucN = MI_ERR;    

    return ucN;
	
}

/*
 * ��������PcdAuthState
 * ����  ����֤��Ƭ����
 * ����  ��ucAuth_mode��������֤ģʽ
 *                     = 0x60����֤A��Կ
 *                     = 0x61����֤B��Կ
 *         uint8_t ucAddr�����ַ
 *         pKey������
 *         pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdAuthState (uint8_t ucAuth_mode, uint8_t ucAddr, uint8_t * pKey, uint8_t * pSnr )
{
    char cStatus;
	uint8_t uc, ucComMF522Buf [ MAXRLEN ];
    uint32_t ulLen;

    ucComMF522Buf [ 0 ] = ucAuth_mode;
    ucComMF522Buf [ 1 ] = ucAddr;
	
    for ( uc = 0; uc < 6; uc ++ )
	    ucComMF522Buf [ uc + 2 ] = * ( pKey + uc );   
	
    for ( uc = 0; uc < 6; uc ++ )
	    ucComMF522Buf [ uc + 8 ] = * ( pSnr + uc );   

    cStatus = PcdComMF522 ( PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, & ulLen );
	
    if ( ( cStatus != MI_OK ) || ( ! ( ReadRawRC ( Status2Reg ) & 0x08 ) ) ){
		cStatus = MI_ERR; 
    }
		
    return cStatus;
}

/*
 * ��������PcdRead
 * ����  ����ȡM1��һ������
 * ����  ��u8 ucAddr�����ַ
 *         pData�����������ݣ�16�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdRead (uint8_t ucAddr, uint8_t * pData )
{
    char cStatus;
	uint8_t uc, ucComMF522Buf [ MAXRLEN ]; 
    uint32_t ulLen;

    ucComMF522Buf [ 0 ] = PICC_READ;
    ucComMF522Buf [ 1 ] = ucAddr;
	
    CalulateCRC( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
   
    cStatus = PcdComMF522( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &ulLen );
	
    if ( ( cStatus == MI_OK ) && ( ulLen == 0x90 ) )
    {
		for ( uc = 0; uc < 16; uc ++ )
			*( pData + uc ) = ucComMF522Buf[ uc ];
    }else{
      cStatus = MI_ERR;
	}
	
    return cStatus;

}

//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          pData[IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK         
char PcdWrite(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
 
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
 
    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
 
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
 
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);
 
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
 
    return status;
}
