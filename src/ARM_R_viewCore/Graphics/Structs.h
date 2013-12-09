#ifndef STRUCTS_H
#define STRUCTS_H

class NovomarMsg
{
public:
    unsigned counter;
    unsigned char body[256];
public:
    unsigned char GetCRC();
    unsigned char Addr()	{ return body[0]; }		// 1
    unsigned Size() {	return body[1];	 }			// 2
    unsigned FullSize() {	return body[1]+3;	 }
    unsigned Type() {	return body[2];	 }			// 2
    unsigned char Body(int i)  { return body[i+3]; }
    void SetAddr(unsigned char byte) { body[0]=byte; }
    void SetType(unsigned char byte) { body[2]=byte; }
    void SetSize(unsigned char byte) { body[1]=byte;	}
    void SetBody(int i, unsigned char byte) { body[i+3]=byte; }

    void Init();
    unsigned Add(unsigned char byte);
    unsigned Used() { return counter; }
    unsigned IsFree() { return 12-counter; }
    int IsEnd();
    int ChkCRC();
    unsigned char TstCRC();
    unsigned char CalcCRC();
    NovomarMsg() { Init(); }
};

struct NvDataInit
{
    unsigned m_frequ;
    unsigned char m_att;
    unsigned char m_att2;
    unsigned char m_att3;
    unsigned char filter;
};

#endif // STRUCTS_H
