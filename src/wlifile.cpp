#include "wlifile.h"

WliFile::WliFile(QListWidget *parent,int type) :
    QListWidgetItem(parent)
{
}

WliFile::WliFile(MpdFileEntry *file, QListWidget *parent,int type) :
    QListWidgetItem(parent)
{
    this->file = file;
    if(file != NULL)
    {
        setText(text());
        if(file->isFile())
        {
            setIcon(QIcon(":/icons/audio-x-generic.png"));
        }
        if(file->isDirectory())
        {
            setIcon(QIcon(":/icons/folder.png"));
        }
    }
}

WliFile::~WliFile()
{

    delete(file);
}

QString WliFile::text()
{
    if(file != NULL)
    {
        return file->getName();
    }
    return "";
}

MpdFileEntry *WliFile::getFile()
{
    return file;
}
