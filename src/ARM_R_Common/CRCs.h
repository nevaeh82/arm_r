#ifndef CRCS_H
#define CRCS_H

/// Class for count crc

class CRCs
{
public:
	CRCs();
	~CRCs();

public:
	unsigned char crc8(const unsigned char* data, int len);
	unsigned short crc16(const unsigned char* data, int len);
	unsigned short crc16_nontable(const unsigned char* data, int len);
};

#endif // CRCS_H
