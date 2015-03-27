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
	
	this->filterLen = filterLen+sizeof(char);
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

void BloomFilter::Add(const unsigned char *buf, size_t bufSize) {
	for(size_t i=0; i<this->numFuncs; ++i) {
		setBit(this->funcs[i](buf, bufSize) % this->filterLen);
	}
}

int BloomFilter::Check(const unsigned char *buf, size_t bufSize) const {
	for(size_t i=0; i<this->numFuncs; ++i) {
		if (!getBit(this->funcs[i](buf, bufSize) % this->filterLen)) {
			return 0;
		}
	}

	return 1;
}

int BloomFilter::Set(const unsigned char *buf, size_t bufSize) {
	if (this->filter) {
		delete this->filter;
	}	
	this->filterLen = bufSize;
	this->filter = new unsigned char(this->filterLen);
	if (!this->filter) {
		return ENOMEM;
	}
	memcpy(this->filter, buf, sizeof(unsigned char)*bufSize);
	return 0;
}

const unsigned char *BloomFilter::Get(size_t &bufSize) const {
	bufSize = this->filterLen;
	return this->filter;
}

void BloomFilter::setBit(unsigned int h) {
	this->filter[h/sizeof(char)] |= (1<<(h%sizeof(char)));
}

int BloomFilter::getBit(unsigned int h) const {
	return this->filter[h/sizeof(char)] & (1<<(h%sizeof(char)));
}