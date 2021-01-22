#pragma once

#include <set>
#include <thread>
#include <mutex>

#include <libusb-1.0/libusb.h>

// forward declaration
class Connection;

class CrazyradioThread
{
public:
    CrazyradioThread(libusb_device * dev);

    CrazyradioThread(CrazyradioThread &&other)
    {
        std::unique_lock<std::mutex> rhs_lk(other.connections_mutex_);
        dev_ = std::move(other.dev_);
        thread_ = std::move(other.thread_);
        connections_ = std::move(other.connections_);
    }

    ~CrazyradioThread();

    bool isActive() const;

    libusb_device* device() {
        return dev_;
    }

    void addConnection(Connection* con);

    void removeConnection(Connection* con);

private: 
    void run();

private:
    libusb_device* dev_;
    std::thread thread_;

    std::mutex connections_mutex_;
    std::set<Connection*> connections_;
};