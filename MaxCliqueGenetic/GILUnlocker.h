#ifndef __GIL_UNLOCKER__
#define __GIL_UNLOCKER__

#include <boost/python.hpp>

class GILUnlocker
{
public:
	inline GILUnlocker()
	{
        thread_state = PyEval_SaveThread();
    }

    inline ~GILUnlocker()
	{
        PyEval_RestoreThread(thread_state);
    }

private:
	PyThreadState *thread_state;
};


#endif // __GIL_UNLOCKER__