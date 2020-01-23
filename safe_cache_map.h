#pragma once

#include<map>
#include<string>

#include"cache.h"






//exceptions for mutex error

class SafeCacheMap{

public:

	SafeCacheMap();

	~SafeCacheMap();

	std::map<std::string, CacheRecord *>::iterator find(std::string& key);

	void insert(std::string& key, CacheRecord * record);

	void erase(std::map<std::string, CacheRecord *>::iterator it);

	std::map<std::string, CacheRecord *>::iterator end(){return this->cache.end();}

	void lock();
	void unlock();

private:

	pthread_mutex_t mutex;
	std::map<std::string, CacheRecord *> cache;
};
