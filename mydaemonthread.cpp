#include "mydaemonthread.h"

MyDaemonThread::MyDaemonThread(QObject *parent, string exe_path) : QThread(parent), exe_path(exe_path)
{
    
}

void MyDaemonThread::run()
{
    // 循环检测进程运行情况打印进程崩溃日志
    do {
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        
        if (!CreateProcessA(exe_path.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            char msg[256] = { 0 };
            SYSTEMTIME st;
            GetSystemTime(&st);
            sprintf_s(msg, "%04d-%02d-%02d-%02d-%02d-%02d Create process SmartChargeWall.exe failed", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
            emit signalLog(msg);
            Sleep(5000);
        }

        // 等待进程异常退出
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD returnCode;
        GetExitCodeProcess(pi.hProcess, &returnCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        // 写异常日志
        char msg[256] = { 0 };
        SYSTEMTIME st;
        GetSystemTime(&st);
        sprintf_s(msg, "%04d-%02d-%02d-%02d-%02d-%02d SmartChargeWall exit(%u)",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond, returnCode);
        emit signalLog(msg);
    } while (1);
}

string MyDaemonThread::get_exe_path() const
{
    return exe_path;
}

void MyDaemonThread::set_exe_path(const string &value)
{
    exe_path = value;
}
