
#ifndef CRYPT_H
#define	CRYPT_H

class crypto{
public:
    crypto();            //инициаизируем SBox нулями
    ~crypto();
    int setSBox(char *,int);        //заполняем SBox данными
    char *encrypt(const char *,char *);
    char *decrypt(const char *,char *);
private:
        int SBox[256];
        int k[256];
};

#endif	/* CRYPT_H */

