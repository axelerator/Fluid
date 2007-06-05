#ifndef CTHREAD_H
  #define CTHREAD_H
  #ifdef WIN32
    #include <windows.h>
  #else
    #include <pthread.h>
  #endif

  class cThread {
    private:
      #ifdef WIN32
        HANDLE pThread;
        static DWORD WINAPI pProc(LPVOID pPtr);
      #else
        pthread_t pThread;
        static void* pProc(void* pPtr);
      #endif

    private:
      virtual void run() = 0;

    public:
      virtual ~cThread() {};
      void start();
      void join();
  };
#endif
