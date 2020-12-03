#include "BitArray.h"
#include <assert.h>
#include <stdint.h>

BitArray::BitArray(unsigned int size)
{
	assert(size > 0);
	totalBits = size;
	m_bits.reserve(size);
	totalBytes = BIT_TO_CHAR(size);
	for (uint32_t i = 0; i < size; i++)
		m_bits.push_back(0);
}

BitArray::~BitArray()
{
}

void BitArray::ClearAllBits()
{
	m_bits.assign(m_bits.size(), 0);
}

void BitArray::SetAllBits()
{
	m_bits.assign(m_bits.size(), 0xff);
}

void BitArray::SetByte(unsigned int byte, unsigned char val)
{
}

void BitArray::SetBit(unsigned int bit)
{
	int index = bit / BIT_PER_BYTE;
	int offset = BIT_PER_BYTE - 1 - bit % BIT_PER_BYTE;

	m_bits[index] |= (1 << offset);
}

void BitArray::CleatBit(unsigned int bit)
{
	assert(totalBits > bit);
	unsigned char mask = BIT_IN_CHAR(bit);
	mask = ~mask;

	m_bits[BIT_TO_CHAR(bit)] &= mask;
}

bool BitArray::operator[](unsigned int bit) const
{
	assert(totalBits > bit);
	return m_bits[BIT_TO_CHAR(bit)] & BIT_IN_CHAR(bit);
}
