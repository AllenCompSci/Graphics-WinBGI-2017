#ifndef MP3_H
#define MP3_H
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdlib>

size_t GetMP3Duration(const std::string sFileName);


unsigned DecodeMP3SafeInt(unsigned nVal)
{
	// nVal has 4 bytes (8-bits each)
	//  - discard most significant bit from each byte
	//  - reverse byte order
	//  - concatenate the 4 * 7-bit nibbles into a 24-bit size.
	unsigned char *pValParts = reinterpret_cast<unsigned char *>(&nVal);
	return (pValParts[3] & 0x7F) |
		((pValParts[2] & 0x7F) << 7) |
		((pValParts[1] & 0x7F) << 14) |
		((pValParts[0] & 0x7F) << 21);
}

#pragma pack(1)
struct MP3Hdr {
	char tag[3];
	unsigned char maj_ver;
	unsigned char min_ver;
	unsigned char flags;
	unsigned int  size;
};
struct MP3ExtHdr {
	unsigned int  size;
	unsigned char num_flag_bytes;
	unsigned char extended_flags;
};
struct MP3FrameHdr {
	char frame_id[4];
	unsigned size;
	unsigned char flags[2];
};
#pragma pack()

size_t GetMP3Duration(const std::string sFileName)
{
	std::ifstream fin(sFileName.c_str(), std::ifstream::binary);
	if (!fin)
		throw std::invalid_argument("Cannot open file");

	// Read Header
	MP3Hdr hdr = { 0 };
	fin.read(reinterpret_cast<char *>(&hdr), sizeof(hdr));
	if (!fin.good())
		throw std::invalid_argument("Error reading file");

	if (0 != ::memcmp(hdr.tag, "ID3", 3))
		throw std::invalid_argument("Not an MP3 File");

	// Read extended header, if present
	if (0 != (hdr.flags & 0x40))
	{
		fin.seekg(sizeof(MP3ExtHdr), std::ifstream::cur);
		if (!fin.good())
			throw std::invalid_argument("Error reading file");
	}

	// read a chunk of file.
	const size_t nDefaultSize(2048);
	std::vector<char> vBuff(nDefaultSize);
	fin.read(&vBuff[0], vBuff.size());
	size_t nSize = fin.gcount();
	if (!nSize)
		throw std::invalid_argument("Error reading file");
	vBuff.resize(nSize);

	size_t nUsed = 0;
	while (nSize - nUsed > sizeof(MP3FrameHdr))
	{
		MP3FrameHdr *pFrame = reinterpret_cast<MP3FrameHdr *>(&vBuff[nUsed]);
		nUsed += sizeof(MP3FrameHdr);
		size_t nDataLen = DecodeMP3SafeInt(pFrame->size);
		if (nDataLen > (nSize - nUsed))
			throw std::invalid_argument("Corrupt file");

		if (!::isupper(pFrame->flags[0])) // past end of tags
			return 0;

		if (0 == ::memcmp(pFrame->frame_id, "TLEN", 4))
		{
			// skip an int
			nUsed += sizeof(int);
			// data is next
			return atol(&vBuff[nUsed]);
		}
		else
		{
			nUsed += nDataLen;
		}
	}
	return 0;
}

#endif