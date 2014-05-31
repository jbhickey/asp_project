#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "file_io.h"
#include "prediction.h"
#include "encoder.h"
#include "decoder.h"

char *g_data_buf;
char *error;
char *write_buf;

int main(int argc, char *argv[])
{    
	int block_size = 0;
	int file_size = 0;
	int block_cnt = 0;
	
	if(argc != 3)
	{
		printf("Incorrect input arguments\n");
		printf("Usage: ./codec filename blocksize\n"); 
		printf("Example: ./codec Samples/test441100.wav 1024\n"); 
	}
	else
	{
		block_size = atoi(argv[2]);

		error = (char*) malloc (sizeof(char)*block_size);
		write_buf = (char*) malloc (sizeof(char)*block_size);
	
		/* Filename as argument */
		file_size = read_file(argv[1]);

		/* Iterate through input data */
		for(block_cnt=0;block_cnt<(file_size/block_size);block_cnt++)
		{
			/* 0.5s[n] - 0.5s[n-1]? */
			//avg_predictor(&g_data_buf[block_cnt*block_size], error, block_size);
			
			/* Encode error vector */
			//encode(error, write_buf, block_size);
			encode(&g_data_buf[block_cnt*block_size], write_buf, block_size);
			
			/* Write encoded data to file */
			write_block("Output/encoded.out", write_buf, block_size);
				
			/* Clear write buffer by size of block not strlen */
			//memset(write_buf, 0x00, block_size);
		}
				
		free(g_data_buf);

		printf("Encoding complete...\n");
		printf("Beginning decoding...\n");

		/* Read encoded output file */
		file_size = read_file("Output/encoded.out");
		for(block_cnt=0;block_cnt<(file_size/block_size);block_cnt++)
		{
			/* Decode to error vector */
			decode(error, &g_data_buf[block_cnt*block_size], block_size);
			
			/* Write encoded data to file */
			write_block("Output/decoded.out", error, block_size);
				
			/* Clear write buffer */
			memset(error, 0x00, block_size);
		}

		printf("Decoding complete...\n");
		  
		/* Free up used memory */
		free(error);
		free(write_buf);
		free(g_data_buf);
	}
	return 0;
}


