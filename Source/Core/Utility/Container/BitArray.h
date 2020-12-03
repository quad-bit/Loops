#pragma once
#include <Vector>


#define BIT_PER_BYTE 8
#define BIT_TO_CHAR(bit) ((bit) / BIT_PER_BYTE)
#define BIT_IN_CHAR(bit) (1 << (BIT_PER_BYTE - 1 - ((bit)%BIT_PER_BYTE )))

class BitArray
{

private:
    std::vector<unsigned char> m_bits;
    unsigned int totalBits, totalBytes;

public:
    BitArray(unsigned int size);
    ~BitArray();

    void ClearAllBits();
    void SetAllBits();
    void SetByte(unsigned int byte, unsigned char val);
    void SetBit(unsigned int bit);
    void CleatBit(unsigned int bit);
    bool operator [] (unsigned int bit) const;
};
