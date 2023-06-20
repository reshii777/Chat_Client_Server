#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта

SOCKET socket_file_descriptor;
struct sockaddr_in serveraddress, client;
char message[MESSAGE_LENGTH];

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_Ru");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Failed to initialize Winsock." << endl;
        cout << "Не удалось инициализировать Winsock." << endl;
        cout << "    " << endl;
        return 1;
    }

    // Создадим сокет
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == INVALID_SOCKET) {
        cout << "Creation of Socket failed!" << endl;
        cout << "Ошибка создания сокета!" << endl;
        cout << "    " << endl;
        WSACleanup();
        return 1;
    }

    // Установим адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Установим соединение с сервером
    int connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == SOCKET_ERROR) {
        cout << "Connection with the server failed!" << endl;
        cout << "Не удалось установить соединение с сервером!" << endl;
        cout << "    " << endl;
        closesocket(socket_file_descriptor);
        WSACleanup();
        return 1;
    }

    // Взаимодействие с сервером
    while (1) {
        memset(message, 0, sizeof(message));
        cout << "Enter the message you want to send to the server: " << endl;
        cout << "Введите сообщение, которое вы хотите отправить на сервер: " << endl;
        cout << "    " << endl;
        cin >> message;
        if ((strncmp(message, "end", 3)) == 0) {
            send(socket_file_descriptor, message, sizeof(message), 0);
            cout << "Client Exit." << endl;
            cout << "Выход из клиента." << endl;
            cout << "    " << endl;
            break;
        }
        int bytes = send(socket_file_descriptor, message, sizeof(message), 0);
        // Если передали >= 0  байт, значит пересылка прошла успешно
        if (bytes >= 0) {
            cout << "Data send to the server successfully!" << endl;
            cout << "Данные успешно отправлены на сервер!" << endl;
            cout << "    " << endl;
        }
        memset(message, 0, sizeof(message));
        // Ждем ответа от сервера
        recv(socket_file_descriptor, message, sizeof(message), 0);
        cout << "Data received from server: " << message << endl;
        cout << "Данные получены от сервера: " << message << endl;
        cout << "    " << endl;
    }

    // закрываем сокет, завершаем соединение
    closesocket(socket_file_descriptor);
    WSACleanup();

    return 0;
}
