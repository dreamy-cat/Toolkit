#include "simple_client.hpp"

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << std::endl;
    std::future<void> localServer(async(std::launch::async, SimpleClient::runLocalServer, 5));
    SimpleClient client;
    client.runExperiment();
    localServer.wait();
    return 0;
}
