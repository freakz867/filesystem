#ifndef MYMAP_H
#define	MYMAP_H
#include <stdlib.h>
#include <utility>
using namespace std;

template <class key,class T>
class Map
{ 
    public:
    typedef key key_Type;
    typedef T data_Type;
    typedef pair<key,T> value_Type;
    typedef pair<key,T> *iterator;
    Map();
    Map(unsigned int size);
    Map(unsigned int size, const value_Type &initial);
    Map(const Map<key,T> &orig);      
    ~Map();

    unsigned int capacity()const;
    unsigned int size()const;
    bool empty()const;
    iterator begin();
    iterator end();

       
    bool insert(const value_Type &value);
    iterator find(const key_Type &findkey);
    iterator erase(iterator);
    void reserve(unsigned int capacity);   
    void resize(unsigned int size);
    void clear();

    T &operator[](unsigned int index);
    const T &operator[](unsigned int index)const; 
    Map<key,T> &operator=(const Map<key,T> &);
    
    void insertionSort(value_Type *arrayPtr, int length);
private:
    unsigned int currSize;
    unsigned int maxSize;
    value_Type *arr;
};

template <class key,class T>
Map<key,T>::Map(){
    maxSize=0;
    currSize=0;
    arr=0;
}

template <class key,class T>
Map<key,T>::Map(const Map<key,T> &orig){
    currSize=orig.currSize;
    maxSize=orig.maxSize;
    arr=new value_Type [currSize];  
    for(unsigned int i=0;i<currSize;i++)
        arr[i]=orig.arr[i];  
}

template <class key,class T>
Map<key,T>::Map(unsigned int size){
    maxSize=size;
    currSize=size;
    arr=new value_Type [size];
}

template <class key,class T>
Map<key,T>::Map(unsigned int size, const value_Type &initial){
    currSize=size;
    maxSize=size;
    arr=new value_Type [size];
    for(unsigned int i=0;i<size;i++)
        arr[i]=initial;
}

template <class key,class T>
Map<key,T> & Map<key,T>::operator = (const Map<key,T> &v){
    delete [] arr;
    currSize=v.currSize;
    maxSize=v.maxSize;
    arr=new value_Type [currSize];
    for(unsigned int i=0;i<currSize;i++)
        arr[i]=v.arr[i];
    return *this;
}

template <class key,class T>
typename Map<key,T>::iterator Map<key,T>::begin(){
    return arr;
}

template <class key,class T>
typename Map<key,T>::iterator Map<key,T>::end(){
    return arr+size();
}

template <class key,class T>
bool Map<key,T>::empty() const{
    if(currSize==0)
        return true;
    if(currSize!=0)
        return false;
}

template <class key,class T>
unsigned int Map<key,T>::capacity()const
{
    return maxSize;
}

template <class key,class T>
unsigned int Map<key,T>::size()const//
{
    return currSize;
}

template <class key,class T>
bool  Map<key,T>::insert(const value_Type& value){
    if(currSize>=maxSize)
        reserve(maxSize+5);
    arr[currSize]=value;
    ++currSize;
    insertionSort(arr,currSize);
   return true;
}

template <class key,class T>
typename Map<key,T>::iterator Map<key,T>::find(const key_Type& findkey){
    for(int i=0;i<currSize;++i)
        if(arr[i].first==findkey)
            return &(arr[i]);
    return (*this).end();
}

template <class key,class T>
void Map<key,T>::reserve(unsigned int capacity){
    if(arr==0){
        currSize=0;
        maxSize=0;
    }    
    value_Type *Newarr=new value_Type [capacity];
    unsigned int l_Size;
    if(capacity<currSize)
        l_Size=capacity;
    else
        l_Size=currSize;
    for (unsigned int i = 0;i<l_Size;i++)
        Newarr[i]=arr[i];
    maxSize=capacity;
    delete [] arr;
    arr=Newarr;
}

template <class key,class T>
typename Map<key,T>::iterator Map<key,T>::erase(typename Map<key,T>::iterator pos){
    int index=0;
    while(index<currSize)
    {
        
        if(pos==&(arr[index]))
        {
            break;
        }
        ++index;
    }
    for(int i=index; i<currSize-1; i++) {
      arr[i]=arr[i+1];
    }
   --currSize;
   return &(arr[index+1]);
}

template <class key,class T>
Map<key,T>::~Map()
{
    delete[ ] arr;
}

template <class key,class T>
void Map<key,T>::clear()
{
    maxSize=0;
    currSize=0;
    arr=0;
}

template <class key,class T>
void Map<key,T>::insertionSort(value_Type *arrayPtr, int length) // сортировка вставками
{
     value_Type temp;// временная переменная для хранения значения элемента сортируемого массива
       int item;  // индекс предыдущего элемента
    for (int counter = 1; counter < length; counter++)
    {
        temp = arrayPtr[counter]; // инициализируем временную переменную текущим значением элемента массива
        item = counter-1; // запоминаем индекс предыдущего элемента массива
        while(item >= 0 && arrayPtr[item].first > temp.first) // пока индекс не равен 0 и предыдущий элемент массива больше текущего
        {
            arrayPtr[item + 1] = arrayPtr[item]; // перестановка элементов массива
            arrayPtr[item] = temp;
            item--;
        }
    }
}


#endif	/* MYMAP_H */

