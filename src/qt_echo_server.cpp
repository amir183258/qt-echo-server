#include <QCoreApplication>
#include <QDebug>

#include "echo_server.hpp"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

	EchoServer server;
	if (!server.start(9999))
		return 1; // error
	qInfo() << "Serve is online now";

	return app.exec();
}
