#ifndef SIMPLE_CLIENT_HPP
#define SIMPLE_CLIENT_HPP

#include <iostream>
#include <string>
#include <future>
#include <map>
#include <vector>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <signal.h>

class SimpleClient {
public:
    explicit SimpleClient();
    ~SimpleClient() = default;
    void request();
    static void runLocalServer(int maxShots = 5);
    static void connectionClosed(int);
protected:
private:
    SimpleClient(const SimpleClient&);
    SimpleClient& operator=(const SimpleClient&);
    SimpleClient(SimpleClient&&);
};

#endif
