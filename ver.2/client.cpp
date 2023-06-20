#include <iostream>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта

SOCKET socket_file_descriptor;
struct sockaddr_in serveraddress;
char message[MESSAGE_LENGTH];

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
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP адрес сервера
    serveraddress.sin_port = htons(PORT); // Номер порта сервера
    serveraddress.sin_family = AF_INET; // Используем IPv4

    // Установим соединение с сервером
    if (connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) == SOCKET_ERROR) {
        cout << "Не удалось установить соединение с сервером!" << endl;
        cout << "    " << endl;
        closesocket(socket_file_descriptor);
        WSACleanup();
        return 1;
    }

    // Взаимодействие с сервером
    while (1) {
        memset(message, 0, sizeof(message));
        cout << "Введите сообщение, которое вы хотите отправить на сервер: ";
        cin.getline(message, sizeof(message));

        if (strcmp(message, "end") == 0) {
            // Отправляем сообщение на сервер
            if (send(socket_file_descriptor, message, strlen(message), 0) == SOCKET_ERROR) {
                cout << "Ошибка отправки сообщения на сервер!" << endl;
                cout << "    " << endl;
                break;
            }

            cout << "Клиент отключен." << endl;
            cout << "    " << endl;
            break;
        }

        // Отправляем сообщение на сервер
        if (send(socket_file_descriptor, message, strlen(message), 0) == SOCKET_ERROR) {
            cout << "Ошибка отправки сообщения на сервер!" << endl;
            cout << "    " << endl;
            break;
        }

        memset(message, 0, sizeof(message));

        // Ждем ответа от сервера
        if (recv(socket_file_descriptor, message, sizeof(message), 0) == SOCKET_ERROR) {
            cout << "Ошибка при получении ответа от сервера!" << endl;
            cout << "    " << endl;
            break;
        }

        cout << "Получены данные от сервера: " << message << endl;
        cout << "    " << endl;
    }

    // Закрываем сокет
    closesocket(socket_file_descriptor);

    // Очищаем Winsock
    WSACleanup();

    return 0;
}