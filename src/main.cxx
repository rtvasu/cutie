#include <iostream>

#include "CutieSslServer.h"
#include <QApplication>
#include <QHostAddress>

int main(int argc, char *argv[]) {

    QApplication app{argc, argv};
    std::cout << "hello world\n";
    auto address = QHostAddress("127.0.0.1");
    quint32 port = 1234;

    auto configureSsl = CutieSslServer(&app, address, port);
}