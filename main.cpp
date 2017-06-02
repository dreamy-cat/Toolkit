#include "simple_client.hpp"

int main(int argc, char *argv[])
{
    std::future<void> localServer(async(std::launch::async, SimpleClient::runLocalServer, 5));
    SimpleClient client(argc, argv);
    client.runExperiment();
    localServer.wait();
    return 0;
}
