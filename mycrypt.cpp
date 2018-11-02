#include "mycrypt.h"
//#define DEBUG

#ifdef DEBUG
    #include <iostream>
    #include <stdio.h>
#endif
#include <string.h>

using namespace std;

void leftShift(uint8_t *array , int ARRAY_LENGTH){
   uint8_t preBuffer = 0;
   uint8_t postBuffer = 0;
   for(uint8_t i = ARRAY_LENGTH - 1; i < ARRAY_LENGTH; --i){
      postBuffer = (array[i] & 0b10000000) >> 7;
      array[i] = array[i]<< 1;
      array[i] |= preBuffer;
      preBuffer = postBuffer;
   }
}

int MSB(uint8_t cc)
{
    return cc>>7;
}

void MAC(uint8_t key_t[32], vector<uint8_t> input, uint8_t output[8], bool full)
{
    int i;
#ifdef DEBUG
    cout<<"\nНачало вычисления имитовставки с байтовым вектором\nВходные данные\n";
    for(i=0;i<input.size();i++)
        cout<<hex<<int(input[i])<<' ';
    cout<<"\nКлюч:\n";
    for(i=0;i<32;i++)
        cout<<hex<<int(key_t[i])<<' ';
#endif    
    uint8_t tmp0[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};    
    
  
	uint8_t R[16], K1[16], K2[16];


    //if(input.size()%16==0)full=true;
    //else full=false;
    int blocks=input.size()/16;
/*    if(!full)blocks++;*/

  /*  if(!full)
        {
        input.push_back(0x80);
        for(i=input.size();i<blocks*16;i++)
            input.push_back(0x00);
        }

*/
    kuz_key_t key;
	w128_t x;   
       
    kuz_set_encrypt_key(&key, key_t);
    
    for (i = 0; i < 16; i++)
		x.b[i] = tmp0[i];
    
 	kuz_encrypt_block(&key, &x);   
 
    memcpy(R,x.b, 16);
    
#ifdef DEBUG
    printf("\nR\t=");
	for(i=0; i<16; i++)
        printf(" %02X", R[i]);
    printf("\n");
#endif

    memcpy(K1,R,16);  
    leftShift(K1,16);
    
    if(MSB(R[0])==1)
    {
        K1[15]=K1[15]^0x87;
    }

#ifdef DEBUG
    printf("K1\t=");
	for(i=0; i<16; i++)
        printf(" %02X", K1[i]);
    printf("\n");
#endif
    
    memcpy(K2,K1,16);
    leftShift(K2,16);
    
    if(MSB(K1[0])==1)
        K2[15]=K2[15]^0x87;
    
#ifdef DEBUG
    printf("K2\t=");
	for(i=0; i<16; i++)
        printf(" %02X", K2[i]);
    printf("\n");
#endif
      
    int j;
    for(i=0;i<16;i++)x.b[i]=0x00; 

    for(j=0;j<blocks-1;j++)
    {
        for(i=0;i<16;i++)
            x.b[i]=input.at(i+j*16)^x.b[i];

#ifdef DEBUG    
        printf("Входные данные на %i итерации\n",j+1);
        for(i=0;i<16;i++)
            printf(" %02X", x.b[i]);
        cout<<endl;
 #endif

        kuz_encrypt_block(&key, &x);  
    
#ifdef DEBUG    
        printf("Выходные данные на %i итерации\n",j+1);
        for(i=0;i<16;i++)
            printf(" %02X", x.b[i]);
        cout<<endl;
 #endif    
    }
    
    for(i=0;i<16;i++)
        {
        x.b[i]=input.at(i+j*16)^x.b[i];
        if(full)x.b[i]=x.b[i]^K1[i];
        else x.b[i]=x.b[i]^K2[i];
        }

#ifdef DEBUG    
    printf("Входные данные на последней %i итерации\n",j+1);
    for(i=0;i<16;i++)
        printf(" %02X", x.b[i]);
    cout<<endl;
 #endif 

    kuz_encrypt_block(&key, &x); 

#ifdef DEBUG    
        printf("Выходные данные на последней %i итерации\n",j+1);
        for(i=0;i<16;i++)
            printf(" %02X", x.b[i]);
        cout<<endl;
 #endif    
    for(i=0;i<8;i++)
        output[i]=x.b[i];
}

