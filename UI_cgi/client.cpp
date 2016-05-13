#include <iostream>
#include <string>
#include <map>
#include "string.h"
#include "getpost.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_BUF 10024

using namespace std;


void send(const char* topic)
{
    int fd;
    umask(0);
    char * myfifo = "/usr/lib/cgi-bin/myfifo";

    //cout<<"Topic = "<<topic<<endl;

    /* create the FIFO (named pipe) */
    if(mkfifo(myfifo, 0777|O_CREAT)<0)
        perror("Named Pipe not working");

    /* write query entered to the FIFO */
    fd = open(myfifo, O_WRONLY|O_NONBLOCK);
    if(write(fd, topic, (unsigned)strlen(topic))<0)
        perror("Unable to write in pipe");

    close(fd);
    int test;

    /* reading back results from fifo*/

    char buf[MAX_BUF];
    fd = open(myfifo, O_RDONLY);
    test = read(fd, buf, MAX_BUF);
    buf[test] = '\0';
    close(fd);

    //Retrieved tuples
    cout<<buf;

    //reading back ends
}

int main()
{
    map<string,string> Get;
    initializeGet(Get); //notice that the variable is passed by reference!
    cout<<"Content-type: text/html"<<endl<<endl;
    //cout<<"<html><body>"<<endl;
    string topic;
    if (Get.find("query")!=Get.end())
    {
        topic = Get["query"];
        //cout<<"Want to Search - "<<topic<<endl;

        //Converting to char* from string
        char *con = new char[topic.length()+1];
        copy(topic.begin(), topic.end(),con);
        con[topic.length()] = '\0';
        send(con);
    }
    else
    {
        cout<<"Enter Query And press Search"<<endl;
    }
    //cout<<"</body></html>"<<endl;
    return 0;
}