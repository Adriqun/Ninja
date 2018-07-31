#include <QtWidgets/QApplication>
#include <qfontdatabase.h>
#include "mainwindow.h"
#include "buttonfactory.h"
#include "logging.h"

int main(int argc, char *argv[])
{
	// App.
	QApplication application(argc, argv);
	int id = QFontDatabase::addApplicationFont("fonts/Jaapokki-Regular.otf");
	QString family = QFontDatabase::applicationFontFamilies(id).at(0);
	QFont font(family);
	QApplication::setFont(font);

	MainWindow* window = new MainWindow;

	Logging logging;
	logging.load(window->size());
	logging.setWindowAsParent(window);

	// Buttons.
	//ButtonFactory buttonFactory;
	//buttonFactory.load(window->size());
	//buttonFactory.setParent(window);

	window->show();
	return application.exec();
}
