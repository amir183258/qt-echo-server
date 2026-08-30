#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QString>
#include <QtTypes>
#include <QDebug>
#include <QByteArray>

#include "echo_client.hpp"

EchoClient::EchoClient(QObject *parent)
	: QObject(parent)
{
	connect(&m_socket, &QTcpSocket::connected, this, &EchoClient::connected);

	connect(&m_socket, &QTcpSocket::disconnected, this, &EchoClient::disconnected);

	connect(&m_socket, &QTcpSocket::readyRead, this, [this]() {
			QByteArray data = m_socket.readAll();
			emit messageReceived(data);
	});

	connect(&m_socket, &QTcpSocket::errorOccurred, this, &EchoClient::errorOccured);
}

bool EchoClient::isConnected() const {
	return m_socket.state() == QTcpSocket::ConnectedState;
}

bool EchoClient::start(const QString ip_address, quint16 port) {
	// if socket is already connected or connecting
	if (m_socket.state() == QAbstractSocket::ConnectedState ||
			m_socket.state() == QAbstractSocket::ConnectingState) {
		qWarning() << "Client is already connected or connecting.";
		return false;
	}

	// if address is invalid
	QHostAddress hostAddress;
	if (!hostAddress.setAddress(ip_address)) {
		qWarning() << "Invalid IP address:" << ip_address;
		return false;
	}

	if (port == 0) {
		qWarning() << "Invalid port number:" << port;
		return false;
	}

	qDebug() << "Connecting to" << ip_address << ":" << port << "...";
	m_socket.connectToHost(hostAddress, port);

	return true;
}

void EchoClient::stop() {
	if (m_socket.state() != QAbstractSocket::UnconnectedState)
		m_socket.disconnectFromHost();
}

qint64 EchoClient::sendMessage(const QByteArray &message) {
	if (!isConnected()) {
		qWarning() << "Cannot send message: Socket is not connected!";
		return -1;
	}

	if (message.isEmpty())
		return 0;

	qint64 bytesWritten = m_socket.write(message);
	if (bytesWritten == -1)
		qWarning() << "Failed to write data to socket:" << m_socket.errorString();

	return bytesWritten;
}
