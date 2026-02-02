#include "CutieSslServer.h"
#include "CutieSslServer.moc"

#include <QFile>
#include <QHostAddress>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslServer>

// Source: https://runebook.dev/en/docs/qt/qsslserver/QSslServer

CutieSslServer::CutieSslServer(QObject * /*parent*/,
                               const QHostAddress &address, quint32 port) {
    // 1. Configure the server's identity (the key and certificate)
    QSslConfiguration config;

    QFile certFile("../network/ca.crt");
    QFile keyFile("../network/ca.key");

    if (certFile.open(QIODevice::ReadOnly) &&
        keyFile.open(QIODevice::ReadOnly)) {

        // Load the certificate
        QSslCertificate certificate(&certFile);
        config.setLocalCertificate(certificate);
        qDebug() << "Certificate loaded:" << !certificate.isNull();

        // Load the private key (assuming RSA and no passphrase for
        // simplicity)
        QSslKey privateKey(keyFile.readAll(), QSsl::Rsa);
        config.setPrivateKey(privateKey);
        qDebug() << "Private Key loaded:" << !privateKey.isNull();

        // Set the configuration to the QSslServer
        this->setSslConfiguration(config);

    } else {
        qWarning() << "CRITICAL: Could not load certificate or key files!";
        // Handle this error: maybe stop the app or log a fatal error.
    }

    // 2. Start Listening!
    if (this->listen(address, port)) {
        qDebug() << "QSslServer is listening securely on address" << address
                 << "and port" << port;
    } else {
        qCritical() << "Server failed to listen:" << this->errorString();
    }

    // 3. Connect to a signal to handle new connections
    connect(this, &QSslServer::pendingConnectionAvailable, this,
            &CutieSslServer::onNewConnection);
}

CutieSslServer::~CutieSslServer() = default;

void CutieSslServer::onNewConnection() {
    // Fetch the new connection (it's a QSslSocket)
    QSslSocket *socket = static_cast<QSslSocket *>(nextPendingConnection());

    if (socket) {
        qDebug() << "New connection pending. Starting handshake...";

        // Handle potential SSL errors during the handshake
        connect(socket, &QSslSocket::sslErrors, this,
                &CutieSslServer::onSslErrors);

        // Handle data when it's ready
        connect(socket, &QSslSocket::readyRead, this,
                &CutieSslServer::onReadyRead);

        // ... (other connections like disconnected, error, etc.)
    }
}

void CutieSslServer::onSslErrors(const QList<QSslError> &errors) {
    // This is crucial: handle issues like self-signed certs or expired
    // certs
    qWarning() << "SSL Errors encountered!";
    for (const auto &error : errors) {
        qWarning() << "Error:" << error.errorString();
    }

    // **Decide if you want to ignore errors (usually BAD for security!)**
    // socket->ignoreSslErrors();

    // If you don't call ignoreSslErrors(), the connection will be aborted.
}

void CutieSslServer::onReadyRead() {
    QSslSocket *socket = qobject_cast<QSslSocket *>(sender());
    if (socket && socket->isEncrypted()) {
        QByteArray data = socket->readAll();
        qDebug() << "Received secure data:" << data;
        // Echo back securely!
        socket->write("Hello Secure Client!");
    }
}
