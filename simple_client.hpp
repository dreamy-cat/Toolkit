#ifndef SIMPLE_CLIENT_HPP
#define SIMPLE_CLIENT_HPP

#include <iostream>
#include <string>
#include <future>
#include <map>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>

class SimpleClient {
public:
    explicit SimpleClient();
    ~SimpleClient() = default;
    void request();
    static void runDebugServer();
    static void stopDebugServer();
protected:
private:
    SimpleClient(const SimpleClient&);
    SimpleClient& operator=(const SimpleClient&);
    SimpleClient(SimpleClient&&);

    static int debugServer, totalClients;
    static sockaddr server;
    // static std::map<int, std::string> addrType;
};

#endif
