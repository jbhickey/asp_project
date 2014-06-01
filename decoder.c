#include <stdio.h>
#include <string.h>
#include "decoder.h"

/* Decode an error vector for each sample in encoded block */
void decode(char *e, char *buf, int block_size, int k)
{
	int n = 0;
	int z = 0;
	int cnt = 0;

	/* Clear error */
	memset(e, 0x00, block_size);

	for(n=0;n<block_size;n++)
	{				
		/* Buffer encoded data and affix last k bits */
		e[n] = buf[n];
		e[n] &= (0xFF >> (8-k));
		
		/* Count how many zeroes occur */		
		while((buf[n] & (0x01 << (k+cnt))) == 0){
			cnt++;
			z++;	
		}

		/* Shift z to kth bit */
		z <<= k;
		
		/* Affix z */
		e[n] |= z;
	}
}

/* Simply returns top three bits of first byte in current block */
int get_encoded_k(char *buf)
{
	return (int)buf[0];
}
