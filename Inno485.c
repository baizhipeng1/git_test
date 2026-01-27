


#include "Inno485.h"
#include "PCF8563.h"
#include "main.h"




//-----通信的数据格式编辑----//

const u8 InnoHeadInfo[4] = {0xAA, 0xAA, 0xAA, 0xAA};         //数据头内容
const u8 InnoHeadLen = 4;                                    //数据头长度

const u8 InnoEndInfo[4] = {0x55, 0x55, 0x55, 0x55};          //数据尾内容
const u8 InnoEndLen = 4;                                     //数据尾长度

const u8 InnoSignInfo[2] = {0x01, 0x02};                     //标识符内容


//----通信的数据地址编辑----//
//只读的运行数据：内容和长度
u8 *InnoDeRunDef = (u8 *)(&ActualRunPara);
u16 InnoDeRunLen = sizeof(ActualRunPara);
//可读可写的参数数据：内容和长度
u8 *InnoDeParaDef = (u8 *)(&DePara);
u16 InnoDeParaLen = sizeof(DePara);


//----遥测命令的相关参数----//
InnoTestType InnoUpTest;
u8 InnoCount = 0;



static const u8 aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const u8 aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

//*************************************************************/
//函数名称: usMBCRC16
//功    能: CRC校验函数
//输    入: pucFrame：数组内容
//          usLen：数组长度
//输    出: 计算后的CRC校验
//备    注: 无
//*************************************************************/
u16 usMBCRC16( u8 * pucFrame, u16 usLen )
{
    u8 ucCRCHi = 0xFF;
    u8 ucCRCLo = 0xFF;
    int iIndex;

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( u8 )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( u16 )( ucCRCHi << 8 | ucCRCLo );
}









//--------------对外函数---------------//
//*************************************************************/
//函数名称: Inno_ReDataJudge
//功    能: 判断接收到的数据是否为内部通信的格式
//输    入: ReBuf：接收到的数据数组
//          ReLen：接收到的数据长度
//输    出: 0-是
//          1-否
//备    注: 无
//*************************************************************/
u8 Inno_ReDataJudge(u8 * ReBuf,u16 ReLen)
{
    u16 i;

    //----判断数据的合理性----//
    //总长度
    if (ReLen < (InnoHeadLen+InnoEndLen+2+6))       //最少的字符数--头、尾、标识符、MODBUS的最短长度
    {
        return 1;
    }

    //数据头
    for (i=0;i<InnoHeadLen;i++)
    {
        if (ReBuf[i] != InnoHeadInfo[i])
        {
            return 1;
        }
    }

    //标识符
    for (i=0;i<2;i++)
    {
        if (ReBuf[i+InnoHeadLen] != InnoSignInfo[i])
        {
            return 1;
        }
    }

    //数据尾
    for (i=0;i<InnoEndLen;i++)
    {
        if (ReBuf[ReLen-i-1] != InnoEndInfo[InnoEndLen-i-1])
        {
            return 1;
        }
    }

    return 0;
}

//*************************************************************/
//函数名称: Inno_ReModBusJudge
//功    能: 判断接收到的数据是否符合modbus通信的结构
//输    入: ReBuf：接收到的数据数组
//          ReLen：接收到的数据长度
//输    出: 0-是
//          1-否
//备    注: 无
//*************************************************************/
u8 Inno_ReModBusJudge(u8 * ReBuf,u16 ReLen)
{
    u16 NowPosi;
    u16 CrcLen;

    NowPosi = InnoHeadLen+2;

    //从机地址
    if (ReBuf[NowPosi] != INNO_MODSADDR)
    {
        return 1;
    }

    //功能码
    if ((ReBuf[NowPosi+1] != INNO_MODR_HOLD) && \
        (ReBuf[NowPosi+1] != INNO_MODR_INPUT) && \
        (ReBuf[NowPosi+1] != INNO_MODW_INPUT) && \
        (ReBuf[NowPosi+1] != INNO_MODW_COIL))
    {
        return 1;
    }

    //CRC校验
    CrcLen = ReLen-NowPosi-InnoEndLen;
    if (usMBCRC16(ReBuf+NowPosi, CrcLen) != 0)
    {
        return 1;
    }

    return 0;
}

//*************************************************************/
//函数名称: Inno_TeHead
//功    能: 整理要发送的数据头
//输    入: TeBuf：要发送的数组
//输    出: 无
//备    注: 无
//*************************************************************/
u8 Inno_TeHead(u8 * TeBuf)
{
    memcpy(TeBuf, InnoHeadInfo, InnoHeadLen);
    memcpy(TeBuf+InnoHeadLen, InnoSignInfo, 2);

    return InnoHeadLen+2;
}

