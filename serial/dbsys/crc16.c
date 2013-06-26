#include "crc16.h"

unsigned short CRC16(unsigned char puchMsg[], unsigned short length) {
	unsigned char uchCRCHi = 0xFF ; /* ��CRC�ֽڳ�ʼ�� */
	unsigned char uchCRCLo = 0xFF ; /* ��CRC �ֽڳ�ʼ�� */
	unsigned uIndex ; 				/* CRCѭ���е����� */

	/* ������Ϣ������ */
	while (length--) {
		uIndex = uchCRCHi ^ *puchMsg++ ; /* ����CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo);
}

/*
int main(int argc, char **argv) {
	unsigned char ppp[13] = {0x12,0x34};
	unsigned short result;   
	// ����CRC
	result = CRC16(ppp, 2);
	printf("%x\n",result);
}
*/
