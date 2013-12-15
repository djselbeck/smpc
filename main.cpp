#include <QGuiApplication>
#include <QQuickView>
#include <QDebug>
#include <src/commondebug.h>

#include "src/controller.h"

#include "sailfishapplication.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(Sailfish::createApplication(argc, argv));
    app->setOrganizationName("smpc");
    app->setApplicationName("smpc");
    QString locale = QLocale::system().name();
    QString translationFile = QString(":translations/harbour-smpc_") + locale;
    QTranslator translator;
    CommonDebug("Trying: " + translationFile);
    translator.load(translationFile);
    app->installTranslator(&translator);
    QScopedPointer<QQuickView> view(Sailfish::createView("main.qml"));
    view->setDefaultAlphaBuffer(true);
    view->showFullScreen();

    Controller *control = new Controller(&(*view),0);
    Sailfish::showView(view.data());
    return app->exec();
}


