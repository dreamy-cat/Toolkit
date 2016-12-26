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
    // runDebugServer();
    addrinfo addrHints, *addrResult, *addrInfo;
    const int bufSize = 1024;
    char buf[bufSize];
    memset(&addrHints, 0, sizeof(struct addrinfo));
    addrHints.ai_family = AF_UNSPEC;
    addrHints.ai_socktype = SOCK_DGRAM;
    addrHints.ai_flags = 0;
    addrHints.ai_protocol = 0;
    int soc;
    char host[] = "127.0.0.1", port[] = "50000";
    int r = getaddrinfo(host, port, &addrHints, &addrResult);
    if (r != 0) {
        cout << "Error getting address information." << gai_strerror(r) << endl;
        return;
    }
    for (addrInfo = addrResult; addrInfo != NULL; addrInfo = addrInfo->ai_next) {
        soc = socket(addrInfo->ai_family, addrInfo->ai_socktype,
                     addrInfo->ai_protocol);
        if (soc == -1)
            continue;
        if (connect(soc, addrInfo->ai_addr, addrInfo->ai_addrlen) != -1) {
            cout << "Connecting to server, ok." << endl;
            break;
        }
        close(soc);
    }
    if (addrInfo == NULL) {
        cout << "Could not connect to server." << endl;
        return;
    }
    freeaddrinfo(addrResult);
    const char *messages[] = { "one", "two", "three", "four", "five" };
    for (int i = 0; i < 5; i++) {
        size_t len = strlen(messages[i]) + 1;
        if (len + 1 > bufSize) {
            cout << "Error, message too long." << endl;
            return;
        }
        if (write(soc, messages[i], len) != len) {
            cout << "Error writing data to socket." << endl;
            return;
        }
        ssize_t nread = read(soc, buf, bufSize);
        if (nread == -1) {
            cout << "Error reading response from server." << endl;
            return;
        }
        cout << "Received " << nread << " bytes from server: " << buf << endl;
    }

}

void SimpleClient::runDebugServer()
{
    cout << "Starting server..." << endl;
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

    addrinfo addrHints, *addrResult, *addrInfo;
    const int bufSize = 1024;
    char buf[bufSize];
    memset(&addrHints, 0, sizeof(struct addrinfo));
    addrHints.ai_family = AF_UNSPEC;
    addrHints.ai_socktype = SOCK_DGRAM;
    addrHints.ai_flags = AI_PASSIVE;
    addrHints.ai_protocol = 0;
    addrHints.ai_canonname = NULL;
    addrHints.ai_addr = NULL;
    addrHints.ai_next = NULL;
    char port[] = "50000";
    int soc, r;
    if ((r = getaddrinfo(NULL, port, &addrHints, &addrResult)) != 0) {
        cout << "Error getting address information." << gai_strerror(r) << endl;
        return;
    }
    for (addrInfo = addrResult; addrInfo != NULL; addrInfo = addrInfo->ai_next) {
        soc = socket(addrInfo->ai_family, addrInfo->ai_socktype,
                     addrInfo->ai_protocol);
        if (soc == -1)
            continue;
        if (bind(soc, addrInfo->ai_addr, addrInfo->ai_addrlen) == 0) {
            cout << "Bind socket ok." << endl;
            break;
        }
        close(soc);
    }
    if (addrInfo == NULL) {
        cout << "Could not bind socket." << endl;
        return;
    }
    freeaddrinfo(addrResult);
    sockaddr_storage peerAddr;
    socklen_t peerAddrLen;
    for (;;) {
        peerAddrLen = sizeof(struct sockaddr_storage);
        ssize_t nread = recvfrom(soc, buf, bufSize, 0, (struct sockaddr *) &peerAddr, &peerAddrLen);
        if (nread == -1) {
            cout << "Request failed, waiting next one." << endl;
            continue;
        }
        char host[NI_MAXHOST], service[NI_MAXSERV];
        r = getnameinfo((struct sockaddr *) &peerAddr, peerAddrLen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
        if (r == 0)
            cout << "Received " << nread << " bytes from " << host << ":" << service << endl;
        else
            cout << "Error getting name information: " << gai_strerror(r) << endl;
        if (sendto(soc, buf, nread, 0, (struct sockaddr *) &peerAddr, peerAddrLen) != nread)
            cout << "Error sending response." << endl;
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
