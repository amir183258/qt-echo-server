#include <QObject>
#include <QTcpSocket>
#include <QDebug>

#include "echo_server.hpp"

EchoServer::EchoServer(QObject *parent)
	: QObject(parent)
{
	connect(&m_server, &QTcpServer::newConnection,
			this, &EchoServer::onNewConnection);
}

bool EchoServer::start(quint16 port) {
	if (m_server.isListening()) {
		qWarning() << "Server is already listening";
		return false;
	}

	if (!m_server.listen(QHostAddress::Any, port)) {
		qCritical() << "Failed to start server: " <<
			m_server.errorString();

		return false;
	}

	qInfo() << "Echo server is listening on port" <<
		m_server.serverPort();

	return true;
}

void EchoServer::stop() {
	m_server.close();
	qInfo() << "Echo server stopped";
}

void EchoServer::onNewConnection() {
	while (m_server.hasPendingConnections()) {
		QTcpSocket *socket = m_server.nextPendingConnection();

		qInfo() << "Client connected from" <<
			socket->peerAddress().toString() <<
			"port" <<
			socket->peerPort();

		// echo data
		connect(socket, &QTcpSocket::readyRead, this, [socket] {
				const QByteArray data = socket->readAll();
				qInfo() << "Received: " << data;

				socket->write(data);
				socket->flush();
				});

		// remove socket if disconnected
		connect(socket, &QTcpSocket::disconnected,
				socket, &QObject::deleteLater);
		
		// log client disconnection
		connect(socket, &QTcpSocket::disconnected, this, [socket] {
				qInfo() << "Client disconnected: " <<
				socket->peerAddress().toString();
				});

		// handle error
		connect(socket, &QTcpSocket::errorOccurred, this,
				[socket](QAbstractSocket::SocketError error) {
				Q_UNUSED(error);

				qWarning() << "Socket error:" <<
					socket->errorString();
				});
	}
}
