#include <stdlib.h>

unsigned int sax_hash(unsigned char *key, size_t len) {
	unsigned int h = 0;

	unsigned char *keyEnd = key+len;
	while (key <= keyEnd) {
		h ^= (h<<5) + (h>>2) + (*key)++;
	}
	return h;
}

unsigned int sdbm_hash(unsigned char *key, size_t len) {
	unsigned int h = 0;

	unsigned char *keyEnd = key+len;
	while (key <= keyEnd) {
		h = (*key)++ + (h<<6) + (h<<16) - h;
	}
	return h;
}

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

unsigned int murmur3(unsigned char *key, size_t len) {
	const size_t nblocks = len / 4;

	unsigned int h = 0;

	const unsigned int c1 = 0xcc9e2d51;
	const unsigned int c2 = 0x1b873593;

	// body

	const unsigned int * blocks = (const unsigned int *)(key + nblocks*4);
	for(int i = -nblocks; i; i++) {
		uint32_t k1 = blocks[i];

		k1 *= c1;
		k1 = ROTATE_LEFT(k1,15);
		k1 *= c2;
    
		h ^= k1;
		h = ROTATE_LEFT(h,13); 
		h = h*5 + 0xe6546b64;
	}

	// tail

	const uint8_t * tail = (const uint8_t*)(key + nblocks*4);

	uint32_t k1 = 0;

	switch(len & 3)
	{
	case 3: k1 ^= tail[2] << 16;
	case 2: k1 ^= tail[1] << 8;
	case 1: k1 ^= tail[0];
          k1 *= c1; k1 = ROTATE_LEFT(k1,15); k1 *= c2; h ^= k1;
	};

	// finalization

	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;	

	return h;
}

unsigned int fnv(unsigned char *key, size_t len) {
	unsigned int h = 2166136261;

	unsigned char *keyEnd = key+len;
	while (key <= keyEnd) {
        h = h ^ (*key)++;
        h = h * 16777619;
	}
	return h;
}