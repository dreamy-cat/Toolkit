#include "simple_client.hpp"

using namespace std;

int SimpleClient::debugServer = 0;

int SimpleClient::totalClients = 0;

sockaddr SimpleClient::server;

map<int, string> SimpleClient::addrType;

SimpleClient::SimpleClient()
{
    cout << "Creating simple client, id = " << totalClients++ << endl;

    // runDebugServer();
    /*
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
*/
}

void SimpleClient::runDebugServer()
{
    cout << "Starting server and getting system configuration.\n";
    char addrString[64];
    hostent* hostEntry;
    cout << "List of entries at net configuration, or empty if error or nothing to show.\n";
    addrType = { {AF_INET, "IPv4"}, {AF_INET6, "IPv6"} };
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
    cout << "All services available or empty if error(/etc/services) by default.\n";
    servent *serviceEntry;
    while ( (serviceEntry = getservent()) != nullptr ) {
        cout << "Service: " << serviceEntry->s_name;
        cout << ", aliases: " << serviceEntry->s_aliases;
        cout << ", port: " << serviceEntry->s_port;
        cout << ", protocol: " << serviceEntry->s_proto;
        cout << endl;
    }
    cout << "Addrinfo list: \n";
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

    sockaddr_in soc;
    cout << "Size of 'sockadr_in' structure is " << sizeof(soc) << " bytes." << endl;

/*
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
    */
}

void SimpleClient::stopDebugServer()
{
    cout << "Stopping debug server " << close(debugServer) << endl;
}
