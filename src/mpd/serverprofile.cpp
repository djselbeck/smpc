#include "serverprofile.h"

ServerProfile::ServerProfile(QObject *parent) :
    QObject(parent)
{
}

ServerProfile::ServerProfile(QString hostname, QString password, int port, QString name, bool autocon, QString macAddr)
{
    this->mHostname = hostname;
    this->mPassword = password;
    this->mPort = port;
    this->mName = name;
    this->mAutoconnect = autocon;
    this->mMACAdress = macAddr;
}

void ServerProfile::setHostname(QString hostname)
{
    this->mHostname = hostname;
    emit valueChanged();
}

void ServerProfile::setPassword(QString password)
{
    this->mPassword = password;
    emit valueChanged();
}

void ServerProfile::setName(QString name)
{
    this->mName = name;
    emit valueChanged();
}

void ServerProfile::setPort(int port)
{
    this->mPort = port;
    emit valueChanged();
}

void ServerProfile::setAutoconnect(bool autoconnect)
{
    this->mAutoconnect = autoconnect;
}

void ServerProfile::setMACAdress(QString macAddr)
{
    this->mMACAdress = macAddr;
}


QString ServerProfile::getHostname()
{
    return mHostname;
}

QString ServerProfile::getPassword()
{
    return mPassword;
}

QString ServerProfile::getName()
{
    return mName;
}

int ServerProfile::getPort()
{
    return mPort;
}

int ServerProfile::getStreamingPort()
{
    return mStreamingPort;
}

void ServerProfile::setStreamingPort(int port)
{
    mStreamingPort = port;
}

bool ServerProfile::getAutoconnect()
{
    return mAutoconnect;
}

QString ServerProfile::getMACAddress()
{
    return mMACAdress;
}
