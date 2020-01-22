#include"parser.h"




// out of bound ++, check
// session free url end protocol
int HttpParser::parse_start_line(char * start_line, int line_length, http_info * info){


	char * cursor = start_line;


	char * method_end = strchr(cursor, ' ');
	*method_end = '\0';


	//parse url fix
	// strcmp(cursor, "CONNECT") != 0
    if (strcmp(cursor, "GET") != 0) {
        fprintf(stderr, "Unsupported method! %s\n", cursor);
        return -1;
    }

    *method_end = ' ';

    cursor = method_end;
    cursor++;


    //printf("%s\n", "method parsed");



	char * url_end = strchr(cursor, ' ');
	int url_length = url_end - cursor;


	char * url = (char *)malloc(url_length + 1);


	if(NULL == url){
		perror("malloc");
		return -1;
	}

	bcopy(cursor, url, url_length);
	url[url_length] = '\0';
	info->url = url;


	//printf("%s %s\n", "url parsed", url);


	*url_end = ' ';

	cursor = url_end;
	cursor++;




	/*int protocol_length = strlen(cursor);

	//printf("Length: %d\n", protocol_length);

	char * protocol = (char *)malloc(protocol_length + 1);


	if(NULL == protocol){
		perror("malloc");
		return -1;
	}

	bcopy(cursor, protocol, protocol_length);
	protocol[protocol_length] = '\0';
	info->protocol = protocol;*/

	//printf("%s ++%s++\n", "protocol parsed", protocol);


	return 0;
}




int HttpParser::parse_url(char* parsing_str, http_info * info) {
    //printf("Parsing str %s\n", parsing_str);
    if (8 > strlen(parsing_str)) {
        fprintf(stderr, "URL format: http://<host_name>[:port]/[page]>\n");
        return -1;
    }

    int url_length = strlen(parsing_str);




    char * found = strstr(parsing_str, "//");
    char * host_start = parsing_str;
    char * host_end = NULL;

    if(NULL != found){

    	if(url_length - 1 == parsing_str - found){
    		printf("%s\n", "Invalid url");
    		return -1;
    	}
    	host_start = found + 2;
    }





    found = strchr(host_start, ':');



    char * resource_start = NULL;
    if(NULL == found){
    	//printf("%s\n", "There is no port");
    	host_end = strchr(host_start + 1, '/');
    	if(NULL == host_end){
    		printf("%s\n", "Invalid url: host");
    		return -1;
    	}
    	resource_start = host_end;
    }else{
    	//printf("%s\n", "Found port");
    	host_end = found;
    	// ресурс начинается со /
    	resource_start = strchr(found, '/');
    	if(NULL == resource_start){
    		printf("%s\n", "Invalid url: resource");
    		return -1;
    	}
    }

    char keep_char = *host_end;
    *host_end = '\0';
    int host_length = strlen(host_start);
    info->host = (char *)malloc(host_length + 1);


    if(NULL == info->host){
    	perror("malloc: info.host");
    	return -1;
    }




    bcopy(host_start, info->host, host_length + 1);
    *host_end = keep_char;


    //printf("++%s++\n", info->host);
    //printf("++%s++\n", resource_start);
    int resource_length = strlen(resource_start);
    info->resource = (char *)malloc(resource_length + 1);


    if(NULL == info->resource){
    	perror("malloc: info.resource");
    	free(info->host);
    	return -1;
    }


    bcopy(resource_start, info->resource, resource_length + 1);

    return 0;
}





