#include "Structs.h"

int NovomarMsg::IsEnd()
{
    if(body[0])
    {
        if(counter > 2)
        {
            if(counter == Size() + 3)
            {
                return 1;
            }
        }
    }
    return 0;
}

unsigned char NovomarMsg::GetCRC()
{
    if(body[1] < 12)
    {
        return body[body[1] + 2];
    }
    else
    {
        return 0;
    }
}

unsigned char NovomarMsg::TstCRC()
{
    int i;
    unsigned char ccc = 0;
    if(Size() <= 12)
    {
        for(i = 0; i < Size() + 2; i++)
        {
            ccc += body[i];
        }
        return ccc + 1;
    }
    return 0;
}

int NovomarMsg::ChkCRC()
{
    unsigned char cc, rr;
    int idx;
    idx = body[1] + 2;
    cc = body[idx]; //GetCRC();
    rr = TstCRC();

    if( body[idx] == rr )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char NovomarMsg::CalcCRC()
{
    int i;
    unsigned char c = 0;
    if(Size() <= 12)
    {
        for(i = 0; i < Size() + 2; i++)
        {
            c += body[i];
        }
        body[Size() + 2] = c + 1;
        return c + 1;
    }
}

unsigned NovomarMsg::Add(unsigned char byte)
{
    if(counter < 12)
    {
        body[counter++] = byte;
        return 1;
    }
    else
    {
        return 0;
    }
}

void NovomarMsg::Init()
{
    counter = 0;
    for(int i = 0; i < 12; i++)
    {
        body[i]=0;
    }
}

