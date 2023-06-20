#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define MESSAGE_LENGTH 1024 // ������������ ������ ������ ��� ������
#define PORT 7777 // ����� ������������ ���� ����� �����

SOCKET socket_file_descriptor;
struct sockaddr_in serveraddress, client;
char message[MESSAGE_LENGTH];

// ������� ��������� ���������� � ��������
void HandleClientConnection(void* client_socket) {
    SOCKET connection = *((SOCKET*)client_socket);
    char message[MESSAGE_LENGTH];

    while (1) {
        memset(message, 0, sizeof(message));

        // ���� ��������� �� �������
        int bytes = recv(connection, message, sizeof(message), 0);
        if (bytes == SOCKET_ERROR) {
            cout << "������ ��� ��������� ��������� �� �������!" << endl;
            cout << "    " << endl;
            break;
        }
        else if (bytes == 0) {
            // ������ ������ ����������
            cout << "������ ����������." << endl;
            cout << "    " << endl;
            break;
        }

        cout << "�������� ������ �� �������: " << message << endl;
        cout << "    " << endl;

        // ���������� ����� �������
        if (send(connection, message, strlen(message), 0) == SOCKET_ERROR) {
            cout << "������ �������� ��������� �������!" << endl;
            cout << "    " << endl;
            break;
        }
    }

    // ��������� ���������� � ��������
    closesocket(connection);

    _endthread();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_Ru");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "�� ������� ���������������� Winsock." << endl;
        cout << "    " << endl;
        return 1;
    }

    // �������� �����
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == INVALID_SOCKET) {
        cout << "������ �������� ������!" << endl;
        cout << "    " << endl;
        WSACleanup();
        return 1;
    }

    // ��������� ����� �������
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    // ������� ����� �����
    serveraddress.sin_port = htons(PORT);
    // ���������� IPv4
    serveraddress.sin_family = AF_INET;

    // �������� ����� � ������ � �����
    if (bind(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) == SOCKET_ERROR) {
        cout << "������ �������� ������!" << endl;
        cout << WSACleanup();
        return 1;
    }

    // ��������� ����� � ����� �������������
    if (listen(socket_file_descriptor, SOMAXCONN) == SOCKET_ERROR) {
        cout << "������ ��������� ������ � ����� �������������!" << endl;
        cout << "    " << endl;
        closesocket(socket_file_descriptor);
        WSACleanup();
        return 1;
    }

    cout << "������ �������. �������� �����������..." << endl;
    cout << "    " << endl;

    while (1) {
        // ��������� �������� ����������
        int clientSize = sizeof(client);
        SOCKET clientSocket = accept(socket_file_descriptor, (struct sockaddr*)&client, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            cout << "������ ��� �������� ��������� ����������!" << endl;
            cout << "    " << endl;
            closesocket(socket_file_descriptor);
            WSACleanup();
            return 1;
        }

        cout << "������ ���������." << endl;
        cout << "    " << endl;

        // ������� ����� ����� ��� ��������� ���������� � ��������
        _beginthread(HandleClientConnection, 0, (void*)&clientSocket);
    }

    // ��������� ����� �������
    closesocket(socket_file_descriptor);

    // ����������� ������� Winsock
    WSACleanup();

    return 0;
}

