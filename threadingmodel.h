#ifndef WAN_THREADINGMODEL_H
#define WAN_THREADINGMODEL_H

/**
 * @author Kevin Wan <wanjunfeng@gmail.com>
 * @date   12/30/2005
 * Copyright (C) Kevin Wan
 */

namespace wan
{
struct SingleThreadTag {};
struct MultiThreadTag {};

template <typename LockType>
class ThreadingModel
{
public:
    typedef MultiThreadTag ThreadTag;

    class Lock
    {
        Lock(const Lock&);
        void operator=(const Lock&);

    public:
        explicit Lock(const ThreadingModel& host)
            : m_host(host)
        {
            m_host.m_mutex.lock();
        }
        ~Lock()
        {
            m_host.m_mutex.unlock();
        }

    private:
        const ThreadingModel& m_host;
    };

    friend class Lock;

    ThreadingModel() {}

private:
    ThreadingModel(const ThreadingModel&);
    ThreadingModel& operator=(const ThreadingModel&);

private:
    mutable LockType    m_mutex;
};

template <>
class ThreadingModel<void>
{
public:
    typedef SingleThreadTag ThreadTag;

    struct Lock
    {
        explicit Lock(const ThreadingModel&) {}
    };

    ThreadingModel() {}

private:
    ThreadingModel(const ThreadingModel&);
    ThreadingModel& operator=(const ThreadingModel&);
};
} // namespace

#endif // WAN_THREADINGMODEL_H
