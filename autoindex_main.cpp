/**
 * @file autoindex.cpp
 * @author swenne (swofferh@student.codam.nl)
 * @brief utilities for generating response with directories listing
 * 
 * @version 0.1
 * @date 2023-03-06
 * @copyright Copyright (c) 2023
 */


#include <sstream>      // strol
#include <dirent.h>     // DIR
#include <stdlib.h>     // for exit()
#include <sys/stat.h> 

#include <iostream>    // for cout, cerr - to output messages.
#include <fstream>     // for ofstream -  to write to a file.
#include <string>      // for string -  to store the path to the directory.
#include <vector>      // for vector - to store the filenames in the directory.
#include <dirent.h>    // for open,read,closedir(), struct dirent - to represent a directory entry.
#include <cstdlib>     // for exit() - to exit the program with an error code when necessary.

using namespace std;
void error_pages_generator(ofstream outfile, int error_number, string msg_error_type)
{
    outfile << "<!DOCTYPE html>";
    outfile << "<html>\n";
    outfile << "<head><title> ERROR " "</title></head>\n";
    outfile << "<body>\n";
    outfile << "<h1> Type: " << error_number << "</h1>\n";
    outfile << "<h1> Type: " << msg_error_type << "</h1>\n";
    outfile << "<hr>\n";
    outfile << "<pre>\n";
        //outfile << "<a href=\"" localhost:8080\ "\">" << Back to home << "</a>\n";
    outfile << "</pre>\n";
    outfile << "<hr>\n";
    outfile << "</body>\n";
    outfile << "</html>\n";
}

vector<string> autoindex_response(char *directory_path)
{
    vector<string> response;
    cout << "<html>\n";
    cout << "<head><title>Index of " << directory_path << "</title></head>\n";
    cout << "<body>\n";
    cout << "<h1>Index of " << directory_path << "</h1>\n";
    cout << "<hr>\n";
    cout << "<pre>\n";
    for (vector<string>::const_iterator it = response.begin(); it != response.end(); ++it) 
    {
        const string& filename = *it;
        if (filename == "." || filename == "..")
            continue;
        cout << "<a href=\"" << filename << "\">" << filename << "</a>\n";
    }
    cout << "</pre>\n";
    cout << "<hr>\n";
    cout << "</body>\n";
    cout << "</html>\n";
    return(response);
}

int main(int argc, char* argv[]) 
{
// [IDEA] main describing expected input with argv[0]
    if (argc != 2)
    {
        cerr << "Usage: [" << argv[0] << "] [directory]\n";
        return 1;
    }

// [CHECK] simple error handling, need more protection?
    DIR* dirp = opendir(argv[1].c_str());
    if (dirp == NULL) 
    {
        cerr << "Error: " << argv[1] << " is not a directory.\n";
        return 1;
    }

// [SUBJECT] not sure if we can use dirent.h functions/struct.
    struct dirent* dp;
    vector<string> filenames = autoindex_response(argv[1]); //using arg for now
    while ((dp = readdir(dirp)) != NULL)
        filenames.push_back(dp->d_name);
    closedir(dirp);

// [IDEA] error pages generator, outstream file for now -testing.
    ofstream outfile("error_page");
    if (!outfile.is_open()) 
    {
        cerr << "Error: Failed to open output file.\n";
        return 1;
    }
    error_pages_generator(outfile, 404); // using 404 for now
    outfile.close();

    return 0;
}
