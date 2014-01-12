#ifndef SERVERPROFILE_H
#define SERVERPROFILE_H

#include <QObject>

class ServerProfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hostname READ getHostname WRITE setHostname  NOTIFY valueChanged)
    Q_PROPERTY(QString password READ getPassword WRITE setPassword  NOTIFY valueChanged)
    Q_PROPERTY(QString name READ getName WRITE setName  NOTIFY valueChanged)
    Q_PROPERTY(int port READ getPort WRITE setPort NOTIFY valueChanged)
    Q_PROPERTY(int streamingport READ getPort WRITE setPort NOTIFY valueChanged)
    Q_PROPERTY(bool autoconnect READ getAutoconnect WRITE setAutoconnect)

public:
    explicit ServerProfile(QObject *parent = 0);
    ServerProfile(QString mHostname, QString mPassword, int mPort, QString mName,bool autocon);
    QString getHostname();
    QString getPassword();
    QString getName();
    int getPort();
    int getStreamingPort();
    bool getAutoconnect();
    void setPort(int mPort);
    void setStreamingPort(int port);
    void setName(QString mName);
    void setHostname(QString mHostname);
    void setPassword(QString mPassword);
    void setAutoconnect(bool autocon);

signals:
    void valueChanged();

private:
    QString mHostname;
    int mPort;
    QString mPassword;
    QString mName;
    bool mAutoconnect;
    int mStreamingPort;

public slots:

};

#endif // SERVERPROFILE_H
