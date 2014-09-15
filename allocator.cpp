#include "allocator.h"

Allocator::Allocator() {
}

Allocator::Allocator(const Allocator& orig) {
}

Allocator::~Allocator() {
    freemap.clear();
}

int Allocator::addFreeBlock(int address, int size){
    freemap.insert(pair<int,int>(address,size));
    return 1;
}

void Allocator::viewAll()const{
    for(map<int,int>::const_iterator itr=freemap.begin(); itr!=freemap.end(); ++itr)
    {
	cout<<(*itr).first<<" | size:"<<(*itr).second<<endl;
    }
}

int Allocator::findByAddress(int address) const{
    for( map<int,int>::const_iterator itr=freemap.begin(); itr!=freemap.end(); ++itr)
    {
        if((*itr).first==address)
            return (*itr).second;
    }
    return -1;
}

int Allocator::deleteFreeBLock(int address){
    for( map<int,int>::iterator itr=freemap.begin(); itr!=freemap.end(); ++itr)
    {
        if((*itr).first==address)
        {
            freemap.erase(itr);
        }
    }
}
