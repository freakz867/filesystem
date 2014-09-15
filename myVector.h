#ifndef MYVECTOR_H
#define	MYVECTOR_H
template <class T>
class Vector
{
public:
    typedef T *iterator;
    Vector();
    Vector(unsigned int size);
    Vector(unsigned int size, const T &initial);
    Vector(const Vector<T> &orig);      
    ~Vector();

    unsigned int capacity()const;
    unsigned int size()const;
    bool empty()const;
    iterator begin();
    iterator end();
    T &front();
    T &back();
    void push_back(const T &value); 
    void pop_back();  
    iterator erase(iterator);
    void reserve(unsigned int capacity);   
    void resize(unsigned int size);   

    T &operator[](unsigned int index);
    const T &operator[](unsigned int index)const; 
    Vector<T> &operator=(const Vector<T> &);
    void clear();
private:
    unsigned int currSize;
    unsigned int maxSize;
    T *arr;
};


template<class T>
Vector<T>::Vector(){
    maxSize=0;
    currSize=0;
    arr=0;
}

template<class T>
Vector<T>::Vector(const Vector<T> &orig){
    currSize=orig.currSize;
    maxSize=orig.maxSize;
    arr=new T [currSize];  
    for(unsigned int i=0;i<currSize;i++)
        arr[i]=orig.arr[i];  
}

template<class T>
Vector<T>::Vector(unsigned int size){
    maxSize=size;
    currSize=size;
    arr=new T [size];
}

template<class T>
Vector<T>::Vector(unsigned int size, const T &initial){
    currSize=size;
    maxSize=size;
    arr=new T [size];
    for(unsigned int i=0;i<size;i++)
        arr[i]=initial;
}

template<class T>
Vector<T> & Vector<T>::operator = (const Vector<T> & v){
    delete [] arr;
    currSize=v.currSize;
    maxSize=v.maxSize;
    arr=new T [currSize];
    for(unsigned int i=0;i<currSize;i++)
        arr[i]=v.arr[i];
    return *this;
}

template<class T>
typename Vector<T>::iterator Vector<T>::begin(){
    return arr;
}

template<class T>
typename Vector<T>::iterator Vector<T>::end(){
    return arr+size();
}

template<class T>
T& Vector<T>::front(){
    return arr[0];
}

template<class T>
T& Vector<T>::back()
{
    return arr[size-1];
}

template<class T>
void Vector<T>::push_back(const T &v){
    if(currSize>=maxSize)
        reserve(maxSize+5);
    arr[currSize++]=v;
}

template<class T>
void Vector<T>::pop_back(){
    --currSize;
}

template<class T>
void Vector<T>::reserve(unsigned int capacity){
    if(arr==0){
        currSize=0;
        maxSize=0;
    }    
    T *Newarr=new T [capacity];
    unsigned int l_Size;
    if(capacity<currSize)
        l_Size=capacity;
    else
        l_Size=currSize;
    for (unsigned int i = 0; i < l_Size; i++)
        Newarr[i]=arr[i];
    maxSize=capacity;
    delete [] arr;
    arr=Newarr;
}

template<class T>
typename Vector<T>::iterator Vector<T>::erase(typename Vector<T>::iterator pos){
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
      arr[i] = arr[i + 1];
    }
   --currSize;
   return &(arr[index+1]);
}


template<class T>
unsigned int Vector<T>::size()const//
{
    return currSize;
}

template<class T>
void Vector<T>::resize(unsigned int size)
{
    reserve(size);
    currSize = size;
}

template<class T>
T& Vector<T>::operator[](unsigned int index)
{
    return arr[index];
}  

template<class T>
const T& Vector<T>::operator[](unsigned int index)const
{
    return arr[index];
}  

template<class T>
unsigned int Vector<T>::capacity()const
{
    return maxSize;
}

template<class T>
Vector<T>::~Vector()
{
    delete [] arr;
}
template <class T>
void Vector<T>::clear()
{
    maxSize=0;
    currSize=0;
    arr=0;
}

#endif	/* MYVECTOR_H */

