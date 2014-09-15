#ifndef ALLOCATOR_H
#define	ALLOCATOR_H

#include "stream.h"


class Allocator {
public:
    Allocator();
    Allocator(const Allocator& orig);
    virtual ~Allocator();
    int addFreeBlock(int,int);
    void viewAll()const;
    int findByAddress(int)const;
    int deleteFreeBLock(int);
private:
    map<int,int> freemap;
};

#endif	/* ALLOCATOR_H */

