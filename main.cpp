#include "SimpleClient.hpp"

// Обновить репозиторий полностью и реализовать исследовательскую модель по управлению UDP трафиком.
// Класс SimpleClient пока оставить или переделать отдельно серверную/клиентскую часть по классам.
// Вынести все вычислительные эксперименты в свой класс и наверное табами оставить просто в интерфейсе.
// Базовое приложение обновить на Qt5 пока что версии, компиляция по минимуму Вин.
// UDP класс также отдельно, вывод по умолчанию в виртуальный лог и подумать над графикой.

void simpleTCPConnection(void)
{   // Remake.
    /*
    std::future<void> localServer(async(std::launch::async, SimpleClient::runLocalServer, 5));
    SimpleClient client(argc, argv);
    client.runExperiment();
    localServer.wait();
    */
}

int main(int argc, char *argv[])
{
    std::cout << "Toolkit with various research projects and exeriments.\n";
    return 0;
}
