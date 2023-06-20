#include <iostream>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_Ru");

    WSADATA wsaData;
    SOCKET socket_file_descriptor, connection;
    struct sockaddr_in serveraddress, client;
    char message[MESSAGE_LENGTH];

    // Инициализация библиотеки Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed!" << endl;
        cout << "Ошибка инициализации WSAStartup!" << endl;
        cout << "    " << endl;
        return 1;
    }

    // Создадим сокет
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == INVALID_SOCKET) {
        cout << "Creation of Socket failed!" << endl;
        cout << "Ошибка создания сокета!" << endl;
        cout << "    " << endl;
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
        cout << "Binding failed!" << endl;
        cout << "Ошибка привязки сокета!" << endl;
        cout << "    " << endl;
        closesocket(socket_file_descriptor);
        WSACleanup();
        return 1;
    }

    // Начнем прослушивание соединений
    if (listen(socket_file_descriptor, 5) == SOCKET_ERROR) {
        cout << "Listening failed!" << endl;
        cout << "Ошибка прослушивания соединений!" << endl;
        cout << "    " << endl;
        closesocket(socket_file_descriptor);
        WSACleanup();
        return 1;
    }

    cout << "Server started. Waiting for connections..." << endl;
    cout << "Сервер запущен. Ожидание подключений..." << endl;
    cout << "    " << endl;

    while (1) {
        // Принимаем новое соединение
        int client_address_size = sizeof(client);
        connection = accept(socket_file_descriptor, (struct sockaddr*)&client, &client_address_size);
        if (connection == INVALID_SOCKET) {
            cout << "Failed to accept connection!" << endl;
            cout << "Не удалось принять соединение!" << endl;
            cout << "    " << endl;
            closesocket(socket_file_descriptor);
            WSACleanup();
            return 1;
        }

        cout << "Client connected. Handling connection..." << endl;
        cout << "Клиент подключен. Обработка соединения..." << endl;
        cout << "    " << endl;

        // Взаимодействие с клиентом
        while (1) {
            memset(message, 0, sizeof(message));

            // Ждем сообщение от клиента
            int bytes = recv(connection, message, sizeof(message), 0);
            if (bytes == SOCKET_ERROR) {
                cout << "Error receiving message from client!" << endl;
                cout << "Ошибка при получении сообщения от клиента!" << endl;
                cout << "    " << endl;
                break;
            }
            else if (bytes == 0) {
                // Клиент закрыл соединение
                cout << "Client disconnected." << endl;
                cout << "Клиент отключился." << endl;
                cout << "    " << endl;
                break;
            }

            cout << "Data received from client: " << message << endl;
            cout << "Получены данные от клиента: " << message << endl;
            cout << "    " << endl;

            // Отправляем ответ клиенту
            if (send(connection, message, strlen(message), 0) == SOCKET_ERROR) {
                cout << "Error sending message to client!" << endl;
                cout << "Ошибка отправки сообщения клиенту!" << endl;
                cout << "    " << endl;
                break;
            }
        }

        // Закрываем соединение с клиентом
        closesocket(connection);
    }

    // Закрываем сокет сервера
    closesocket(socket_file_descriptor);

    // Выгружаем библиотеку Winsock
    WSACleanup();

    return 0;
}
