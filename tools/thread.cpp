/*
 * $Log: thread.cpp,v $
 * Revision 1.1.1.1  2006/03/17 19:49:17  wamas
 * own tools reponsitority
 *
 * Revision 1.2  2006/03/09 00:48:27  wamas
 * Added CVS Log Info
 *
 */

#ifndef WIN32
#  include <unistd.h>
#  include <errno.h>
#endif


#include <stdio.h>
#include "thread.h"

#ifdef TOOLS_USE_THREADS

#include <iostream>

namespace Tools {
                                                                       
#ifdef WIN32
DWORD __stdcall run_thread( void* ptr )
#else
extern "C" void* run_thread( void* ptr ) 
#endif
{
    Thread* thread = (Thread*) ptr;

    thread->runThread();
//    printf( "done run\n" );

#ifdef WIN32
    ExitThread(0);
#endif
    return 0;
}

#ifdef WIN32
unsigned Thread::Mutex::MutexStruct::id = 1;
#endif

Thread::Mutex::Mutex()
{
#ifdef WIN32
    char buffer[50];
    sprintf( buffer, "BS Thread Mutex %d", mutex.id++ );
    mutex.handle = CreateMutex( 0, false, buffer );
    mutex.name = buffer;
#else 
    pthread_mutex_init( &mutex, 0 ); 
#endif
}

Thread::Mutex::~Mutex()
{
#ifdef WIN32   
    CloseHandle( mutex.handle );
#else
    pthread_mutex_destroy( &mutex );
#endif
}

void Thread::Mutex::lock()
{
#ifdef WIN32
    WaitForSingleObject( mutex.handle, INFINITE);    
#else
    pthread_mutex_lock( &mutex );
#endif
}

void Thread::Mutex::unlock()
{ 
#ifdef WIN32
    ReleaseMutex( mutex.handle );
#else
    pthread_mutex_unlock( &mutex );
#endif
}

bool Thread::Mutex::locked()
{ 
#ifdef WIN32
    if( WaitForSingleObject( mutex.handle, 1L ) == WAIT_TIMEOUT )
    return true;
#else
    if( pthread_mutex_trylock( &mutex ) == EBUSY )
	return true; 
#endif
    
    unlock(); 
    return false; 
}

Thread::~Thread()
{

}

void Thread::start()
{
    int sc = getStartCount();

#ifdef WIN32
    DWORD tid; // required on WIN9x (even if M$ says only "may" here)
    CreateThread( NULL, 0, run_thread, this, 0, &tid );
#else
    pthread_create( &thread, 0, run_thread, this ); 	   		
#endif

    // wait until the thread runns
    while( !running() && getStartCount() == sc )
    {
#ifdef WIN32
    Sleep(0);
#else    
	sched_yield();
#endif
    }
}

int Thread::getStartCount()
{ 
    int r; 
    m_start_count.lock(); 
    r = start_count; 
    m_start_count.unlock(); 
    return r; 
}

void Thread::exit()
{
#ifdef WIN32
    ExitThread(0);
#else
    pthread_exit(0);
#endif
}

void Thread::runThread()
{ 
    m_running.lock(); 
    m_start_count.lock();
    start_count++; 
    m_start_count.unlock();
    run(); 
    m_start_count.lock();
    start_count--;
    m_start_count.unlock();
    m_running.unlock(); 
}

} // namespace Tools

#endif
