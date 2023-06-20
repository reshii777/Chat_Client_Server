#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#define MESSAGE_LENGTH 1024 // ������������ ������ ������ ��� ������
#define PORT 7777 // ����� ������������ ���� ����� �����

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
        cout << "�� ������� ���������������� Winsock." << endl;
        cout << "    " << endl;
        return 1;
    }

    // �������� �����
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == INVALID_SOCKET) {
        cout << "Creation of Socket failed!" << endl;
        cout << "������ �������� ������!" << endl;
        cout << "    " << endl;
        WSACleanup();
        return 1;
    }

    // ��������� ����� �������
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // ������� ����� �����
    serveraddress.sin_port = htons(PORT);
    // ���������� IPv4
    serveraddress.sin_family = AF_INET;
    // ��������� ���������� � ��������
    int connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == SOCKET_ERROR) {
        cout << "Connection with the server failed!" << endl;
        cout << "�� ������� ���������� ���������� � ��������!" << endl;
        cout << "    " << endl;
        closesocket(socket_file_descriptor);
        WSACleanup();
        return 1;
    }

    // �������������� � ��������
    while (1) {
        memset(message, 0, sizeof(message));
        cout << "Enter the message you want to send to the server: " << endl;
        cout << "������� ���������, ������� �� ������ ��������� �� ������: " << endl;
        cout << "    " << endl;
        cin >> message;
        if ((strncmp(message, "end", 3)) == 0) {
            send(socket_file_descriptor, message, sizeof(message), 0);
            cout << "Client Exit." << endl;
            cout << "����� �� �������." << endl;
            cout << "    " << endl;
            break;
        }
        int bytes = send(socket_file_descriptor, message, sizeof(message), 0);
        // ���� �������� >= 0  ����, ������ ��������� ������ �������
        if (bytes >= 0) {
            cout << "Data send to the server successfully!" << endl;
            cout << "������ ������� ���������� �� ������!" << endl;
            cout << "    " << endl;
        }
        memset(message, 0, sizeof(message));
        // ���� ������ �� �������
        recv(socket_file_descriptor, message, sizeof(message), 0);
        cout << "Data received from server: " << message << endl;
        cout << "������ �������� �� �������: " << message << endl;
        cout << "    " << endl;
    }

    // ��������� �����, ��������� ����������
    closesocket(socket_file_descriptor);
    WSACleanup();

    return 0;
}
