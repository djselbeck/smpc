#include <QGuiApplication>
#include <QQuickView>

#include "src/controller.h"

#include "sailfishapplication.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(Sailfish::createApplication(argc, argv));
    QScopedPointer<QQuickView> view(Sailfish::createView("main.qml"));
    Controller *control = new Controller(&(*view),0);
    Sailfish::showView(view.data());

    return app->exec();
}


