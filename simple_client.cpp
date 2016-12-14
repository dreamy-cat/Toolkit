#include "simple_client.hpp"

using namespace std;

int SimpleClient::debugServer = 0;

int SimpleClient::totalClients = 0;

sockaddr SimpleClient::server;

SimpleClient::SimpleClient(string newName) : name(newName)
{
    cout << "Creating simple client, id = " << name << endl;
    // testing...
    unsigned char addr[4] = { 192, 168, 0, 1 };
    char addrStr[64];
    inet_ntop(AF_INET, addr, addrStr, INET_ADDRSTRLEN);
    cout << addrStr << endl;
    hostent* entry = gethostent();
    if ( entry ) cout << "Gethost ok, entry " << entry->h_name << endl;
    netent* ent1 = getnetent();
    if ( ent1 ) {
        auto netAddr = ent1->n_net;
        inet_ntop(AF_INET, &netAddr, addrStr, INET_ADDRSTRLEN);
        cout << "Get net info, " << ent1->n_name << ", address " << addrStr << endl;
    }
    addrinfo addrHints, *addrResult, *addrInfo;
    addrHints.ai_family = AF_INET;
    addrHints.ai_socktype = SOCK_STREAM;
    addrHints.ai_flags = AI_PASSIVE;
    addrHints.ai_protocol = 0;
    addrHints.ai_canonname = NULL;
    addrHints.ai_addr = NULL;
    addrHints.ai_next = NULL;
    char serv[] = "50000";
    int s = getaddrinfo(NULL, serv, &addrHints, &addrResult);
    cout << "getaddrinfo, " << s << endl;
    for (addrInfo = addrResult; addrInfo != NULL; addrInfo = addrInfo->ai_next) {
        // cout << "Name: " << addrInfo->ai_canonname << endl;
        cout << "Fam" << addrInfo->ai_family << endl;
    }
}

void SimpleClient::runDebugServer()
{
    server.sa_family = AF_INET;
    debugServer = socket(server.sa_family, SOCK_STREAM, 0);
    cout << "Starting debug server, " << debugServer << endl;
    totalClients++;
    cout << "Bind socket, " << bind(debugServer, &server, sizeof(server)) << endl;
    /*
    for (int i = 0; i < 12; i++) {
        cout << (int)server.sa_data[i] << endl;
    }
    */
    cout << server.sa_data << sizeof(server) << endl;
    cout << "Listen, " << listen(debugServer, 1024) << endl;
    // auto serverF = async(std::launch::async, sendingDebugData);


}

void SimpleClient::stopDebugServer()
{
    cout << "Stopping debug server " << close(debugServer) << endl;
    totalClients--;
}

void SimpleClient::sendingDebugData()
{
    int client;
    char buf[1024] = "abcdefgh";
    /*
        client = accept(debugServer, NULL, NULL );
        send(client, &buf, 5, 0);
        cout << "Data has sent..." << endl;
    */
}
