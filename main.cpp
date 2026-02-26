#include "StudentScoreManager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    StudentScoreManager window;
    window.show();
    return app.exec();
}
