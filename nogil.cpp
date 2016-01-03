#include <iostream>
#include <vector>
#include <thread>
#include <boost/python.hpp>

class ScopedGILRelease {
public:
  inline ScopedGILRelease() { m_thread_state = PyEval_SaveThread(); }
  inline ~ScopedGILRelease() { PyEval_RestoreThread(m_thread_state); m_thread_state = NULL; }
private:
  PyThreadState* m_thread_state;
};

void loop(long count)
{
  while (count != 0) {
    count -= 1;
  }
  return;
}

void nogil(int threads, long count)
{
  if (threads <= 0 || count <= 0)
    return;

  ScopedGILRelease release_gil = ScopedGILRelease();
  long thread_count = (long)ceil(count / threads);

  std::vector<std::thread> v_threads;
  for (int i=0; i != threads; i++)
    {
      v_threads.push_back(std::thread(loop,thread_count));
    }

  std::for_each(v_threads.begin(),v_threads.end(), std::mem_fn(&std::thread::join));
 
  return;
}

BOOST_PYTHON_MODULE(nogil)
{
  using namespace boost::python;
  def("nogil", nogil);
}


