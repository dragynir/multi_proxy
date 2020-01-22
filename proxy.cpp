#include"proxy.h"


Proxy::Proxy(int listener){
	this->listener = listener;
	this->is_alive = true;
}



void * start_new_session(void * session_params){


    assert(NULL != session_params);

    session_attr * attr = (session_attr *)session_params;


    assert(NULL != attr->cache);



    Session * session = NULL;
    try
    {
        session = new Session(attr->client_socket, attr->cache);
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        free(attr);
        return NULL;
    }
    free(attr);



    int res = 0;

    if(session->read_client_request() < 0){
        delete session;
        std::cout << "Thread out" << "\n";
        return NULL;
    }


    if(SEND_REQUEST == session->getState()){
        if(session->send_request() < 0){
            delete session;
            std::cout << "Thread out" << "\n";
            return NULL;
        }
        if(session->manage_response() < 0){
            delete session;
            std::cout << "Thread out" << "\n";
            return NULL;
        }
    }else if(USE_CACHE == session->getState()){
        if(session->use_cache() < 0){
            delete session;
            std::cout << "Thread out" << "\n";
            return NULL;
        }
    }else{
        std::cout << "Unknown state" << "\n";
    }

    std::cout << "Thread out" << "\n";
    delete session;
    return NULL;
}





void Proxy::start(){

    errno = pthread_attr_init(&this->pthread_detach_attr);

    if(0 != errno){
        perror("pthread_attr_init");
        return;
    }


    errno = pthread_attr_setdetachstate(&this->pthread_detach_attr, PTHREAD_CREATE_DETACHED);


    if (0 != errno){
        perror("pthread_attr_setdetachstate");
        return;
    }


    int res = 0;
	while(this->is_alive){
        res = accept_connection();
        if(res < 0){
            close(this->listener);
            break;
        }
	}
}



/*struct session_attr{
    int client_socket;
    std::map<std::string, CacheRecord *> * cache;
};*/

int Proxy::accept_connection(){
	int client_socket = accept(this->listener, NULL, NULL);

	if(client_socket < 0){
		perror("accept");
		return -1;
	}

    pthread_t thread_id;

    session_attr * attr = (session_attr *)malloc(sizeof(session_attr));

    if(NULL == attr){
        perror("malloc");
        return -1;
    }

    attr->client_socket = client_socket;
    attr->cache = &this->cache;

	

    errno = pthread_create(&thread_id, &this->pthread_detach_attr, start_new_session, attr);
    

    if(0 != errno){
        free(attr);
        perror("pthread_create");
        return -1;
    }

	return 0;
}	






Proxy::~Proxy(){

}
