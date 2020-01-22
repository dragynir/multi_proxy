
#include"session.h"


#include<string>
#include<vector>


class Proxy{


public:

	Proxy(int listener);

	~Proxy();


	void start();

private:

	int accept_connection();


	int listener;
	bool is_alive;
	SafeCacheMap cache;
	pthread_attr_t pthread_detach_attr;
	//std::vector<Session *> sessions;
};