//*************************************************************/
//函数名称: Inno_TeCRCEnd
//功    能: 整理要发送的CRC校验和数据尾
//输    入: TeBuf：CRC校验的起始位置开始
//          CRCLen：CRC校验的数据长度
//输    出: 无
//备    注: 无
//*************************************************************/
void Inno_TeCRCEnd(u8 * TeBuf, u16 CRCLen)
{
    u16 CalcCRC;

    CalcCRC = usMBCRC16(TeBuf, CRCLen);

    TeBuf[CRCLen] = CalcCRC & 0x00FF;
    TeBuf[CRCLen+1] = CalcCRC >> 8;

    memcpy(TeBuf+CRCLen+2, InnoEndInfo, InnoEndLen);
}

//*************************************************************/
//函数名称: Inno_ReCoilDataDeal
//功    能: 根据写线圈命令的地址和数据，设置对应的状态位
//输    入: GetAddr：地址
//          GetData：数值
//输    出: 无
//备    注: 无
//*************************************************************/
void Inno_ReCoilDataDeal(u16 GetAddr, u16 GetData)
{
    switch (GetAddr)
    {
        case INNO_CO_OVERVOL:
            if (GetData == INNO_CO_START)
            {
                InnoUpTest.InnoOverVolFlag = 1;
            }
            else if (GetData == INNO_CO_END)
            {
                InnoUpTest.InnoOverVolFlag = 0;
            }
            else
            {
                ;
            }
            break;
        case INNO_CO_CUTDOWN:
            InnoUpTest.InnoCutDownFlag = 1;
            break;
        case INNO_CO_DOWNVOL:
            if (GetData == INNO_CO_START)
            {
                InnoUpTest.InnoDownVolFlag = 1;
            }
            else if (GetData == INNO_CO_END)
            {
                InnoUpTest.InnoDownVolFlag = 0;
            }
            else
            {
                ;
            }
            break;
        case INNO_CO_OVERTMP:
            if (GetData == INNO_CO_START)
            {
                InnoUpTest.InnoOverTmpFlag = 1;
            }
            else if (GetData == INNO_CO_END)
            {
                InnoUpTest.InnoOverTmpFlag = 0;
            }
            else
            {
                ;
            }
            break;
        case INNO_CO_DOWNTMP:
            if (GetData == INNO_CO_START)
            {
                InnoUpTest.InnoDownTmpFlag = 1;
            }
            else if (GetData == INNO_CO_END)
            {
                InnoUpTest.InnoDownTmpFlag = 0;
            }
            else
            {
                ;
            }
            break;
        case INNO_CO_BLANCE:
            InnoCount = 0;
            if (GetData == 0xFFFF)
            {
                InnoUpTest.InnoBlanFlag = 0xFF;
                InnoUpTest.InnoBlanNum = 1;
            }
            else if (GetData == 0x0000)
            {
                InnoUpTest.InnoBlanFlag = 0;
                InnoUpTest.InnoBlanNum = 0;
                //InBus_Send_Dealwith(SEND_SLAVE_BLANCE, 0);      //需要关闭从机均衡
            }
            else
            {
                InnoUpTest.InnoBlanFlag = GetData & 0x00FF;
                InnoUpTest.InnoBlanNum = GetData & 0x00FF;
            }
            break;
        default:
            break;
    }
}

