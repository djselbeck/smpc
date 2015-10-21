#include <QGuiApplication>
#include <QDebug>

#include "src/controller.h"

#ifdef Q_OS_SAILFISH
#include <QQuickView>
#include <sailfishapp.h>
#endif

#ifdef Q_OS_ANDROID
#include <QApplication>
#include <QQmlApplicationEngine>
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
#ifdef Q_SAILFISH
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setOrganizationName("harbour-smpc");
    app->setApplicationName("harbour-smpc");
    QString locale = QLocale::system().name();
    QString translationFile = QString(":translations/harbour-smpc_") + locale;
    QTranslator translator;
    translator.load(translationFile);
    app->installTranslator(&translator);
//    QScopedPointer<QQuickView> view(SailfishApp::createView());
    QQuickView *view = SailfishApp::createView();
    view->engine()->addImportPath("/usr/share/harbour-smpc/qml/");
    view->setSource(SailfishApp::pathTo("qml/main.qml"));
    view->setDefaultAlphaBuffer(true);

    foreach (QString path, view->engine()->importPathList()) {
        qDebug() << path;
    }

    Controller control(view->engine(),view->rootObject(),0);
    view->show();
    return app->exec();
#endif

#ifdef Q_OS_ANDROID
    QApplication app(argc, argv);
    app.setOrganizationName("ampc");
    app.setApplicationName("ampc");


    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/qml/android/main.qml")));
    Controller controller(&engine,engine.rootObjects().first());



    return app.exec();
#endif
}


