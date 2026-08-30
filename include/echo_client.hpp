#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QtTypes>

class EchoClient final : public QObject {
Q_OBJECT
public:
	explicit EchoClient(QObject *parent = nullptr);

	bool isConnected() const;

public slots:
	bool start(const QString ip_address, quint16 port);
	void stop();
	qint64 sendMessage(const QByteArray &message);

signals:
	void connected();
	void disconnected();
	void messageReceived(const QByteArray &message);
	void errorOccured(QAbstractSocket::SocketError error);

private:
	QTcpSocket m_socket;

};
