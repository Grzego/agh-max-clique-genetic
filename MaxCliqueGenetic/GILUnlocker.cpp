#include "GILUnlocker.h"

GILUnlocker::GILUnlocker()
{
	thread_state = PyEval_SaveThread();
}

GILUnlocker::~GILUnlocker()
{
	PyEval_RestoreThread(thread_state);
	thread_state = 0;
}