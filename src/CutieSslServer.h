#pragma once

#include <QObject>
#include <QSslServer>
#include <QString>

class QSslConfiguration;
class QSslServer;
class QHostAddress;

class CutieSslServer : public QSslServer {
    Q_OBJECT
public:
    CutieSslServer(QObject *parent = nullptr,
                   const QHostAddress &address = QHostAddress::Any,
                   quint32 port = 1234);
    ~CutieSslServer() override;
    Q_DISABLE_COPY_MOVE(CutieSslServer)

private slots:
    void onNewConnection();

    void onSslErrors(const QList<QSslError> &errors);

    void onReadyRead();
};
