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
    runDebugServer();
}

void SimpleClient::runDebugServer()
{
    /*
    server.sa_family = AF_INET;
    debugServer = socket(server.sa_family, SOCK_STREAM, 0);
    cout << "Starting debug server, " << debugServer << endl;
    // cout << "Server info"
    totalClients++;
    cout << "Bind socket, " << bind(debugServer, &server, sizeof(server)) << endl;
    /*
    for (int i = 0; i < 12; i++) {
        cout << (int)server.sa_data[i] << endl;
    }

    cout << server.sa_data << sizeof(server) << endl;
    cout << "Server starting listen, " << listen(debugServer, 1024) << endl;
*/
    // auto serverF = async(std::launch::async, sendingDebugData);

    char addrStr[64];
    addrinfo addrHints, *addrResult, *addrInfo;
    memset(&addrHints, 0, sizeof( addrinfo));
    addrHints.ai_family = AF_UNSPEC;
    addrHints.ai_socktype = SOCK_STREAM;
    addrHints.ai_flags = AI_PASSIVE;
    addrHints.ai_protocol = 0;
    addrHints.ai_canonname = NULL;
    addrHints.ai_addr = NULL;
    addrHints.ai_next = NULL;
    char serv[] = "50000";
    int s = getaddrinfo(NULL, serv, &addrHints, &addrResult);
    if ( s == -1) cout << "Error getaddrinfo(), " << gai_strerror(s) << endl;
    cout << "getaddrinfo, " << s << endl;
    for (addrInfo = addrResult; addrInfo != NULL; addrInfo = addrInfo->ai_next) {
        // cout << "Name: " << addrInfo->ai_canonname << endl;

        cout << "Family addrinfo " << addrInfo->ai_family << endl;

        if ( addrInfo->ai_family == AF_INET) cout << "INET" << endl;
        if ( addrInfo->ai_family == AF_INET6) cout << "INET6" << endl;
        cout << "Protocol, ";
        if ( addrInfo->ai_protocol == IPPROTO_TCP) cout << "TCP" << endl;
        cout << "Socket type, ";
        if (addrInfo->ai_socktype == SOCK_STREAM) cout << " stream" << endl;

        cout << "Address data: ";
        for (int i = 0; i < 8; ++i) cout << (int)addrInfo->ai_addr->sa_data[i] << " ";
        cout << endl;

        sockaddr_in *sockIn = (sockaddr_in*) addrInfo->ai_addr;
        const char *iPtr;
        iPtr = inet_ntop(AF_INET, &sockIn->sin_addr, addrStr, INET_ADDRSTRLEN);
        cout << "Adress from addrInfo is " << addrStr << endl;
    }
    int socFD;
    for (addrInfo = addrResult; addrInfo != NULL; addrInfo = addrInfo->ai_next) {
        socFD = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
        if (socFD == -1)
            continue;
        if (bind(socFD, addrInfo->ai_addr, addrInfo->ai_addrlen) == 0) {
            cout << "Success." << endl;
            break;
        }
        close(socFD);
    }
    if (addrInfo == NULL) cout << "Could not bind address to socket.\n" << endl;
    // Testing loop...
    sockaddr_storage peerPtr;
    socklen_t peerAddrLen;
    char buffer[1024];
    for (;;) {
        peerAddrLen = sizeof(struct sockaddr_storage);
        int actual = recvfrom(socFD, buffer, 1024, 0, (struct sockaddr *) &peerPtr, &peerAddrLen);
        if (actual == -1) {
            // cout << "Attempt failed to connect failed." << endl;
            continue;
        }
        char host[NI_MAXHOST], service[NI_MAXSERV];
        int r = getnameinfo((struct sockaddr *) &peerPtr, peerAddrLen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
        if (r == 0)
            cout << "Recived "  << actual << " bytes from " << host << ":" << service << endl;
        else
            cout << "Error, getnameinfo: " << gai_strerror(r);
        if (sendto(socFD, buffer, actual, 0, (struct sockaddr *) &peerPtr, peerAddrLen) != actual)
            cout << "Error sending response to client." << endl;
    }
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
