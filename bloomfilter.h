#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

typedef unsigned int (*hashfunc_t)(const unsigned char *, size_t len);

unsigned int sax_hash(const unsigned char *, size_t len);
unsigned int sdbm_hash(const unsigned char *, size_t len);
unsigned int fnv(const unsigned char *key, size_t len);
unsigned int murmur3(const unsigned char *key, size_t len);
	
class BloomFilter {
	unsigned char *filter;
	size_t filterLen;
	hashfunc_t *funcs;
	size_t numFuncs;

	void setBit(unsigned int h);
	int getBit(unsigned int h) const;
public:
	BloomFilter() : filter(NULL), filterLen(0), funcs(NULL), numFuncs(0) {};
	~BloomFilter();
	
	int Create(size_t filterLen, size_t numFuncs, hashfunc_t foo1, ...);
	int Set(const unsigned char *buf, size_t bufSize);
	const unsigned char *Get(size_t &bufSize) const;
	void Clear();
	void Add(const unsigned char *buf, size_t bufSize);
	int Check(const unsigned char *buf, size_t bufSize) const;
};

#endif // BLOOMFILTER_H