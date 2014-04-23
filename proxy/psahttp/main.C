#include<iostream>
#include "httpserver.h"
#include "httpclient.h"
using namespace std;

int main()
{
    int port = 8088;
    CHttpServer *g_httpserver = new CHttpServer;
    g_httpserver->init(NULL, port);
}
