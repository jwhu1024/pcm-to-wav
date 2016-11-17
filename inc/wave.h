// WAVE file header format
typedef struct {
    unsigned char   chunk_id[4];        // RIFF string
    unsigned int    chunk_size;         // overall size of file in bytes (36 + data_size)
    unsigned char   sub_chunk1_id[8];   // WAVEfmt string with trailing null char
    unsigned int    sub_chunk1_size;    // 16 for PCM.  This is the size of the rest of the Subchunk which follows this number.
    unsigned short  audio_format;       // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
    unsigned short  num_channels;       // Mono = 1, Stereo = 2
    unsigned int    sample_rate;        // 8000, 16000, 44100, etc. (blocks per second)
    unsigned int    byte_rate;          // SampleRate * NumChannels * BitsPerSample/8
    unsigned short  block_align;        // NumChannels * BitsPerSample/8
    unsigned short  bits_per_sample;    // bits per sample, 8- 8bits, 16- 16 bits etc
    unsigned char   sub_chunk2_id[4];   // Contains the letters "data"
    unsigned int    sub_chunk2_size;    // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
} wav_header_t;
