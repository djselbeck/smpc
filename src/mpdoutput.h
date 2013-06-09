#ifndef MPDOUTPUT_H
#define MPDOUTPUT_H

#include <QObject>
#include <QString>

class MPDOutput : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString outputname READ getName )
    Q_PROPERTY(bool outputenabled READ getEnabled WRITE setEnabled NOTIFY activechanged )
    Q_PROPERTY(int id READ getID )
public:
    explicit MPDOutput(QString name, bool enabled, int id,QObject *parent = 0);

    QString getName() {return mName;}
    bool getEnabled() {return mEnabled;}
    int getID() {return mID;}

    void setEnabled(bool en);


private:
    bool mEnabled;
    QString mName;
    int mID;
    
signals:
    void activechanged();
    
public slots:
    
};

#endif // MPDOUTPUT_H
