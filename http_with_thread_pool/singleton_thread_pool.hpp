/*这是一个基于任务队列的单例线程池，传入的任务类型*/
#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include <unistd.h>
#include "task_for_engine.hpp"
#define NUMS 5

namespace thread_pool_util
{
    template <class T>
    class thread_pool
    {
        typedef thread_pool<T> self;
    private:
        static self *_tp;
        static pthread_mutex_t _tp_mtx;
        int _nums;
        std::queue<T> _task_queue;
        pthread_mutex_t mtx; //保护队列内的临界资源
        pthread_cond_t cond; //条件变量，同步
    private:
        //使该类无法被拷贝和赋值，且只能通过对外接口getInstance创建
        thread_pool(int nums = NUMS) : _nums(nums)
        {
            pthread_mutex_init(&mtx, nullptr);
            pthread_cond_init(&cond, nullptr);
        }
        thread_pool(const self& tp) = delete;
        self &operator=(const self& tp) = delete;
    public:
        //通过该接口创建对象并初始化线程池
        static self* getInstance(int nums=NUMS)
        {
            if (_tp == nullptr)
            {
                pthread_mutex_lock(&_tp_mtx);
                if (_tp == nullptr)
                {
                    _tp=new self();
                    _tp->init_thread_pool(nums);
                }
                pthread_mutex_unlock(&_tp_mtx);
            }
            return _tp;
        }
        //在类内要让函数执行类内成员方法，需使用静态成员函数。此与thread的底层实现有关
        static void work(void *args)
        {
            std::cout << "thread " << std::this_thread::get_id() << "is runing" << std::endl;
            thread_pool<T> *tp = (thread_pool<T> *)args;
            while (1)
            {
                tp->Lock();
                while (tp->isEmpty())
                {
                    std::cout<<"当前无请求需处理"<<std::endl;
                    tp->needWait();
                }
                T task;
                tp->popTask(&task);
                tp->Unlock();
                //sleep(1);
                std::cout<<"处理请求"<<std::endl;
                task.run();
            }
        }
        //创建创建线程池
        void init_thread_pool(int nums)
        {   
            std::cout<<"当前线程池线程数："<<nums<<std::endl;
            for (int i = 0; i < nums; ++i)
            {
                std::thread thrd(work, (void *)this);
                thrd.detach();
            }
        }
        bool isEmpty()
        {
            return _task_queue.empty();
        }
        void push(const T &task)
        {
            Lock();
            _task_queue.push(task);
            Unlock();
            Singal();
        }
        // void pop()
        // {
        //     Lock();
        //     while (isEmpty())
        //     {
        //         needWait();
        //     }
        //     _task_queue.pop();
        //     Unlock();
        // }
        // popTask非线程安全的
        void popTask(T *task)
        {
            *task = _task_queue.front();
            _task_queue.pop();
        }
        
        // lock
        void Lock()
        {
            pthread_mutex_lock(&mtx);
        }
        void Unlock()
        {
            pthread_mutex_unlock(&mtx);
        }
        // cond
        void needWait()
        {
            pthread_cond_wait(&cond, &mtx);
        }
        void Singal()
        {
            pthread_cond_signal(&cond);
        }

        ~thread_pool()
        {
            pthread_mutex_destroy(&mtx);
            pthread_cond_destroy(&cond);
        }
    };
    template <class T>
    thread_pool<T>* thread_pool<T>::_tp = nullptr;
    template <class T>
    pthread_mutex_t thread_pool<T>::_tp_mtx=PTHREAD_MUTEX_INITIALIZER;
}