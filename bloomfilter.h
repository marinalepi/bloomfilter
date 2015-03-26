#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

typedef unsigned int (*hashfunc_t)(unsigned char *, size_t len);

unsigned int sax_hash(const unsigned char *, size_t len);
unsigned int sdbm_hash(const unsigned char *, size_t len);

#define CHAR_BIT 8
	
class BloomFilter {
	unsigned char *filter;
	size_t filterLen;
	hashfunc_t *funcs;
	size_t numFuncs;

	void setBit(unsigned int h);
	int getBit(unsigned int h) const;
public:
	BloomFilter() : filterLen(0), filter(NULL), numFuncs(0), funcs(NULL) {};
	~BloomFilter();
	
	int Create(size_t filterLen, size_t numFuncs, hashfunc_t foo1, ...);
	void Clear();
	void Add(unsigned char *buf, size_t bufSize);
	int Check(unsigned char *buf, size_t bufSize) const;
};

#endif // BLOOMFILTER_H