#ifndef MYDAEMONTHREAD_H
#define MYDAEMONTHREAD_H

#include <QObject>
#include <QThread>

#include <QObject>
#include <windows.h>
#include <tchar.h>
#include <libloaderapi.h>

#include <string>
#include <iostream>
using namespace std;

class MyDaemonThread : public QThread
{
    Q_OBJECT
public:
    MyDaemonThread(QObject *parent, string exe_path);
    
    void run(); 
    
    string get_exe_path() const;
    void set_exe_path(const string &value);
    
signals:
    void signalLog(const QString& msg);
    
private:
    string exe_path;
};

#endif // MYDAEMONTHREAD_H
