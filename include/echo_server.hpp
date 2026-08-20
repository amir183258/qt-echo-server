#pragma once

#include <QObject>
#include <QTcpServer>

class EchoServer final : public QObject {
Q_OBJECT
public:
	explicit EchoServer(QObject* parent = nullptr);

public slots:
	bool start(quint16 port);
	void stop();

private slots:
	void onNewConnection();

private:
	QTcpServer m_server;

};

