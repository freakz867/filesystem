#include "crypt.h"
#include "string.h"
#include <iostream>
crypto::crypto(){
    for(int i=0;i<256;++i)
    {
        SBox[i]=0;
        k[i]=0;
    }
}

crypto::~crypto(){
    for(int i=0;i<256;++i)
    {
        SBox[i]=0;
        k[i]=0;
    }
}

char *crypto::encrypt(const char *data,char *key){
    char * temp;
	int i,j=0,t,tmp,tmp2;
	for (tmp=0;tmp<256;tmp++){
		SBox[tmp]=tmp;
		k[tmp]=key[(tmp % strlen(key))];
	}
        
        for (i=0;i<256;i++){
		j = (j + SBox[i] + k[i]) % 256;
                //меняем SBox i с j
		tmp=SBox[i];
		SBox[i]=SBox[j];
		SBox[j]=tmp;
	}

        temp = new char [strlen(data)] ;
	i=j=0;
	for (tmp=0;tmp<(int)strlen(data);tmp++){
                i = (i + 1) % 256;
                j = (j + SBox[i]) % 256;
		tmp2=SBox[i];
		SBox[i]=SBox[j];
		SBox[j]=tmp2;
                t = (SBox[i] + SBox[j]) % 256;
                if (SBox[t]==data[tmp])
                        temp[tmp]=data[tmp];
                else
                        temp[tmp]=SBox[t]^data[tmp];
	}
//temp[tmp]='\0';
return temp;
}

char *crypto::decrypt(const char *data, char *key){
    return encrypt(data,key);
}