//============================================================: cond jump, старая функция url
// case when url with host/resource
/*int HttpParser::parse_url(char* parsing_str, http_info * info) {
    //printf("Parsing str %s\n", parsing_str);
    if (8 > strlen(parsing_str)) {
        fprintf(stderr, "URL format: http://<host_name>[:port]/[page]>\n");
        return -1;
    }

    //char host[80];
    int port;


    char protocol[8];
    strncpy(protocol, parsing_str, 7);
    protocol[7] = '\0';
    if (0 != strcmp(protocol, "http://")) {
        fprintf(stderr, "URL format: http://<host_name>[:port]/[page]>\n");
        return -1;
    }
    char* start_host = parsing_str + 7;
    char* move = start_host;

    while ((*move) != '/' && (*move) != ':') {
        move++;
    }
    char symb = (*move);
    *move = '\0';

    char* end_host = move;
    int host_length = strlen(start_host);
    char * host = (char *) malloc(host_length + 1);
    strcpy(host, start_host);
    info->host = host;
    *move = symb;

    host[end_host - start_host] = '\0';//===============================================
    //printf("host : %s\n", host);

    if ((*end_host) == ':') {
        while ((*move) !=  '/') {
            move++;
        }
        char port_str[move - end_host];
        strncpy(port_str, end_host + 1, move - end_host - 1);
        port = atoi(port_str);
        if ((port) < 0) {
            fprintf(stderr, "URL format: http://<host_name>[:port]/[page]>\n");
            return -1;
        }
    }

    int resource_length = strlen(move);

    char * resource = (char *) malloc(resource_length + 1);

    bcopy(move, resource, resource_length);
    resource[resource_length] = '\0';
    info->resource = resource;

    //strcpy(resource, move);

    return 0;
}*/


/*int HttpParser::parse_host_line(){

}*/



// доделать парсинг host при наличии host в url






// добавить парсинг хоста при наличии его после метода запроса
int HttpParser::parse_client_request(
	char *  request, int request_length, char ** url, /*char ** protocol,*/ char ** host, char ** resource){
	//printf("%s\n", "HttpParser::parse_client_request");

	//char * request = session->getBuffer();

	char * start_line_end = strchr(request, '\r');
	*start_line_end = '\0';


	//printf("All start line length: %d, %c\n", strlen(request), *(start_line_end - 1));

	http_info info;

	int res = 0;
	res = parse_start_line(request, request  - start_line_end, &info);


	if(res < 0){
		return -1;
	}


	*start_line_end = '\r';


	//printf("%s\n", "first line");

	*url = info.url;
	//*protocol = info.protocol;


	/*printf("Url: ++%s++\n", info.url);
	printf("Protocol: ++%s++\n", info.protocol);*/
		


	//printf("+++%s+++\n", info.url);

	// хост есть в url
	res = parse_url(info.url, &info);
	// в url есть хост
	if(0 == res){
		//printf("Res: ++%s++\n", info.resource);
		//printf("Host: ++%s++\n", info.host);
		*resource = info.resource;
		//free(info.url);
		*url = info.url;
		//*protocol = info.protocol;
		*host = info.host;
		return 0;
	}

	
	
	

	char * cursor = start_line_end;
	*cursor = '\r';
	cursor+=2;
	// next line




	// работает при корректном http запросе

	while(1){
		
		char * host_line_end = strchr(cursor, '\r');

		if(NULL == host_line_end){
			free(info.url);
			return -1;
		}

		*host_line_end = '\0';

		char * has_host = strstr(cursor, "Host:");

		if(NULL != has_host){
			cursor = strchr(cursor, ' ');

			if(NULL == cursor){
				free(info.url);
				//free(info.protocol);
				return -1;
			}


			cursor++;

			int host_length = host_line_end - cursor;

			char * host = (char *)malloc(host_length);


			if(NULL == host){
				perror("malloc");
				free(info.url);
				//free(info.protocol);
				return -1;
			}


			bcopy(cursor, host, host_length);
			info.host = host;
			break;
		}

		cursor = host_line_end;
		*cursor = '\r';
		cursor+=2;
	}

	//printf("-------++%s++\n", info.host);

	*url = NULL;
	*resource = info.url;
	//*protocol = info.protocol;
	*host = info.host;




	return 0;
}
