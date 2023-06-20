#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта

SOCKET socket_file_descriptor;
struct sockaddr_in serveraddress, client;
char message[MESSAGE_LENGTH];

// Функция обработки соединения с клиентом
void HandleClientConnection(void* client_socket) {
    SOCKET connection = *((SOCKET*)client_socket);
    char message[MESSAGE_LENGTH];

    while (1) {
        memset(message, 0, sizeof(message));

        // Ждем сообщение от клиента
        int bytes = recv(connection, message, sizeof(message), 0);
        if (bytes == SOCKET_ERROR) {
            cout << "Ошибка при получении сообщения от клиента!" << endl;
            cout << "    " << endl;
            break;
        }
        else if (bytes == 0) {
            // Клиент закрыл соединение
            cout << "Клиент отключился." << endl;
            cout << "    " << endl;
            break;
        }

        cout << "Получены данные от клиента: " << message << endl;
        cout << "    " << endl;

        // Отправляем ответ клиенту
        if (send(connection, message, strlen(message), 0) == SOCKET_ERROR) {
            cout << "Ошибка отправки сообщения клиенту!" << endl;
            cout << "    " << endl;
            break;
        }
    }

    // Закрываем соединение с клиентом
    closesocket(connection);

    _endthread();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_Ru");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Не удалось инициализировать Winsock." << endl;
        cout << "    " << endl;
        return 1;
    }

    // Создадим сокет
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == INVALID_SOCKET) {
        cout << "Ошибка создания сокета!" << endl;
        cout << "    " << endl;
        WSACleanup();
        return 1;
    }

    // Установим адрес сервера
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    // Зададим номер порта
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;

    // Привяжем сокет к адресу и порту
    if (bind(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) == SOCKET_ERROR) {
        cout << "Ошибка привязки сокета!" << endl;
        cout << WSACleanup();
        return 1;
    }

    // Установим сокет в режим прослушивания
    if (listen(socket_file_descriptor, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Ошибка установки сокета в режим прослушивания!" << endl;
        cout << "    " << endl;
        closesocket(socket_file_descriptor);
        WSACleanup();
        return 1;
    }

    cout << "Сервер запущен. Ожидание подключений..." << endl;
    cout << "    " << endl;

    while (1) {
        // Принимаем входящее соединение
        int clientSize = sizeof(client);
        SOCKET clientSocket = accept(socket_file_descriptor, (struct sockaddr*)&client, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            cout << "Ошибка при принятии входящего соединения!" << endl;
            cout << "    " << endl;
            closesocket(socket_file_descriptor);
            WSACleanup();
            return 1;
        }

        cout << "Клиент подключен." << endl;
        cout << "    " << endl;

        // Создаем новый поток для обработки соединения с клиентом
        _beginthread(HandleClientConnection, 0, (void*)&clientSocket);
    }

    // Закрываем сокет сервера
    closesocket(socket_file_descriptor);

    // Освобождаем ресурсы Winsock
    WSACleanup();

    return 0;
}

