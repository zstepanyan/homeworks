#include <QApplication>
#include "EntryWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    EntryWindow entryWindow;
    entryWindow.show();
    return app.exec();
}
