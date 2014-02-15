#include "comtypedef.h"
#include "comtypedef_vchar.h"
#include "tinyxml.h"
#include "udpsocket.h"
#include "getopt_pp.h"
#include "threadif.h"
#include "fifo.h"
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace GetOpt;

char * appName = "XClient";

const char * defaultConfig = "config.xml";
/********************************************/
const int maxLen = 10000;
#pragma pack (1)
struct Command
{
    int type;  //0 present for reload,1 present for status
    int taskId;
    int instId;
    char cmd[100];
    Command():taskId(-1), instId(-1)
    {
        cmd[0] = 0;
    }
};
/********************************************/

class SocketThread : public ThreadIf
{
public:
	SocketThread(CUDPSocket & socket):mThreadName("SocketThread")
	{
        mSocket = &socket;
        code.content = new CHAR[maxLen];
	}
	~SocketThread(){delete code.content;}
    void process()
    {
        try
        {
            code.length = 0;
            memset(code.content, 0, maxLen);
            if(mSocket->recvCode(code)== 1 && code.length >0)
            {
                cout<<code.content<<"\n";
                cout<<">";
                cout.flush();
            }
        }
        catch (exception& e)
        {
            cout<<"Unhandled exception: "<< e.what()<<endl;
        }
    }
private:
	CStr	mThreadName;
    CUDPSocket * mSocket;
    BOOL    success;
    CCode   code;
};


bool getConfig(string & remoteIp, int & remotePort)
{
	TiXmlDocument *config=new TiXmlDocument(defaultConfig);
	if(config==NULL || !config->LoadFile())
	{
		cout<<"can not open config file:" << defaultConfig <<endl;
		if(config) delete config;
		return false;
    }
    TiXmlHandle handle(config->RootElement());
	TiXmlElement* xclient=NULL;
	xclient=handle.FirstChild("xclient").Element();
    if(xclient)
	{
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
		remoteIp = xclient->Attribute("remoteIp");
		if(remoteIp == "") remoteIp = "127.0.0.1";
		xclient->Attribute("remotePort", &remotePort);
		if(remotePort == 0) remotePort = 9000;
		cout<<"remote Ip : "<< remoteIp << "\nremote Port : "<<remotePort<<endl;
    }
    else
    {
        cout<<"can read config field <xclient> in " << defaultConfig <<endl;
		if(config) delete config;
		return false;
    }
    if(config) delete config;
    return true;
}
void show_help()
{
    CStr str;
    str<<"usage : [-t|--taskid taskid] [-i|--instid instid] -c|--comand command\n";
    str<<"command should be one of the next:\n";
    str<<"\tset logLevel DEBUG|default\n";
    str<<"\treload all|log|db|timer\n";
    str<<"\twatch status\n";
    str<<"use \"quit\" or \"exit\" to stop it";
    cout<<str.c_str()<<endl;
}

bool parse_cmd(GetOpt_pp &ops, Command &cmd)
{
    vector<string> command;
    ops>>Option('c',"command", command);
    if(command.size() == 0)
    {
        cout<< "can not find command string" <<endl;
        return false;
    }
    else
    {
        //reload or set log level
        if(command[0] == "reload"||command[0] == "set") cmd.type = 0;
        else if(command[0] == "watch") cmd.type = 1;
        else
        {
            cout<< "command string is not correct" <<endl;
            return false;
        }
        for(int i=0;i<command.size();i++)
        {
            strcat(cmd.cmd, command[i].c_str());
            strcat(cmd.cmd, " ");
        }
    }
    ops>>Option('t', "taskid", cmd.taskId);
    ops>>Option('i', "instid", cmd.instId);

    //debug out
    //cout<< "taskid: "<< cmd.taskId<<endl;
    //cout<< "instid: "<< cmd.instId<<endl;
    //cout<< "command: "<< cmd.cmd<<endl;

    return true;
}

void send_to_remote(CUDPSocket & socket, CHAR * remoteIp, INT remotePort, const char * params, const int len)
{
    string ret = "";
    CCode code;
    code.content = (CHAR *)params;
    code.length = len;
    int sendstatus = socket.sendCode(code, remoteIp, remotePort);
}

void splitArgs(char * line, int & argc, char ** argv)
{
    char *tok = strtok(line, " \t");
    while(tok!=NULL)
    {
        argv[argc++] = tok;
        tok=strtok(NULL," \t");
    }
}
int main(int argc, char **argv)
{
    string remoteIp = "127.0.0.1";
    int remotePort = 9000;

    if(!getConfig(remoteIp, remotePort))
    {
        cout<<"read config failed"<<endl;
        return 0;
    }

    char line[100];
    char * args[20];
    args[0] = "";

    CStr * recvStr = NULL;
    CUDPSocket socket;
    BOOL success = socket.openServer(NULL, remotePort+1);
    if(!success)
    {
        cout<< "create socket failed"<<endl;
        return 0;
    }
    socket.setRecvBufSize(maxLen);
    socket.setSendBufSize(maxLen);

    SocketThread mThread(socket);
    mThread.run();

    cout<<">";
    while(cin.getline(line, 100) && strcmp(line, "quit")!=0 && strcmp(line, "exit")!=0)
    {
        if(strlen(line) == 0)
        {
            cout<<">";
            continue;
        }
        int argcnt = 1;
        splitArgs(line, argcnt,args);
        GetOpt_pp ops(argcnt, args);
        //check if it is a show help command
        if (ops >> OptionPresent('h', "help"))
        {
            cout<<"show help"<<endl;
            show_help();
            continue;
        }
        Command cmd;
        if(parse_cmd(ops, cmd))
        {
            send_to_remote(socket, (CHAR *)remoteIp.c_str(), remotePort, (char *)&cmd, sizeof(cmd));
        }
        else
        {
            cout<<"your args are not correct!"<<endl;
            show_help();
        }
        recvStr = NULL;
        cout<<">";
        cout.flush();

    }
    mThread.detach();
    socket.closeSocket();
    return 0;
}
