#include<list>
#include<thread>
#include<functional>
#include<memory>
#include <atomic>
#include <iostream>
#include <string>
#include <unistd.h>
#include "SyncQueue.cpp"

using namespace std;

const int MaxTaskCount = 100;
mutex g_mutex;
class ThreadPool
{
public:
    using Task = std::function<void()>;
    ThreadPool(int numThreads = 2) : m_queue(MaxTaskCount)
    {
        Start(numThreads);
    }

    ~ThreadPool(void)
    {
        //如果没有停止时则主动停止线程池
        Stop();
    }

    void Stop()
    {
        std::call_once(m_flag, [this]{StopThreadGroup(); }); //保证多线程情况下只调用一次StopThreadGroup
    }

    void AddTask(Task&&task)
    {
        m_queue.Put(std::forward<Task>(task));
    }

    void AddTask(const Task& task)
    {
        m_queue.Put(task);
    }

private:
    void Start(int numThreads)
    {
        m_running = true;
        //创建线程组，/消费者线程
        for (int i = 0; i <numThreads; ++i)
        {
            m_threadgroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
        }
    }    

    //消费者线程
    void RunInThread()
    {
        auto thdId = this_thread::get_id();
        while (m_running)
        {
            //取任务分别执行
            g_mutex.lock();
            cout << "WT: " << thdId << " is fetching task" << endl;
            g_mutex.unlock();

            std::list<Task> list;
            m_queue.Take(list);

            g_mutex.lock();
            cout << "WT: " << thdId << " fetching complete" << endl;
            for (auto& task : list)
            {
                if (!m_running)
                    return;
            cout << "WT: " << thdId << " task start" << endl;
                task();
            cout << "WT: " << thdId << " task end" << endl;
            }
            g_mutex.unlock();
        }
    }

    void StopThreadGroup()
    {
        m_queue.Stop(); //让同步队列中的线程停止
        m_running = false; //置为false，让内部线程跳出循环并退出

        for (auto thread : m_threadgroup) //等待线程结束
        {
            if (thread)
                thread->join();
        }
        m_threadgroup.clear();
    }
    std::list<std::shared_ptr<std::thread>> m_threadgroup; //处理任务的线程组
    SyncQueue<Task> m_queue; //同步队列     
    atomic_bool m_running; //是否停止的标志
    std::once_flag m_flag;
};

void test_pool()
{
    ThreadPool pool;
    //sleep(1);
    //生产者线程
    std::thread thd1([&pool]{
        auto thdId = this_thread::get_id();
        for(int i = 0; i < 5; i++)
        {
            //sleep(100);
            g_mutex.lock();
            cout << "synchronization thread1 ID: " << thdId << " is adding task" << endl;
            g_mutex.unlock();


            pool.AddTask([thdId]{
                cout << "task added from thread1 ID: "<< thdId << " is running..."<< endl;
            });

            g_mutex.lock();
            cout << "synchronization thread1 ID: " << thdId << " adding task complete" << endl;
            g_mutex.unlock();
        }
        g_mutex.lock();
        cout << "synchronization thread1 ID: " << thdId << " run out of task prduction!" << endl;
        g_mutex.unlock(); 
    });

    std::thread thd2([&pool]{
        auto thdId = this_thread::get_id();
        for(int i = 0; i < 5; i++)
        {
            g_mutex.lock();
            cout << "synchronization thread2 ID: " << thdId << " is adding task" << endl;
            g_mutex.unlock();

            pool.AddTask([thdId]{
                cout << "task added from thread2 ID: "<< thdId << " is running..."<< endl;
            });

            g_mutex.lock();
            cout << "synchronization thread2 ID: " << thdId << " adding task complete" << endl;
            g_mutex.unlock();
        }
        g_mutex.lock();
        cout << "synchronization thread2 ID: " << thdId << " run out of task prduction!" << endl;
        g_mutex.unlock(); 
    });
    system("pause");
}
int main()
{
   test_pool();
   return 0;
}
