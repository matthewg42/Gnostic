#include <QApplication>
#include "PasswordDialog.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QStringList args = app.arguments();
	args.pop_front();
	PasswordDialog d(args.join(" "));
	d.exec();
	std::cout << d.getPassword().toUtf8().constData() << std::endl;
	return 0;
}
