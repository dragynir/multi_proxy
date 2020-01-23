

#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<strings.h>
#include<string.h>


#include<errno.h>
#include<assert.h>
#include<pthread.h>



#include<exception>
#include<iostream>

struct InitRwlockException : public std::exception {
   const char * what () const throw () {
      return "Can't init rwlock structure!";
   }
};


struct RwlockException : public std::exception {
   const char * what () const throw () {
      return "Rwlock lock or unlock error!";
   }
};


struct MutexError : public std::exception {
   const char * what () const throw () {
      return "Mutex lock or unlock error!";
   }
};


struct MutexInitException : public std::exception {
   const char * what () const throw () {
      return "Can't init mutex!";
   }
};


struct CondInitException : public std::exception{
   const char * what () const throw () {
      return "Can't init conditional variable!";
   }
};


struct ConditionException : public std::exception{
   const char * what () const throw () {
      return "Conditional variable error!";
   }
};




class CacheRecord{


public:

	CacheRecord(bool local);

	~CacheRecord();

	
	bool is_local(){return this->local;}

	bool is_full(){return full;}
	void finish(){this->full = true;};
	bool is_outdated(){return out_of_date;}

	void outdated(){
		this->out_of_date = true;
		this->links_count--;
	}


	int links(){return this->links_count;}

	void use(){
		this->links_count++;
	}
	void unuse(){
		if(this->links_count >= 0){
			this->links_count--;
		}
	}


	int add_data(char * data, size_t size);

	int get_size(){return size;}

	char * get_data(){return this->data;}
	



	void read_lock();
	void write_lock();
	void unlock();



	void cond_broadcast();


	void cond_wait();


	void lock_cond_mutex();


	void unlock_cond_mutex();





	/*bool is_empty(){return 0 == size;}

	bool in_progress(){return !full && (0 != size);}*/

private:

	pthread_rwlock_t rwlock;

	pthread_cond_t cond; 
    pthread_mutex_t mutex;



	int links_count;
	char * data;
	size_t capacity;
	size_t size;
	bool full;
	bool out_of_date;
	bool local;

};
