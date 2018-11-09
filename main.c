// kuznechik.c
// 04-Jan-15  Markku-Juhani O. Saarinen <mjos@iki.fi>

// main() for testing

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <iostream>
#include "mycrypt.h"
using namespace std;

int main(int argc, char **argv)
{	
	int i;

    uint8_t testvec_key[32] = {
		0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
		0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10, 
		0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF	
	};

/*	vector<bool> inputvecb={
		0,0,0,1,0,0,0,1,
		0,0,1,0,0,0,1,0, 
		0,0,1,1,0,0,1,1,
		0,1,0,0,0,1,0,0,
		0,1,0,1,0,1,0,1,
		0,1,1,0,0,1,1,0,
		0,1,1,1,0,1,1,1,
		0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,
		1,1,1,0,1,1,1,0,
		1,1,0,1,1,1,0,1,
		1,1,0,0,1,1,0,0,
		1,0,1,1,1,0,1,1,
		1,0,1,0,1,0,1,0,
		1,0,0,1,1,0,0,1,
		1,0,0,0,1,0
		};
*/
	vector<uint8_t> inputvec={
		0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00,
		0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xEE, 0xFF, 0x0A,
		0x11, 0X22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
		0x99, 0xAA, 0xBB, 0xCC, 0xEE, 0xFF, 0x0A, 0x00,
		0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
		0xAA, 0xBB, 0xCC, 0xEE, 0xFF, 0x0A, 0x00
	};
	vector<uint8_t> cryptvec;
	vector<uint8_t> decryptvec;
	cout<<"\nИсходное сообщение:\n";
	for(i=0;i<inputvec.size();i++)
		{
			cout<<hex<<int(inputvec[i])<<' ';
			if(i%16==15)cout<<endl;
		}
 	
    uint8_t cmac[8],decryptcmac[8],decryptmesmac[8];

	MAC(testvec_key, inputvec, cmac);
	cout<<"\nИмитовставка\n";
	for(i=0; i<8; i++)
		cout<<hex<<int(cmac[i])<<' ';
	cout<<endl;

	crypt(testvec_key,inputvec,cryptvec,cmac);

	cout<<"\nЗашифрованное сообщение:\n";
	for(i=0;i<cryptvec.size();i++)
		{
			cout<<hex<<int(cryptvec[i])<<' ';
			if(i%16==15)cout<<endl;
		}
	cout<<endl;

	
	decrypt(testvec_key, cryptvec, decryptvec);

	cout<<"\nСырое расшифрованное сообщение:\n";
	
	for(i=0;i<decryptvec.size();i++)
		{
			cout<<hex<<int(decryptvec[i])<<' ';
			if(i%16==15)cout<<endl;
		}
	cout<<endl;
	
	//decryptvec[10]=0;

	int startbitmac=inputvec.size();
	cout<<"\nНачало имитовставки : "<<dec<<startbitmac<<endl;

	for(i=0;i<8;i++)
	{
		decryptcmac[i]=int(decryptvec[i+startbitmac]);
	}

	cout<<"\nРасшифрованная имитовставка\n";
	for(i=0;i<8;i++)
		cout<<hex<<int(decryptcmac[i])<<' ';
	cout<<endl;
	
	decryptvec.erase(decryptvec.begin()+startbitmac,decryptvec.end());

	MAC(testvec_key,decryptvec,decryptmesmac);
	cout<<"\nИмитовставка расшифрованного сообщения\n";
	for(i=0;i<8;i++)
		cout<<hex<<int(decryptmesmac[i])<<' ';
	cout<<endl;

	
return 0;

}


