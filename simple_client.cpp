#include "simple_client.hpp"

using namespace std;

int SimpleClient::serverShots = 0;

int SimpleClient::serverShotsMax = 5;

int SimpleClient::totalClients = 0;

SimpleClient::SimpleClient()
{
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Creating simple client, id = " << totalClients++ << endl;
    const char serverIP[] = "127.0.0.1";
    int serverPort = 50000;
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient < 0) {
        cout << "Can't open socket." << endl;
        return;
    }
    sockaddr socketAddr;
    sockaddr_in *sPtr = (sockaddr_in*)&socketAddr;
    sPtr->sin_family = AF_INET;
    sPtr->sin_port = serverPort;
    if ( inet_pton(AF_INET, serverIP, (void*)&sPtr->sin_addr) == -1 ) {
        cout << "Error in address." << endl;
        return;
    }
    cout << "Creating socket ok.\n";
    cout << "Connecting... " << connect(socketClient, &socketAddr, INET_ADDRSTRLEN) << endl;
    char buffer[32];
    int bytesReaded;
    auto start = chrono::system_clock::now();
    while ( chrono::system_clock::now() < start + chrono::seconds(3)) {
        bytesReaded = recv(socketClient, buffer, 32, 0);
        string timeStamp(buffer, bytesReaded);
        cout << "Client recived  " << bytesReaded << " bytes, buffer: " << timeStamp;
        // this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "Client closing socket, " << close(socketClient) << endl;
}

void SimpleClient::runLocalServer()
{
    cout << "Starting local server and getting system configuration.\n";
    char addrString[64];
    map<int, string> addrType = { {AF_INET, "IPv4"}, {AF_INET6, "IPv6"} };
    hostent* hostEntry;
    cout << "List of entries at net configuration, or empty if error or nothing to show.\n";
    while ( (hostEntry = gethostent()) !=  NULL ) {
        cout << "Host name: " << hostEntry->h_name;
        // inet_ntop(AF_INET, *hostEntry->h_aliases, addrStr, INET_ADDRSTRLEN);
        cout << ", aliases: " << hostEntry->h_aliases;
        cout << ", type: " << addrType[hostEntry->h_addrtype];
        cout << ", address length in bytes: " << hostEntry->h_length;
        cout << ", network adresses: ";
        for (char** aPtr = hostEntry->h_addr_list; *aPtr != nullptr; aPtr++) {
            inet_ntop(AF_INET, *aPtr, addrString, INET_ADDRSTRLEN);
            cout << addrString << " ";
        }
        cout << endl;
    }
    cout << "Network configuration or empty list if something goes wrong.\n";
    netent *netEntry;
    while ( (netEntry = getnetent()) != nullptr ) {
        cout << "Network: " << netEntry->n_name;
        // inet_ntop(AF_INET, *netEntry->n_aliases, addrStr, INET_ADDRSTRLEN);
        cout << ", aliases: " << netEntry->n_aliases;
        cout << ", type: " << addrType[netEntry->n_addrtype];
        cout << ", net id: " << netEntry->n_net;
        cout << endl;
    }
    /*
    cout << "All services available or empty if error(/etc/services) by default.\n";
    servent *serviceEntry;
    while ( (serviceEntry = getservent()) != nullptr ) {
        cout << "Service: " << serviceEntry->s_name;
        cout << ", aliases: " << serviceEntry->s_aliases;
        cout << ", port: " << serviceEntry->s_port;
        cout << ", protocol: " << serviceEntry->s_proto;
        cout << endl;
    }
    */
    cout << "Addreses information list: \n";
    addrinfo *aiList, *aiPtr, hint;
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = 0;
    hint.ai_socktype = 0;
    hint.ai_protocol = 0;
    hint.ai_addrlen = 0;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    int r;
    if ((r = getaddrinfo("localhost", NULL, &hint, &aiList)) != 0)
        cout << "Error calling getaddrinfo " << gai_strerror(r) << endl;
    map<int, string> flagNames = { {0, "0"}, {AI_PASSIVE, "passive"}, {AI_CANONNAME, "canonical"},
                                   {AI_NUMERICHOST, "numeric host"}, {AI_NUMERICSERV, "numeric service"} };
    map<int, string> familyNames = { {AF_INET, "IPv4"}, {AF_INET6, "IPv6"}, {AF_UNIX, "unix"}, {AF_UNSPEC, "unspecified"} };
    map<int, string> sockTypeNames = { {SOCK_STREAM, "stream"}, {SOCK_DGRAM, "datagram"}, {SOCK_SEQPACKET, "seqpacket"},
                                       {SOCK_RAW, "raw"} };
    map<int, string> protocolNames = { {0, "default"}, {IPPROTO_TCP, "TCP"}, {IPPROTO_UDP, "UDP"}, {IPPROTO_RAW, "RAW"} };
    for (aiPtr = aiList; aiPtr != NULL; aiPtr = aiPtr->ai_next) {
        cout << "Flags: " << flagNames[aiPtr->ai_flags];
        cout << ", family: " << familyNames[aiPtr->ai_family];
        if ( aiPtr->ai_family == AF_INET ) {
            sockaddr_in *sockIn = (sockaddr_in*)aiPtr->ai_addr;
            const char* addr = inet_ntop(AF_INET, &sockIn->sin_addr, addrString, INET_ADDRSTRLEN);
            cout << ":" << addrString << ", port:" << ntohs(sockIn->sin_port);
        }
        cout << ", socket type: " << sockTypeNames[aiPtr->ai_socktype];
        cout << ", protocol: " << protocolNames[aiPtr->ai_protocol];
        cout << endl;
    }
    int serverD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr *socketServer = aiList->ai_addr;
    sockaddr_in *sPtr = (sockaddr_in *)socketServer;
    const char* addr = inet_ntop(AF_INET, &sPtr->sin_addr, addrString, INET_ADDRSTRLEN);
    sPtr->sin_port = 50000;
    cout << "Size of 'sockadr_in' structure is " << sizeof(socketServer) << " bytes." << endl;
    int res = bind(serverD, socketServer, aiList->ai_addrlen);
    cout << "Socket " << serverD << ", address: " << addrString << ", port:" << sPtr->sin_port;
    if (!res) cout << ", binding socket ok." << endl; else
        cout << ", error binding socket." << endl;
    socklen_t defaultLen = INET_ADDRSTRLEN;
    addr = inet_ntop(AF_INET, &sPtr->sin_addr, addrString, INET_ADDRSTRLEN);
    getsockname(serverD, socketServer, &defaultLen);
    cout << "Getting socket name, actual address length " << defaultLen << endl;
    cout << "Server starts listen, ";
    if (listen(serverD, 1024) == -1) {
        cout << "error.\n";
        return;
    }
    cout << "ok.\n";
    int sD = accept(serverD, NULL, NULL);
    if ( sD < 0 ) {
        cout << "Something wrong with accept connection from client, exiting..." << endl;
        close(sD);
        return;
    }
    int bytesSend = 0;
    while ( serverShots < serverShotsMax || bytesSend == -1 ) {
        time_t stamp = chrono::system_clock::to_time_t(chrono::system_clock::now());
        string timeStamp = ctime(&stamp);
        bytesSend = send(sD, timeStamp.data(), timeStamp.size(), 0);
        cout << "Local server: " << bytesSend << " bytes sent " << endl;
        this_thread::sleep_for(chrono::seconds(1));
        serverShots++;
    }
    cout << "Server closing connection with client, " << close(sD) << endl;
    cout << "Stopping local server, " << close(serverD) << endl;
}
