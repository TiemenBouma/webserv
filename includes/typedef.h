#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <string>
#include <vector>
#include <map>

using namespace std;

typedef std::map<std::string, std::vector<std::string > > map_str_vec_str;
typedef std::map<std::string, std::string> map_str_str;

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

#define MIME_TYPES_FILE "../data/MIME_TYPES.txt"
#define HOMEPAGE_FILE "../data/homepage.html"
#define ROOT_DIR "../data/"
const int PORT = 8080;
const int MAX_CONNECTIONS = 100;
const int BUFFER_SIZE = 1024;
// typedef struct sockaddr_in SA_IN;
// typedef struct sockaddr SA;

#endif
