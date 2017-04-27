#include "simple_client.hpp"

using namespace std;

SimpleClient::SimpleClient()
{
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Creating simple client." << endl;
    const char serverIP[] = "127.0.0.1";
    int serverPort = 50000;
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient < 0) {
        cout << "Can't open socket for client." << endl;
        return;
    }
    sockaddr socketAddr;
    sockaddr_in *sPtr = (sockaddr_in*)&socketAddr;
    sPtr->sin_family = AF_INET;
    sPtr->sin_port = serverPort;
    if ( inet_pton(AF_INET, serverIP, (void*)&sPtr->sin_addr) == -1 ) {
        cout << "Client, error in address." << endl;
        return;
    }
    cout << "Client creating socket ok.\n";
    if ( connect(socketClient, &socketAddr, INET_ADDRSTRLEN) ) {
        cout << "Client can't connect to the server " << serverIP << ":" << serverPort << endl;
        return;
    }
    cout << "Client connecting to server " << serverIP << ":" << serverPort << endl;
    char buffer[0x10000];
    int bytesReaded;
    auto start = chrono::system_clock::now();
//    while ( chrono::system_clock::now() < start + chrono::seconds(1)) {
        bytesReaded = recv(socketClient, buffer, 0x10000, 0);
        string timeStamp(buffer, bytesReaded);
        cout << "Client recived " << bytesReaded << " bytes." << endl; // buffer: " << timeStamp;
        // this_thread::sleep_for(chrono::seconds(1));
//    }
    cout << "Client closing socket, " << close(socketClient) << endl;
    vector<float> received(4096, 0);
    for (int i = 0; i < 4096*sizeof(float); i++)
        ((char*)received.data())[i] = buffer[i];
    cout << "Vector: ";
    int sum = 0;
    for (auto &element : received) {
        // cout << element;
        sum += element;
    }
    cout << "\nSum = " << sum << endl;
    cout << "Quick try with many threads:\n";
    for (int i = 0; i < 8; i++) {
        async(launch::async, [i, received]() {
            cout << "Thread "  << i << ", sum: ";
            int sum = 0;
            for (int j = i * 512; j < (i+1)*512; j++)
                sum += received[j];
            cout << sum << endl;
        });
    }
    // Fast testing...
    const int mSize = 4;
    float am[mSize][mSize] = { 2, 1, 0, 0, 3, 2, 0, 0, 1, 1, 3, 4, 2, -1, 2, 3 };
    float bm[mSize][mSize] = { 2, 1, 0, 0, 3, 2, 0, 0, 1, 1, 3, 4, 2, -1, 2, 3 };
    float im[mSize][mSize];
    for (int i = 0; i < mSize*mSize; ++i) {
        // am[i / mSize][i % mSize] = (i+1)*2;
        im[i / mSize][i % mSize] = (i / mSize == i % mSize);
    }
    cout << "Simple matrix A and I:\n";
    outputMatrix(am, im);
    vector< future<void> > localTasks;
    for (int k = 0; k < mSize; ++k) {
        localTasks.clear();
        float mult = am[k][k];
        int perThread = mSize / activeThreads;
        cout << "Elements per thread in line " << perThread << ", remains " << mSize % activeThreads << endl;
        for (int i = 0; i < activeThreads; i++) {
            localTasks.push_back(async(launch::async, [mult, &am, &im, perThread, i, k]() {
                for (int j = i * perThread; j < (i+1) * perThread; ++j) {
                    cout << "i = " << i << " j = " << j << endl;
                    am[k][j] /= mult;
                    im[k][j] /= mult;
                }
            }));
        }
        for (int j = perThread * activeThreads; j < mSize; j++) {
            am[k][j] /= mult;
            im[k][j] /= mult;
        }
        for (auto &e : localTasks) e.get();
        cout << "Iteration " << k << ":\n";
        outputMatrix(am, im);
        auto lower = async(launch::async, [k, &am, &im]() {
            for (int i = k + 1; i < mSize; ++i) {
                float multLower = am[i][k];
                cout << "Lower, multiplicator " << multLower << endl;
                for (int j = 0; j < mSize; ++j) {
                    am[i][j] = am[i][j] - am[k][j] * multLower;
                    im[i][j] = im[i][j] - im[k][j] * multLower;
                }
            }

        });

/*
        cout << "Iteration " << k << ":\n";
        outputMatrix(am, im);
*/
        auto upper = async(launch::async, [k, &am, &im]() {
            for (int i = k - 1; i >= 0; --i) {
                float multUpper= am[i][k];
                cout << "Upper, multiplicator " << multUpper << "\n";
                cout << "i " << i << " k " << k << endl;
                for (int j = 0 ; j < mSize; ++j) {
                    am[i][j] = am[i][j] - am[k][j] * multUpper;
                    im[i][j] = im[i][j] - im[k][j] * multUpper;
                }
            }
        });

        lower.get();
        upper.get();

        cout << "Iteration " << k << ":\n";
        outputMatrix(am, im);
    }
    // Quick check result.
    float rm[mSize][mSize];
    int sumE = 0;
    cout << "Quick check.\n";
    for (int i = 0; i < mSize; i++) {
        for (int j = 0; j < mSize; j++) {
            rm[i][j] = 0;
            for (int k = 0; k < mSize; k++) {
                rm[i][j] += bm[i][k] * im[k][j];
            }
            sumE += trunc(rm[i][j]);
            cout << trunc(rm[i][j]) << "\t";
        }
        cout << "\n";
    }
    cout << "Sum of matirix elements " << sumE << endl;
}

void SimpleClient::outputMatrix(float am[mS][mS], float im[mS][mS])
{
    int mSize = mS;
    for (int i = 0; i < mSize; i++) {
        for (int j = 0; j < mSize; j++)
            cout << am[i][j] << "\t";
        cout << "|\t";
        for (int j = 0; j < mSize; j++)
            cout << im[i][j] << "\t";
        cout << endl;
    }
}

void SimpleClient::connectionClosed(int)
{
    cout << "Signal handler called, connection closed by remote client." << endl;
}

void SimpleClient::runLocalServer(int maxShots)
{
    signal(SIGPIPE, SimpleClient::connectionClosed);
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
    cout << "Getting socket name, default address length in bytes: " << defaultLen << endl;
    if (listen(serverD, 1024) == -1) {
        cout << "Server error listening at port " << sPtr->sin_port << ".\n";
        return;
    }
    cout << "Server starts listen at port " << sPtr->sin_port << ", ok.\n";
    int sD = accept(serverD, NULL, NULL);
    if ( sD < 0 ) {
        cout << "Something wrong with accept connection from client, exiting..." << endl;
        close(sD);
        return;
    }
    int bytesSend = 0, serverShots = 0;
    vector<float> data(4096, 2);
    while ( serverShots < maxShots && bytesSend != -1 ) {
        time_t stamp = chrono::system_clock::to_time_t(chrono::system_clock::now());
        string timeStamp = ctime(&stamp);
        // bytesSend = send(sD, timeStamp.data(), timeStamp.size(), 0);
        cout << "Local server: " << bytesSend << " bytes sent, total shots " << ++serverShots << endl;
        bytesSend = send(sD, data.data(), 4096*sizeof(float), 0);
        cout << "Local server: " << bytesSend << " bytes sent, total shots " << ++serverShots << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "Server closing connection with client, " << close(sD) << endl;
    cout << "Stopping local server, " << close(serverD) << endl;
}
