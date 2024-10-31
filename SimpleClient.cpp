#include "SimpleClient.hpp"

/*

using namespace std;

SimpleClient::SimpleClient(int argc, char *argv[])
{
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Creating simple client." << endl;
    cout << "Command line parameters to settings.\n";
    for (int i = 1; i < argc - 1 ; i += 2)
        settings[argv[i]] = argv[i+1];
    string serverAddress("127.0.0.1:50000");
    int serverPort = 50000;
    if (settings.find("--server") != settings.end()) {
        serverAddress = settings.at("--server");
        string ipPort(serverAddress, serverAddress.find(':') + 1, 5);
        serverPort = atoi(ipPort.data());
        string ip(serverAddress, 0, serverAddress.find(':'));
        serverAddress = ip;
    } else
        cout << "Key '--server' not found, using default 127.0.0.1:50000.\n";
    if (settings.find("--size") != settings.end())
        defaultMSize = atoi(settings.at("--size").data());
    else {
        cout << "Key '--size' not found, using default size 3.\n";
        defaultMSize = 3;
    }
    if (settings.find("--threads") != settings.end())
        defaultThreads = atoi(settings.at("--threads").data());
    else {
        cout << "Key '--threads' not found, only single thread will be using.\n";
        defaultThreads = 1;
    }
    const int timeStampSize = 32;
    char *buffer = new char[timeStampSize];
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient < 0) {
        cout << "Can't open socket for client." << endl;
        return;
    }
    sockaddr socketAddr;
    sockaddr_in *sPtr = (sockaddr_in*)&socketAddr;
    sPtr->sin_family = AF_INET;
    sPtr->sin_port = serverPort;
    if ( inet_pton(AF_INET, serverAddress.data(), (void*)&sPtr->sin_addr) == -1 ) {
        cout << "Client, error in address." << endl;
        return;
    }
    cout << "Client creating socket ok.\n";

    if ( connect(socketClient, &socketAddr, INET_ADDRSTRLEN) ) {
        cout << "Client can't connect to the server " << serverAddress << ":" << serverPort << endl;
        return;
    }
    cout << "Client connecting to server " << serverAddress << ":" << serverPort << endl;
    int bytesReaded;
    auto start = chrono::system_clock::now();
    while ( chrono::system_clock::now() < start + chrono::seconds(3)) {
        bytesReaded = recv(socketClient, buffer, timeStampSize, 0);
        string timeStamp(buffer, bytesReaded);
        cout << "Client recived " << bytesReaded << " bytes, " << "buffer: " << timeStamp;
    }

    cout << "Client closing socket, " << close(socketClient) << endl;
}

void SimpleClient::runExperiment()
{
    int mSize, totalElements, activeThreads;
    mSize = defaultMSize;
    activeThreads = defaultThreads;
    totalElements = mSize * mSize;
    cout << "Run a simple task to find invertible matrix.\n"
         << "Random square matrix with size " << mSize
         << ", total float elements " << totalElements
         << ", using " << activeThreads << " threads.\n";
    if (mSize <= 0 || mSize > 0x8000 || activeThreads <= 0 || activeThreads > 0x100 || mSize % activeThreads != 0 ) {
        cout << "Matrix size must be in [1..0x8000].\n"
             << "Threads must be in [1..0x100] and matrix size must be divided by threads without remainder." << endl;
        return;
    }
    vector<float> am, bm, im;
    am.reserve(totalElements);
    bm.reserve(totalElements);
    im.reserve(totalElements);
    srand(0);
    for (int i = 0; i < mSize; ++i) {
        for (int j = 0; j < mSize; ++j) {
            am[i*mSize + j] = float(rand() % 10);
            cout << am[i * mSize + j] << " ";
            bm[i*mSize + j] = am[i * mSize + j];
            if (i == j) im[i*mSize + j] = 1; else im[i * mSize + j] = 0;
        }
        cout << endl;
    }
    cout << "Simple matrix A and I:\n" << endl;
    vector< future<void> > localTasks;
    for (int k = 0; k < mSize; ++k) {
        localTasks.clear();
        float mult = am[k * mSize + k];
        int perThread = mSize / activeThreads;
//        cout << "Elements per thread in line " << perThread << endl;
        for (int i = 0; i < activeThreads; i++) {
            localTasks.push_back(async(launch::async, [mSize, mult, &am, &im, perThread, i, k]() {
                for (int j = i * perThread; j < (i+1) * perThread; ++j) {
//                    cout << "i = " << i << " j = " << j << endl;
                    am[k*mSize + j] /= mult;
                    im[k*mSize + j] /= mult;
                }
            }));
        }
        for (auto& e : localTasks) e.get();
        //cout << "Iteration " << k << ":\n";
        //outputMatrix(am, im);
        localTasks.clear();
        for (int t = 0; t < activeThreads; t++) {
            localTasks.push_back(async(launch::async, [mSize, &am, &im, perThread, t, k]() {
                for (int i = t * perThread; i < (t + 1) * perThread; i++) {
                    if ( i != k) {
                        float multiplier = am[i*mSize + k];
                        // cout << "Multilpler " << multiplier << endl;
                        for (int j = 0; j < mSize; ++j) {
                            am[i*mSize + j] = am[i*mSize + j] - am[k*mSize + j] * multiplier;
                            im[i*mSize + j] = im[i*mSize + j] - im[k*mSize + j] * multiplier;
                        }
                    }
                }
            }));
        }
        // cout << "Iteration " << k << ":\n";
        // outputMatrix(am, im);
    }

    cout << "Result of matrix:\n";
//    outputMatrix(am, im);
    for (auto& e : localTasks) e.get();
    localTasks.clear();
    vector<float> rm(totalElements, 0);
    int checkSum = 0;
    cout << "Quick check.\n";
    int perThread = mSize / activeThreads;
    for (int t = 0; t < activeThreads; t++) {
        for (int i = t * perThread; i < (t + 1) * perThread; i++) {
            localTasks.push_back(async(launch::async, [mSize, &rm, &bm, &im, i, &checkSum]() {
                for (int j = 0; j < mSize; j++) {
                    rm[i*mSize + j] = 0;
                    for (int k = 0; k < mSize; k++) {
                        rm[i*mSize + j] += bm[i*mSize + k] * im[k*mSize + j];
                    }
                    if (abs(rm[i*mSize + j]) >= 0.5) checkSum++;   // think later.
                    // cout << rm[i][j] << "\t";
                }
            }));
        }
    }
    for (auto &e : localTasks) e.get();

    // cout << "\n";
    if ( checkSum == mSize ) cout << "Check ok, sum " << checkSum << " equal size of matrix " << mSize << endl; else
        cout << "Something goes wrong, sum " << checkSum << " differs from size of matrix " << mSize << endl;
}

void SimpleClient::connectionClosed(int)
{
    cout << "Signal handler called, connection closed by remote client." << endl;
}

void SimpleClient::runLocalServer(int serverShots)
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
    int bytesSend = 0, shots = 0;
    while ( shots < serverShots && bytesSend != -1 ) {
        time_t stamp = chrono::system_clock::to_time_t(chrono::system_clock::now());
        string timeStamp = ctime(&stamp);
        bytesSend = send(sD, timeStamp.data(), timeStamp.size(), 0);
        cout << "Local server: " << bytesSend << " bytes sent, total shots " << ++shots << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "Server closing connection with client, " << close(sD) << endl;
    cout << "Stopping local server, " << close(serverD) << endl;
}

*/

