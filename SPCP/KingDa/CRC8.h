#ifndef __CRC8_KINGDA_H
#define __CRC8_KINGDA_H


unsigned char CRC8(unsigned char *p, unsigned short counter);
unsigned char CRC8_Extend(unsigned char *p,char counter,unsigned char Checkdata);
void CRC16_Table(unsigned char *P,unsigned char Num);


#endif