void MAC(uint8_t key_t[32], vector<bool> input, uint8_t output[8])
{
#ifdef DEBUG 
    cout<<"\nНачало вычисления имитовставки с булевским вектором\n";
#endif
    vector<uint8_t> inputi;
    int i,j,k;
    k=0;
    bool full=true;
#ifdef DEBUG
    cout<<"Входные булевские данные\n";
    for(i=0;i<input.size();i++)
    {
        cout<<int(input.at(i))<<' ';
        if(i%8==7)cout<<endl;
    }
#endif

    i=0;
    while(i<input.size())
    {
        inputi.push_back(int(input.at(i)));
        i++;
        for(j=1;j<8;j++)
        {
            inputi[k]=inputi[k]<<1 | input[i];
            i++;
            if(i==input.size() && j!=7)
                {
                    inputi[k]=inputi[k]<<1 | 0x01;
                    inputi[k]=inputi[k]<<(8-j-2);
                    full=false;
                    break;
                }
        }
        k++;
    }

    if(inputi.size()%16!=0)
    {
        int bk=inputi.size()/16+1;
         if(full)
            {
                full=false;
                inputi.push_back(0x80);
            }
        for(i=inputi.size();i<bk*16;i++)
            inputi.push_back(0x00);
    }
#ifdef DEBUG 
    cout<<"\nПолученный (дополненный) вектор\n";
    for(i=0;i<inputi.size();i++)
        cout<<hex<<int(inputi[i])<<' ';
    cout<<"\nПередача данных в следующую функцию\n";
#endif
    //uint8_t key_t[32];
    //for(i=0;i<32;i++)key_t[32]=key[32];
    MAC(key_t, inputi , output, full);

#ifdef DEBUG
    cout<<"Данные на выходе\n";
    for(i=0;i<8;i++)cout<<hex<<int(output[i])<<' ';
#endif

}

int crypt(uint8_t key_t[32], vector<uint8_t> input, vector<uint8_t> &output)
{
    
    uint8_t tmpblock[16];
    kuz_key_t key;
     
    kuz_set_encrypt_key(&key, key_t);
    bool full;
    int i,j;

    if(input.size()%16==0)full=true;
    else full=false;

    int blocks=input.size()/16;
    if(!full)blocks++;

    output.reserve(input.size());

    if(!full)
        for(i=input.size();i<blocks*16;i++)
            input.push_back(0x00);

 #ifdef DEBUG       
    cout<<"Input: ";
    for(i=0;i<input.size();i++)
        cout<<hex<<int(input.at(i))<<' ';
    cout<<endl;
#endif

    for(i=0;i<blocks;i++)
    {
    copy(input.begin()+i*16,input.begin()+(i+1)*16,tmpblock);
    kuz_encrypt_block(&key, tmpblock); 

    for(j=0;j<16;j++)
        output.push_back(tmpblock[j]);
    }

#ifdef DEBUG
    cout<<"Output:";
    for(i=0;i<output.size();i++)
        cout<<hex<<int(output.at(i))<<' ';
    cout<<endl;
#endif
    return 1;
}

int crypt(uint8_t key_t[32], vector<bool> input, vector<bool> &output, uint8_t mac[8])
{
#ifdef DEBUG
    cout<<"\nНачало шифрования\n";
#endif
    int i,j;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            input.push_back(mac[i]>>7);
            mac[i]=mac[i]<<1;
        }
    }

    int kk=input.size()%128;
    if(kk!=0)
        for(i=0;i<128-kk;i++)
            input.push_back(0);

#ifdef DEBUG
    cout<<"Входное сообщение с имитовставкой и дополненное нулями\n";
    for(i=0;i<input.size();i++)
    {
        cout<<input[i]<<' ';
        if(i%8==7)cout<<' ';
    }
    cout<<'\n';
#endif
    vector<uint8_t> inputi,outputi;
    i=0;
    int k;
    while(i<input.size())
    {
        inputi.push_back(int(input.at(i)));
        i++;
        for(j=1;j<8;j++)
        {
            inputi[k]=inputi[k]<<1 | input[i];
            i++;
        }
        k++;
    }   
#ifdef DEBUG
    cout<<"Входное байтовое сообщение\n";
    for(i=0;i<inputi.size();i++)
        cout<<hex<<int(inputi[i])<<' ';
    cout<<'\n';
#endif
    crypt(key_t,inputi,outputi);
    for(i=0;i<outputi.size();i++)
        for(j=0;j<8;j++)
            {
                output.push_back(outputi[i]>>7);
                outputi[i]=outputi[i]<<1;
            }
    return 1;
}

int decrypt(uint8_t key_t[32], std::vector<bool> input, std::vector<bool> &output)
{
    vector<uint8_t> inputi;
    vector<uint8_t> outputi;
    int i,j;
    i=0;
    int k;
    uint8_t tmpblock[16];
    kuz_key_t key;
    int blocks=input.size()/128;
    while(i<input.size())
    {
        inputi.push_back(int(input.at(i)));
        i++;
        for(j=1;j<8;j++)
        {
            inputi[k]=inputi[k]<<1 | input[i];
            i++;
        }
        k++;
    }  

    kuz_set_decrypt_key(&key, key_t);
    
    for(i=0;i<blocks;i++)
        {
        copy(inputi.begin()+i*16,inputi.begin()+(i+1)*16,tmpblock);
        kuz_decrypt_block(&key, tmpblock); 

        for(j=0;j<16;j++)
            outputi.push_back(tmpblock[j]);
        }
    
    for(i=0;i<outputi.size();i++)
        for(j=0;j<8;j++)
            {
                output.push_back(outputi[i]>>7);
                outputi[i]=outputi[i]<<1;
            }
    return 1;
}