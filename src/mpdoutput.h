#ifndef MPDOUTPUT_H
#define MPDOUTPUT_H

#include <QObject>
#include <QString>

class MPDOutput : public QObject
{
    Q_OBJECT
public:
    explicit MPDOutput(QString name, bool enabled, int id,QObject *parent = 0);


private:
    bool mEnabled;
    QString mName;
    int mID;
    
signals:
    
public slots:
    
};

#endif // MPDOUTPUT_H
