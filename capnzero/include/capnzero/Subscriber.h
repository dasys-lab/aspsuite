#pragma once

#include "capnzero/Common.h"

#include <capnp/serialize-packed.h>

#include <zmq.h>

#include <string>

#include <assert.h>
#include <functional>
#include <thread>
#include <unistd.h>

namespace capnzero
{

class Subscriber
{
public:
    typedef std::function<void(::capnp::FlatArrayMessageReader&)> callbackFunction;
    // typedef void (ObjType::*callbackMemberFunction)(::capnp::FlatArrayMessageReader &);
    // typedef void (*callbackFunction)(::capnp::FlatArrayMessageReader &);

    //    template <class CallbackObjType>
    //    Subscriber(void *context, std::string connection, std::string multicastGroupName,
    //               void (CallbackObjType::*callbackFunction)(::capnp::FlatArrayMessageReader &),
    //               CallbackObjType *callbackObject)
    //        : socket(nullptr)
    //        , multicastGroupName(multicastGroupName)
    //        , callbackObject_(callbackObject)
    //        , callbackFunction_(callbackFunction)
    //		, callbackMemberFunction_(nullptr)
    //    {
    //        this->socket = zmq_socket(context, ZMQ_DISH);
    //        check(zmq_bind(this->socket, connection.c_str()), "zmq_connect");
    //        check(zmq_join(this->socket, this->multicastGroupName.c_str()), "zmq_join");
    //    }

    Subscriber(void* context, CommType commType, std::string address, std::string multicastGroupName = "")
            : socket(nullptr)
            , multicastGroupName(multicastGroupName)
            , callbackFunction_(nullptr)
            , running(true)
            , runThread(nullptr)
            , rcvTimeout(15) {
        if (commType == CommType::UDP_MULTICAST) {
            this->socket = zmq_socket(context, ZMQ_DISH);
            check(zmq_setsockopt(this->socket, ZMQ_RCVTIMEO, &rcvTimeout, sizeof(rcvTimeout)), "zmq_setsockopt");
            std::string connection = "udp://" + address;
            check(zmq_bind(this->socket, connection.c_str()), "zmq_connect");
            check(zmq_join(this->socket, this->multicastGroupName.c_str()), "zmq_join");
        } else if (commType == CommType::TCP_P2P) {
            this->socket = zmq_socket(context, ZMQ_STREAM);
            std::string connection = "tcp://" + address;
            check(zmq_bind(this->socket, connection.c_str()), "zmq_connect");
        } else {
            std::cerr << "Subscriber: Got unknown CommType! " << std::endl;
        }
    }

    virtual ~Subscriber();

    template <class CallbackObjType>
    void subscribe(void (CallbackObjType::*callbackFunction)(::capnp::FlatArrayMessageReader&), CallbackObjType* callbackObject);

    void subscribe(void (*callbackFunction)(::capnp::FlatArrayMessageReader&));

    static const int wordSize;

    // ObjType *callbackObject_;
    // void (ObjType::*callbackFunction_)(MsgType::Reader);
    // callbackMemberFunction callbackMemberFunction_;
    callbackFunction callbackFunction_;

protected:
    void* socket;
    int rcvTimeout;
    std::string multicastGroupName;
    std::thread* runThread;
    bool running;

    void receive();
};

const int Subscriber::wordSize = sizeof(capnp::word);

Subscriber::~Subscriber()
{
    this->running = false;
    this->runThread->join();
    delete this->runThread;
    check(zmq_close(this->socket), "zmq_close");
}

template <class CallbackObjType>
void Subscriber::subscribe(void (CallbackObjType::*callbackFunction)(::capnp::FlatArrayMessageReader&), CallbackObjType* callbackObject)
{
    using std::placeholders::_1;
    this->callbackFunction_ = std::bind(callbackFunction, callbackObject, _1);
    this->runThread = new std::thread(&Subscriber::receive, this);
}

void Subscriber::subscribe(void (*callbackFunction)(::capnp::FlatArrayMessageReader&))
{
    this->callbackFunction_ = callbackFunction;
    this->runThread = new std::thread(&Subscriber::receive, this);
}

void Subscriber::receive()
{
    while (this->running) {
        zmq_msg_t msg;
        check(zmq_msg_init(&msg), "zmq_msg_init");
//        std::cout << "Subscriber::received() waiting ..." << std::endl;
        int nbytes = zmq_msg_recv(&msg, this->socket, 0);

//        std::cout << "Subscriber::receive(): nBytes: " << nbytes << " errno: " << errno << "(EAGAIN: " << EAGAIN << ")" << std::endl;

        // handling for unsuccessful call to zmq_msg_recv
        if (nbytes == -1) {
            if (errno != EAGAIN) // receiving a message was unsuccessful
            {
                std::cerr << "Subscriber::receive(): zmq_msg_recv returned: -1 - " << zmq_strerror(errno) << std::endl;
            } else // no message available
            {
//                std::cout << "Subscriber::receive(): continue because of EAGAIN!" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            check(zmq_msg_close(&msg), "zmq_msg_close");
            continue;
        }

        // Received message must contain an integral number of words.
        if (zmq_msg_size(&msg) % Subscriber::wordSize != 0) {
            std::cout << "Non-Integral number of words!" << std::endl;
            check(zmq_msg_close(&msg), "zmq_msg_close");
            continue;
        }

        // Check whether message is memory aligned
        assert(reinterpret_cast<uintptr_t>(zmq_msg_data(&msg)) % Subscriber::wordSize == 0);

        int numWordsInMsg = zmq_msg_size(&msg);
        auto wordArray = kj::ArrayPtr<capnp::word const>(reinterpret_cast<capnp::word const*>(zmq_msg_data(&msg)), numWordsInMsg);

        ::capnp::FlatArrayMessageReader msgReader = ::capnp::FlatArrayMessageReader(wordArray);

        (this->callbackFunction_)(msgReader);

        check(zmq_msg_close(&msg), "zmq_msg_close");
    }
}
} // namespace capnzero