//*************************************************************/
//函数名称: Inno_ReDataDeal
//功    能: 接收到数据后的处理
//输    入: ReBuf：接收到的数据数组
//          ReLen：接收到的数据长度
//          TeBuf：要发送的数组数据
//          TeLen：要发送的数据长度
//输    出: 0-数据解析成功，可以发送数据
//          1-数据解析失败，不可以发送数据
//备    注: 该函数的作用是，按照协议格式解析数据，如解析成功，则
//          将发送的数据复制到对应发送数组中
//*************************************************************/
INNO_TYPE Inno_ReDataDeal(u8 * ReBuf,u16 ReLen, u8 *TeBuf, u16 *TeLen)
{
    u16 NowPosi;     //去掉包头包尾等信息后的数据位置
//    u16 NowLen;      //去掉包头包尾等信息后的数据长度
    u16 RegAddr;     //寄存器地址
    u16 RegLen;      //寄存器数量

    if (Inno_ReDataJudge(ReBuf, ReLen) != 0)        //判断数据是否符合内部通信协议结构
    {
        return INNO_NONE;
    }

    if (Inno_ReModBusJudge(ReBuf, ReLen) != 0)      //判断数据是否符合modbus通信协议的结构
    {
        return INNO_NONE;
    }

    NowPosi = InnoHeadLen+2;
//    NowLen = ReLen-InnoHeadLen-2-InnoEndLen;
    switch (ReBuf[NowPosi+1])
    {
        case INNO_MODR_HOLD:            //读保持寄存器
            RegAddr = *((u16 *)(ReBuf+NowPosi+2));
            RegLen = *((u16 *)(ReBuf+NowPosi+4));
            if (((RegAddr+RegLen)*2) > InnoDeRunLen)            //数据超限，不返回命令
            {
                return INNO_NONE;
            }
            NowPosi = Inno_TeHead(TeBuf);                                       //数据头
            TeBuf[NowPosi] = INNO_MODMADDR;                                     //主机地址
            TeBuf[NowPosi+1] = INNO_MODR_HOLD;                                  //功能码
            TeBuf[NowPosi+2] = RegLen*2;                                        //字节数
            memcpy(TeBuf+NowPosi+3, InnoDeRunDef+(RegAddr*2), RegLen*2);        //寄存器值
            Inno_TeCRCEnd(TeBuf+NowPosi, (RegLen*2)+3);                         //CRC校验和数据尾
            *TeLen = NowPosi+(RegLen*2)+3+2+InnoEndLen;
            return INNO_READ;
            //break;
        case INNO_MODR_INPUT:           //读输入寄存器
            RegAddr = *((u16 *)(ReBuf+NowPosi+2));
            RegLen = *((u16 *)(ReBuf+NowPosi+4));
            if (((RegAddr+RegLen)*2) > InnoDeParaLen)           //数据超限，不返回命令
            {
                return INNO_NONE;
            }
            NowPosi = Inno_TeHead(TeBuf);                                       //数据头
            TeBuf[NowPosi] = INNO_MODMADDR;                                     //主机地址
            TeBuf[NowPosi+1] = INNO_MODR_INPUT;                                 //功能码
            TeBuf[NowPosi+2] = RegLen*2;                                        //字节数
            memcpy(TeBuf+NowPosi+3, InnoDeParaDef+(RegAddr*2), RegLen*2);       //寄存器值
            Inno_TeCRCEnd(TeBuf+NowPosi, (RegLen*2)+3);                         //CRC校验和数据尾
            *TeLen = NowPosi+(RegLen*2)+3+2+InnoEndLen;
            return INNO_READ;
            //break;
        case INNO_MODW_INPUT:           //写多个寄存器
            RegAddr = *((u16 *)(ReBuf+NowPosi+2));
            RegLen = *((u16 *)(ReBuf+NowPosi+4));
            if (((RegAddr+RegLen)*2) > InnoDeParaLen)           //数据超限，不返回命令
            {
                return INNO_NONE;
            }
			if (*((u16 *)(ReBuf+NowPosi+7+6)) == 0)
			{
				return INNO_NONE;
			}
            //数据保存
            memcpy(InnoDeParaDef+(RegAddr*2), ReBuf+NowPosi+7, (RegLen*2));

            //返回数据准备
            NowPosi = Inno_TeHead(TeBuf);                                       //数据头
            TeBuf[NowPosi] = INNO_MODMADDR;                                     //主机地址
            TeBuf[NowPosi+1] = INNO_MODW_INPUT;                                 //功能码
            *((u16 *)(TeBuf+NowPosi+2)) = RegAddr;                   //起始地址
            *((u16 *)(TeBuf+NowPosi+4)) = RegLen;                    //寄存器数量
            Inno_TeCRCEnd(TeBuf+NowPosi, 6);                                    //CRC校验和数据尾
            *TeLen = NowPosi+6+2+InnoEndLen;
            if (RegAddr == 0)       //写SOC
            {
                return INNO_WRITE_SOC;
            }
            else if (RegAddr == 1)  //写SOH
            {
                return INNO_WRITE_SOH;
            }
            else                    //写参数
            {
                return INNO_WRITE_PARA;
            }
            //break;
        case INNO_MODW_COIL:            //写单个线圈
            RegAddr = *((u16 *)(ReBuf+NowPosi+2));   //地址
            RegLen = *((u16 *)(ReBuf+NowPosi+4));    //数值

            Inno_ReCoilDataDeal(RegAddr, RegLen);               //根据地址和数值，设置对应的状态位

            //返回数据准备
            NowPosi = Inno_TeHead(TeBuf);                                       //数据头
            TeBuf[NowPosi] = INNO_MODMADDR;                                     //主机地址
            TeBuf[NowPosi+1] = INNO_MODW_COIL;                                  //功能码
            *((u16 *)(TeBuf+NowPosi+2)) = RegAddr;                   //起始地址
            *((u16 *)(TeBuf+NowPosi+4)) = RegLen;                    //寄存器数量
            Inno_TeCRCEnd(TeBuf+NowPosi, 6);                                    //CRC校验和数据尾
            *TeLen = NowPosi+6+2+InnoEndLen;
            return INNO_WRITE_COIL;
            //break;
        default:
            return INNO_NONE;
            //break;
    }

}













