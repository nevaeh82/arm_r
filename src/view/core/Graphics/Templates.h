#ifndef TEMPLATES_H
#define TEMPLATES_H

template<class T,int Size> class TRBuffer
{
    unsigned BuffSize;
    unsigned Head;
    unsigned Tail;
    T Buff[Size];
  public:
    TRBuffer();
    void Init();
    void Add(T elem);
    T Get();
    unsigned Used();
    unsigned Free();
};

template<class T,int Size> TRBuffer<T,Size>::TRBuffer()
{
    Init();
}

template<class T,int Size> void TRBuffer<T,Size>::Init()
{
  BuffSize = Size;
  Head = 0;
  Tail = 0;
}

template<class T,int Size> void TRBuffer<T,Size>::Add(T elem)
{
  Buff[Head++] = elem;
  if(Head == BuffSize) Head = 0;
}

template<class T,int Size> T TRBuffer<T,Size>::Get()
{
  if(++Tail < BuffSize) return Buff[Tail-1];
  Tail = 0;
  return Buff[BuffSize-1];
}

template<class T,int Size> unsigned TRBuffer<T,Size>::Used()
{
  int n = Head - Tail;
  if( n >= 0 ) return n;
  else return n+BuffSize;
}

template<class T,int Size> unsigned TRBuffer<T,Size>::Free()
{
  int n = Tail - Head;
  if( n > 0 ) return n;
  else return n+BuffSize;
}
#endif // TEMPLATES_H
