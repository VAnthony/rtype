#pragma once
#include "IThreadPool.hpp"

class AThreadPool :	public IThreadPool
{
public:
	AThreadPool();
	virtual ~AThreadPool();
	virtual bool addThread(IThread *thread) = 0;
	virtual bool removeThread(IThread *thread) = 0;
	virtual void joinAll() = 0;
	virtual int  getSize();
	virtual bool is_thread_in(IThread *thread) = 0;
	virtual void deleteUnusedThread() = 0;

protected:
	int _size;
};
