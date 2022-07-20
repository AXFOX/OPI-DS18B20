#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

lite DS18B20 driver for OrangePi which supported wiringOP.
author:xfox
contact:https://xfox.fun 
*/

int PIN = 16; //OrangePi Zero2 PC10

#define SKIP_ROM 0xCC
#define SEARCH_ROM 0xF0
#define READ_ROM 0x33
#define CONVERT_T 0x44
#define WRITE_SCRATCHPAD 0x4E
#define READ_SCRTCHPAD 0xBE


//封装digitalRead() LOW --> 0
char OneWire_digitalRead(int pin)
{
    unsigned char low, high;
    low = 0;
    high = 1;
    if (digitalRead(pin) == LOW)
    {
        return low;
    }
    else
    {
        return high;
    }
}

    /*初始化时序 */
int Init_OneWire(void){
    /* 是初始化时序，初始化时序包括主机发出的复位脉冲和从机发出的应答脉冲。主机通过拉低单总线至少480μs产生Tx复位脉冲；
    然后由主机释放总线，并进入Rx接收模式。主机释放总线时，会产生一由低电平跳变为高电平的上升沿，
    单总线器件检测到该上升沿后，延时15～60μs，接着单总线器件通过拉低总线60～240μsμ来产生应答脉冲。
    主机接收到从机的以应答脉冲后，说明有单总线器件在线，然后主机就可以开始对从机进行ROM命令和功能命令操作。 */
    int reponse;
    //wiringPiSetup(void);
    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, LOW);
    usleep(480);
    pinMode(PIN, INPUT);
    usleep(60);
    reponse=OneWire_digitalRead(PIN);
    usleep(410);
    return reponse;
}

/*写流程时序*/
void OneWire_writeBit(int pin, unsigned char bit){
    if (bit & 1){
        //写1时序
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        usleep(10);
        pinMode(pin, INPUT);
        usleep(55);
    }else{
        //写0时序
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        usleep(65);
        pinMode(pin, INPUT);
        usleep(5);
    }

}

/*写一字节数据*/
void OneWire_writeByte(int pin, unsigned char byte)
{
    unsigned char i = 8;
    while (i--)
    {
        OneWire_writeBit(pin, byte & 1);
        byte = byte >> 1;
    }
}

/*读时序*/
unsigned char OneWire_readBit(int pin){
    unsigned char bit;
    bit = 0;
    unsigned long flags;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    usleep(3);
    pinMode(pin, INPUT);
    usleep(10);
    bit = OneWire_digitalRead(pin);
    usleep(45);
    return bit;
}
/*读一字节数据*/
unsigned char OneWire_readByte(int pin)
{
    unsigned char i = 8, byte = 0;
    while (i--)
    {
        byte = byte>> 1;
        byte |= (OneWire_readBit(pin) << 7);
    }
    return byte;
}

int main(void){
    wiringPiSetup();
    unsigned char temp;
    if (Init_OneWire()==0){
        printf("error,DS18B20 not found.\n");
    }else{
        usleep(1);
        OneWire_writeByte(PIN, SKIP_ROM);
        OneWire_writeByte(PIN, CONVERT_T);
        Init_OneWire();
        usleep(1);
        OneWire_writeByte(PIN, READ_SCRTCHPAD);
        unsigned char firstByte;           //低八位
        unsigned char lastByte;            //高八位
        firstByte = OneWire_readByte(PIN); //从最低位开始读
        lastByte = OneWire_readByte(PIN);
        temp = lastByte;
        temp = temp << 8;        //转为十进制
        temp = temp | firstByte; //原始的12位温度值
        temp = temp * 0.0625;
    }
    return temp;
}
