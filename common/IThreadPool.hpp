#pragma once

#include "Thread.hh"

namespace Spider
{
	class IThreadPool
	{
	public:
		virtual ~IThreadPool() {};
		virtual bool addThread(IThread *thread) = 0;
		virtual bool removeThread(IThread *thread) = 0;
		virtual void joinAll() = 0;
		virtual void interruptAll() = 0;
		virtual int  getSize() = 0;
		virtual bool is_this_thread_in() = 0;
		virtual bool is_thread_in(IThread *thread) = 0;
	};
}