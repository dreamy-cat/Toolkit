#include "simple_client.hpp"

int main(int argc, char *argv[])
{
    std::future<void> localServer(async(std::launch::async, SimpleClient::runLocalServer));
    SimpleClient client;
    localServer.wait();
    return 0;
}
