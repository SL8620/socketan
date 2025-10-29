// socketcan_driver.hpp
#ifndef SOCKETCAN_DRIVER_HPP
#define SOCKETCAN_DRIVER_HPP

#include <string>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sched.h>  // For real-time scheduling

// Callback type for received CAN frames
using CanFrameCallback = std::function<void(const struct can_frame&)>;

class SocketCanDriver {
public:
    /**
     * Constructor.
     * @param interface_name The name of the CAN interface (e.g., "can0").
     * @param recv_callback Callback function for received frames.
     */
    SocketCanDriver(const std::string& interface_name, CanFrameCallback recv_callback);

    /**
     * Destructor. Closes the socket and stops threads.
     */
    ~SocketCanDriver();

    /**
     * Start the driver threads.
     * @return True if started successfully, false otherwise.
     */
    bool start();

    /**
     * Stop the driver threads.
     */
    void stop();

    /**
     * Send a CAN frame.
     * @param frame The CAN frame to send.
     * @return True if enqueued successfully, false if queue is full or stopped.
     */
    bool send(const struct can_frame& frame);

private:
    // Internal methods
    void recvThread();
    void sendThread();

    // Member variables
    std::string interface_name_;
    CanFrameCallback recv_callback_;
    int socket_fd_ = -1;
    std::atomic<bool> running_ = false;

    // Send queue for high-rate sending
    std::queue<struct can_frame> send_queue_;
    std::mutex send_mutex_;
    std::condition_variable send_cv_;
    static constexpr size_t MAX_QUEUE_SIZE = 10000;  // Adjust based on needs

    // Threads
    std::thread recv_thread_;
    std::thread send_thread_;
};

#endif  // SOCKETCAN_DRIVER_HPP