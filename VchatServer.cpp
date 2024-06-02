#include <windows.h>
#include <iostream>
#include <filesystem>
#include <SFML/Network.hpp>
#include <thread>
#include <vector>
#include <mutex>

#include "iniManager/iniManager.h"

std::vector<sf::TcpSocket*> clients;
std::mutex clients_mutex;

void listenForNewConnections(sf::TcpListener& listener)
{
    while (true)
    {
        sf::TcpSocket* client = new sf::TcpSocket;
        if (listener.accept(*client) == sf::Socket::Done)
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            std::cout << "New connection from: " << client->getRemoteAddress() << std::endl;
            clients.push_back(client);
        }
    }
}

void removeDisconnectedClients()
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto it = clients.begin(); it != clients.end();)
    {
        sf::TcpSocket* client = *it;
        if (client->getRemotePort() == 0)
        {
            std::cout << "Client disconnected." << std::endl;
            delete client;
            it = clients.erase(it);
        }
        else
        {
            ++it;
        }
    }
}





int main(int argc, char* argv[])
{
    // initialization file processing. Получаем путь к исполняющему файлу, Получаем директорию исполняющего файла 
    std::filesystem::path executablePath = std::filesystem::absolute(std::filesystem::path(argv[0]));
    std::filesystem::path directory = executablePath.parent_path();

    std::string debug = iniRead(directory, "SETTINGS", "debug");
    std::string strPort = iniRead(directory, "Network", "port");

    sf::TcpListener listener;
    //std::vector<sf::TcpSocket*> clients;

    if (listener.listen(std::stoi(strPort)) != sf::Socket::Done) // прослушиваем новые подключения
    {
        std::cerr << "Failed to open listening port" << std::endl;
        return 1;
    }

    std::cout << "The server is running and waiting for connections..." << std::endl;

    std::thread listenThread(listenForNewConnections, std::ref(listener));
    listenThread.detach();

    while (true)
    {
        removeDisconnectedClients();

        for (size_t i = 0; i < clients.size(); ++i)
        {
            char data[255];
            std::size_t received;
            if (clients[i]->receive(data, sizeof(data), received) == sf::Socket::Done) // в отдельный поток
            {
                std::cout << "Message from " << clients[i]->getRemoteAddress() << ": " << data << std::endl;

                // Broadcast received message to all clients
                for (size_t j = 0; j < clients.size(); ++j)
                {
                    //if (j != i)
                    clients[j]->send(data, received);
                    std::cout << "broadcast to: " << clients[j] << std::endl;
                }
            }
        }
    }

    listener.close();
    return 0;
}
