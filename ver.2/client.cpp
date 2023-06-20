#include <iostream>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define MESSAGE_LENGTH 1024 // ������������ ������ ������ ��� ������
#define PORT 7777 // ����� ������������ ���� ����� �����

SOCKET socket_file_descriptor;
struct sockaddr_in serveraddress;
char message[MESSAGE_LENGTH];

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
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP ����� �������
    serveraddress.sin_port = htons(PORT); // ����� ����� �������
    serveraddress.sin_family = AF_INET; // ���������� IPv4

    // ��������� ���������� � ��������
    if (connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) == SOCKET_ERROR) {
        cout << "�� ������� ���������� ���������� � ��������!" << endl;
        cout << "    " << endl;
        closesocket(socket_file_descriptor);
        WSACleanup();
        return 1;
    }

    // �������������� � ��������
    while (1) {
        memset(message, 0, sizeof(message));
        cout << "������� ���������, ������� �� ������ ��������� �� ������: ";
        cin.getline(message, sizeof(message));

        if (strcmp(message, "end") == 0) {
            // ���������� ��������� �� ������
            if (send(socket_file_descriptor, message, strlen(message), 0) == SOCKET_ERROR) {
                cout << "������ �������� ��������� �� ������!" << endl;
                cout << "    " << endl;
                break;
            }

            cout << "������ ��������." << endl;
            cout << "    " << endl;
            break;
        }

        // ���������� ��������� �� ������
        if (send(socket_file_descriptor, message, strlen(message), 0) == SOCKET_ERROR) {
            cout << "������ �������� ��������� �� ������!" << endl;
            cout << "    " << endl;
            break;
        }

        memset(message, 0, sizeof(message));

        // ���� ������ �� �������
        if (recv(socket_file_descriptor, message, sizeof(message), 0) == SOCKET_ERROR) {
            cout << "������ ��� ��������� ������ �� �������!" << endl;
            cout << "    " << endl;
            break;
        }

        cout << "�������� ������ �� �������: " << message << endl;
        cout << "    " << endl;
    }

    // ��������� �����
    closesocket(socket_file_descriptor);

    // ������� Winsock
    WSACleanup();

    return 0;
}