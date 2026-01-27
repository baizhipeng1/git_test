

#ifndef __INNO485_H__
#define __INNO485_H__


#include "common.h"



//----modbus通信的相关参数----//
#define INNO_MODMADDR       0x00        //主机地址
#define INNO_MODSADDR       0x01        //从机地址

#define INNO_MODR_HOLD      0x03        //读保持寄存器（不可写）
#define INNO_MODR_INPUT     0x04        //读输入寄存器（可写）
#define INNO_MODW_INPUT     0x10        //写多个寄存器
#define INNO_MODW_COIL      0x05        //写单个线圈


//----遥测命令的相关状态位----//
typedef struct
{
    u8 InnoOverVolFlag;      //单体过压测试
    u8 InnoDownVolFlag;      //单体欠压测试
    u8 InnoOverTmpFlag;      //高温测试
    u8 InnoDownTmpFlag;      //低温测试
    u8 InnoBlanFlag;         //均衡测试      0xFF-全部   其他-对应的通道，1开始
    u8 InnoBlanNum;          //均衡测试当前打开的通道
    u8 InnoCutDownFlag;      //模拟下电测试
}InnoTestType;

//大小端模式--两者只能打开一个
#define INNO_CO_LSB         //小端模式
//#define INNO_CO_MSB         //大端模式

#ifdef INNO_CO_LSB
//----遥测命令（写线圈）对应的地址和动作----//
#define INNO_CO_OVERVOL     0x0000      //模拟过压
#define INNO_CO_CUTDOWN     0x0100      //模拟下电
#define INNO_CO_DOWNVOL     0x0200      //模拟低压
#define INNO_CO_OVERTMP     0x0300      //模拟高温
#define INNO_CO_DOWNTMP     0x0400      //模拟低温
#define INNO_CO_BLANCE      0x0500      //模拟均衡

#define INNO_CO_START       0x00FF      //动作
#define INNO_CO_END         0xFF00      //不动作
#endif

#ifdef INNO_CO_MSB
//----遥测命令（写线圈）对应的地址和动作----//
#define INNO_CO_OVERVOL     0x0000      //模拟过压
#define INNO_CO_CUTDOWN     0x0001      //模拟下电
#define INNO_CO_DOWNVOL     0x0002      //模拟低压
#define INNO_CO_OVERTMP     0x0003      //模拟高温
#define INNO_CO_DOWNTMP     0x0004      //模拟低温
#define INNO_CO_BLANCE      0x0005      //模拟均衡

#define INNO_CO_START       0xFF00      //动作
#define INNO_CO_END         0x00FF      //不动作
#endif

#define INNO_CO_DELAY       5           //遥测命令动作时之前的延时时间，单位为秒（5代表2秒，7代表3秒）



//----数据格式标记----//

typedef enum
{
    INNO_NONE=0,            /*数据格式不对*/
    INNO_READ=1,            /*接收到的数据为读*/
    INNO_WRITE_PARA=2,      /*接收到的数据为写参数*/
    INNO_WRITE_SOC=3,       /*接收到的数据为写SOC*/
    INNO_WRITE_SOH=4,       /*接收到的数据为写SOH*/
    INNO_WRITE_COIL=5       /*接收到的数据为写线圈*/
}INNO_TYPE;









extern InnoTestType InnoUpTest;
extern u8 InnoCount;



extern INNO_TYPE Inno_ReDataDeal(u8 * ReBuf,u16 ReLen, u8 *TeBuf, u16 *TeLen);


#endif
