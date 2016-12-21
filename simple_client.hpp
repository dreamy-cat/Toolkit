#ifndef SIMPLE_CLIENT_HPP
#define SIMPLE_CLIENT_HPP

#include <iostream>
#include <string>
#include <future>

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
    explicit SimpleClient(std::string newName = "default");
    ~SimpleClient() = default;
    static void runDebugServer();
    static void stopDebugServer();
    static void sendingDebugData();
protected:
private:
    SimpleClient(const SimpleClient&);
    SimpleClient& operator=(const SimpleClient&);
    SimpleClient(SimpleClient&&);

    std::string name;
    static int debugServer, totalClients;
    static sockaddr server;
};

#endif
