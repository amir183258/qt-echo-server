#include <iostream>
#include <unistd.h>

#include <QCoreApplication>
#include <QSocketNotifier>
#include <QDebug>

#include "echo_client.hpp"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

	EchoClient client;

	QSocketNotifier stdinNotifier(STDIN_FILENO, QSocketNotifier::Read);
	stdinNotifier.setEnabled(false);

	QObject::connect(&client, &EchoClient::connected, [&stdinNotifier]() {
			qInfo() << "Connected to server! Type a message and press Enter:";
			stdinNotifier.setEnabled(true);
	});

	QObject::connect(&client, &EchoClient::disconnected, [&stdinNotifier]() {
			qInfo() << "Disconnected from server. Disabling input.";
			stdinNotifier.setEnabled(false);
	});

	QObject::connect(&client, &EchoClient::messageReceived, [](const QByteArray &message) {
			qInfo().noquote() << "[Echo from Server]:" << message.trimmed();
	});

	QObject::connect(&stdinNotifier, &QSocketNotifier::activated, [&client]() {
			std::string line;
			if (std::getline(std::cin, line))
				client.sendMessage(QByteArray::fromStdString(line + '\n'));
			else {
				qInfo() << "EOF reached. Exiting...";
				QCoreApplication::quit();
			}
	});
			
	if (!client.start("127.0.0.1", 9999)) {
		qCritical() << "Failed to initiate connection.";
		return 1;
	}

	return app.exec();
}
