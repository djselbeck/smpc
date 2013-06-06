#ifndef WLIFILE_H
#define WLIFILE_H

#include <QListWidgetItem>

#include "mpdfileentry.h"

class WliFile : public QListWidgetItem
{

public:
    explicit WliFile(QListWidget *parent = 0,int type = 1000);
    WliFile(MpdFileEntry *file, QListWidget *parent = 0,int type = 1000);
    virtual ~WliFile();
    MpdFileEntry *getFile();
    QString text();


signals:

public slots:

private:
    MpdFileEntry *file;

};

#endif // WLIFILE_H
