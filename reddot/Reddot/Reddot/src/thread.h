
#ifndef THREAD_H
#define THREAD_H

typedef HANDLE thread_t;
typedef DWORD (WINAPI *thread_func_t)(void*);

bool thread_init(thread_t& t, thread_func_t func, void* param = NULL);
bool thread_resume(thread_t& t);
bool thread_suspend(thread_t& t);
bool thread_join(thread_t& t);
bool thread_done(thread_t& t);

#endif // THREAD_H
