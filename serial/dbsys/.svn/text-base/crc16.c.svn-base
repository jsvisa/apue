#include "crc16.h"

unsigned short CRC16(unsigned char puchMsg[], unsigned short length) {
	unsigned char uchCRCHi = 0xFF ; /* 高CRC字节初始化 */
	unsigned char uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */
	unsigned uIndex ; 				/* CRC循环中的索引 */

	/* 传输消息缓冲区 */
	while (length--) {
		uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo);
}

/*
int main(int argc, char **argv) {
	unsigned char ppp[13] = {0x12,0x34};
	unsigned short result;   
	// 计算CRC
	result = CRC16(ppp, 2);
	printf("%x\n",result);
}
*/
