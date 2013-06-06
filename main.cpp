
#include <QApplication>
#include <QDeclarativeView>

#include "sailfishapplication.h"
#include "src/controller.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(Sailfish::createApplication(argc, argv));
    QScopedPointer<QDeclarativeView> view(Sailfish::createView("main.qml"));
    Controller *control = new Controller(&(*view),0);


    Sailfish::showView(view.data());

    
    return app->exec();
}


