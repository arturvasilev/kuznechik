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


void print_w128(w128_t *x)
{
	int i;
	
	for (i = 0; i < 16; i++)
		printf(" %02X", x->b[i]);
	printf("\n");
}


int main(int argc, char **argv)
{	
	int i,j;
	kuz_key_t key;
	w128_t x;

    uint8_t testvec_key[32] = {
		0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
		0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10, 
		0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF	
	};

	vector<bool> inputvecb={
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

	vector<bool> cryptvecb;
	vector<bool> decryptvecb;
	cout<<"\nИсходное сообщение:\n";
	for(i=0;i<inputvecb.size();i++)
		{
			cout<<inputvecb[i]<<' ';
			if(i%8==7)cout<<' ';
		}
    
    uint8_t cmac[8],decryptcmac[8],decryptmesmac[8];

	MAC(testvec_key, inputvecb, cmac);
	cout<<"\nИмитовставка\n";
	for(i=0; i<8; i++)
		cout<<hex<<int(cmac[i])<<' ';
	cout<<endl;
	
	crypt(testvec_key,inputvecb,cryptvecb,cmac);

	cout<<"\nЗашифрованное сообщение:\n";
	for(i=0;i<cryptvecb.size();i++)
		{
			cout<<cryptvecb[i]<<' ';
			if(i%8==7)cout<<' ';
		}
	cout<<endl;

	decrypt(testvec_key, cryptvecb, decryptvecb);

	cout<<"\nСырое расшифрованное сообщение:\n";
	
	for(i=0;i<decryptvecb.size();i++)
		{
			cout<<decryptvecb[i]<<' ';
			if(i%8==7)cout<<' ';
		}
	cout<<endl;

	//decryptvecb[10]=0;


	int startbitmac=inputvecb.size();
	cout<<"\nНачало имитовставки : "<<dec<<startbitmac<<endl;

	for(i=0;i<8;i++)
	{
		decryptcmac[i]=int(decryptvecb[i*8+startbitmac]);
		for(j=1;j<8;j++)
			{
				decryptcmac[i]=decryptcmac[i]<<1;
				decryptcmac[i]=decryptcmac[i] | int(decryptvecb[i*8+j+startbitmac]);
			}
	}

	cout<<"\nРасшифрованная имитовставка\n";
	for(i=0;i<8;i++)
		cout<<hex<<int(decryptcmac[i])<<' ';
	cout<<endl;

	decryptvecb.erase(decryptvecb.begin()+startbitmac,decryptvecb.end());

	MAC(testvec_key,decryptvecb,decryptmesmac);
	cout<<"\nИмитовставка расшифрованного сообщения\n";
	for(i=0;i<8;i++)
		cout<<hex<<int(decryptmesmac[i])<<' ';
	cout<<endl;

	
return 0;

}


