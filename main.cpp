#include <QGuiApplication>
#include <QQuickView>
#include <QDebug>
#include <src/commondebug.h>

#include "src/controller.h"

//#include "sailfishapplication.h"

#include <sailfishapp.h>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setOrganizationName("smpc");
    app->setApplicationName("smpc");
    QString locale = QLocale::system().name();
    QString translationFile = QString(":translations/harbour-smpc_") + locale;
    QTranslator translator;
    CommonDebug("Trying: " + translationFile);
    translator.load(translationFile);
    app->installTranslator(&translator);
    QScopedPointer<QQuickView> view(SailfishApp::createView());
    view->setSource(SailfishApp::pathTo("qml/main.qml"));
    view->setDefaultAlphaBuffer(true);

    Controller *control = new Controller(&(*view),0);
    view->show();
    return app->exec();
}


