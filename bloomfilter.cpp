#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "bloomfilter.h"
	
BloomFilter::~BloomFilter() {
	if (filter) {
		delete filter;
	}
	if (funcs) {
		delete [] funcs;
	}
}

int BloomFilter::Create(size_t filterLen, size_t numFuncs, hashfunc_t foo1, ...) {
	if (filter) {
		return 0; // call only once
	}
	
	// invalid argument - size
	if (filterLen < 1 || numFuncs < 1) {
		return EINVAL;
	}
	
	this->filterLen = filterLen+CHAR_BIT;
	this->filter = new unsigned char(this->filterLen);
	if (!this->filter) {
		return ENOMEM;
	}

	this->numFuncs = numFuncs;
	this->funcs = new hashfunc_t[this->numFuncs];
	if (!this->funcs) {
		return ENOMEM;
	}
	this->funcs[0] = foo1;

	va_list l;
	va_start(l, foo1);
	for(size_t i=1; i<this->numFuncs; ++i) {
		this->funcs[i] = va_arg(l, hashfunc_t);
	}
	va_end(l);		
	
	return 0;
}

void BloomFilter::Clear() {
	if (this->filter) {
		memset(this->filter, 0, sizeof(unsigned char)*this->filterLen);
	}
}

void BloomFilter::Add(unsigned char *buf, size_t bufSize) {
	for(size_t i=0; i<this->numFuncs; ++i) {
		setBit(this->funcs[i](buf, bufSize) % this->filterLen);
	}
}

int BloomFilter::Check(unsigned char *buf, size_t bufSize) const {
	for(size_t i=0; i<this->numFuncs; ++i) {
		if (!getBit(this->funcs[i](buf, bufSize) % this->filterLen)) {
			return 0;
		}
	}

	return 1;
}

void BloomFilter::setBit(unsigned int h) {
	this->filter[h/CHAR_BIT] |= (1<<(h%CHAR_BIT));
}

int BloomFilter::getBit(unsigned int h) const {
	return this->filter[h/CHAR_BIT] & (1<<(h%CHAR_BIT));
}