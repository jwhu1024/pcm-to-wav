#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wave.h"

char* dummy_get_raw_pcm (char *p, int *bytes_read)
{
	long lSize;
	char *pcm_buf;
	size_t result;
	FILE *fp_pcm;
	
	fp_pcm = fopen (p, "rb");
	if (fp_pcm == NULL) {
		printf ("File error");
		exit (1);
	}
	
	// obtain file size:
	fseek (fp_pcm , 0 , SEEK_END);
	lSize = ftell (fp_pcm);
	rewind (fp_pcm);
	
	// allocate memory to contain the whole file:
	pcm_buf = (char*) malloc (sizeof(char) * lSize);
	if (pcm_buf == NULL) {
		printf ("Memory error");
		exit (2);
	}
	
	// copy the file into the pcm_buf:
	result = fread (pcm_buf, 1, lSize, fp_pcm);
	if (result != lSize) {
		printf ("Reading error");
		exit (3);
	}
	
	*bytes_read = (int) lSize;
	return pcm_buf;	
}

void get_wav_header(int raw_sz, wav_header_t *wh)
{
	// RIFF chunk
	strcpy(wh->chunk_id, "RIFF");
	wh->chunk_size = 36 + raw_sz;
	
	// fmt sub-chunk (to be optimized)
	strncpy(wh->sub_chunk1_id, "WAVEfmt ", strlen("WAVEfmt "));
	wh->sub_chunk1_size = 16;
	wh->audio_format = 1;
	wh->num_channels = 1;
	wh->sample_rate = 16000;
	wh->bits_per_sample = 16;
	wh->block_align = wh->num_channels * wh->bits_per_sample / 8;
	wh->byte_rate = wh->sample_rate * wh->num_channels * wh->bits_per_sample / 8;
	
	// data sub-chunk
	strncpy(wh->sub_chunk2_id, "data", strlen("data"));
	wh->sub_chunk2_size = raw_sz;
}

void dump_wav_header (wav_header_t *wh)
{
	printf ("=========================================\n");
	printf ("chunk_id:\t\t\t%s\n", wh->chunk_id);
	printf ("chunk_size:\t\t\t%d\n", wh->chunk_size);
	printf ("sub_chunk1_id:\t\t\t%s\n", wh->sub_chunk1_id);
	printf ("sub_chunk1_size:\t\t%d\n", wh->sub_chunk1_size);
	printf ("audio_format:\t\t\t%d\n", wh->audio_format);
	printf ("num_channels:\t\t\t%d\n", wh->num_channels);
	printf ("sample_rate:\t\t\t%d\n", wh->sample_rate);
	printf ("bits_per_sample:\t\t%d\n", wh->bits_per_sample);
	printf ("block_align:\t\t\t%d\n", wh->block_align);
	printf ("byte_rate:\t\t\t%d\n", wh->byte_rate);
	printf ("sub_chunk2_id:\t\t\t%s\n", wh->sub_chunk2_id);
	printf ("sub_chunk2_size:\t\t%d\n", wh->sub_chunk2_size);
	printf ("=========================================\n");
}

int main (int argc, char* argv[])
{
	int raw_sz = 0;
	FILE *fwav;
	wav_header_t wheader;
	
	memset (&wheader, '\0', sizeof (wav_header_t));
	
	// check argument
	if (argc != 2)
		return -1;
	
	// dummy raw pcm data
	char *pcm_buf = dummy_get_raw_pcm ("./time.pcm", &raw_sz);
	
	// construct wav header
	get_wav_header (raw_sz, &wheader);
	dump_wav_header (&wheader);
	
	// write out the .wav file
	fwav = fopen(argv[1], "wb");
	fwrite(&wheader, 1, sizeof(wheader), fwav);
	fwrite(pcm_buf, 1, raw_sz, fwav);
	fclose(fwav);
	
	if (pcm_buf)
		free (pcm_buf);
	
	return 0;
}
