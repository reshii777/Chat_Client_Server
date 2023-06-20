#include <iostream>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define MESSAGE_LENGTH 1024 // ������������ ������ ������ ��� ������
#define PORT 7777 // ����� ������������ ���� ����� �����

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_Ru");

    WSADATA wsaData;
    SOCKET socket_file_descriptor, connection;
    struct sockaddr_in serveraddress, client;
    char message[MESSAGE_LENGTH];

    // ������������� ���������� Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed!" << endl;
        cout << "������ ������������� WSAStartup!" << endl;
        cout << "    " << endl;
        return 1;
    }

    // �������� �����
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == INVALID_SOCKET) {
        cout << "Creation of Socket failed!" << endl;
        cout << "������ �������� ������!" << endl;
        cout << "    " << endl;
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
        cout << "Binding failed!" << endl;
        cout << "������ �������� ������!" << endl;
        cout << "    " << endl;
        closesocket(socket_file_descriptor);
        WSACleanup();
        return 1;
    }

    // ������ ������������� ����������
    if (listen(socket_file_descriptor, 5) == SOCKET_ERROR) {
        cout << "Listening failed!" << endl;
        cout << "������ ������������� ����������!" << endl;
        cout << "    " << endl;
        closesocket(socket_file_descriptor);
        WSACleanup();
        return 1;
    }

    cout << "Server started. Waiting for connections..." << endl;
    cout << "������ �������. �������� �����������..." << endl;
    cout << "    " << endl;

    while (1) {
        // ��������� ����� ����������
        int client_address_size = sizeof(client);
        connection = accept(socket_file_descriptor, (struct sockaddr*)&client, &client_address_size);
        if (connection == INVALID_SOCKET) {
            cout << "Failed to accept connection!" << endl;
            cout << "�� ������� ������� ����������!" << endl;
            cout << "    " << endl;
            closesocket(socket_file_descriptor);
            WSACleanup();
            return 1;
        }

        cout << "Client connected. Handling connection..." << endl;
        cout << "������ ���������. ��������� ����������..." << endl;
        cout << "    " << endl;

        // �������������� � ��������
        while (1) {
            memset(message, 0, sizeof(message));

            // ���� ��������� �� �������
            int bytes = recv(connection, message, sizeof(message), 0);
            if (bytes == SOCKET_ERROR) {
                cout << "Error receiving message from client!" << endl;
                cout << "������ ��� ��������� ��������� �� �������!" << endl;
                cout << "    " << endl;
                break;
            }
            else if (bytes == 0) {
                // ������ ������ ����������
                cout << "Client disconnected." << endl;
                cout << "������ ����������." << endl;
                cout << "    " << endl;
                break;
            }

            cout << "Data received from client: " << message << endl;
            cout << "�������� ������ �� �������: " << message << endl;
            cout << "    " << endl;

            // ���������� ����� �������
            if (send(connection, message, strlen(message), 0) == SOCKET_ERROR) {
                cout << "Error sending message to client!" << endl;
                cout << "������ �������� ��������� �������!" << endl;
                cout << "    " << endl;
                break;
            }
        }

        // ��������� ���������� � ��������
        closesocket(connection);
    }

    // ��������� ����� �������
    closesocket(socket_file_descriptor);

    // ��������� ���������� Winsock
    WSACleanup();

    return 0;
}
