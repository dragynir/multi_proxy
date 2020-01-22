
#include"safe_cache_map.h"


SafeCacheMap::SafeCacheMap(){
	pthread_mutex_init(&this->mutex, NULL);
}



std::map<std::string, CacheRecord *>::iterator SafeCacheMap::find(std::string& key){

	//lock_mutex();
	/*CacheRecord * record = NULL;
	std::map<std::string, CacheRecord *>::iterator it = this->cache->find(key);

	if(this->cache->end() != it){
		record = it->second;
	}*/

	//unlock_mutex();

	return this->cache.find(key);
}



void SafeCacheMap::insert(std::string& key, CacheRecord * record){
	//lock_mutex();

	this->cache.insert(std::pair<std::string, CacheRecord *>(key, record));

	//unlock_mutex();
}


void SafeCacheMap::erase(std::map<std::string, CacheRecord *>::iterator it){
	this->cache.erase(it);
}



int SafeCacheMap::lock(){
	errno = pthread_mutex_lock(&this->mutex);
	if(0 != errno){
		perror("pthread_mutex_lock");
		//throw
		return -1;
	}
}

int SafeCacheMap::unlock(){
	errno = pthread_mutex_unlock(&this->mutex);
	if(0 != errno){
		perror("pthread_mutex_lock");
		//throw
		return -1;
	}
}