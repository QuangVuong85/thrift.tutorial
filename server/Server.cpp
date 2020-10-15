#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <iostream>
#include <stdexcept>

#include "../gen-cpp/Calculator.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace tutorial;
using namespace shared;

class CalculatorHandler : public CalculatorIf {
public:
    CalculatorHandler() = default;

    void ping() override { cout << "ping()" << endl; }

    int32_t add(const int32_t n1, const int32_t n2) override {
        cout << "add(" << n1 << ", " << n2 << ")" << endl;
        return n1 + n2;
    }

    int32_t calculate(const int32_t logid, const Work &work) override {
        cout << "calculate(" << logid << ", " << work << ")" << endl;
        int32_t val;

        switch (work.op) {
            case Operation::ADD:
                val = work.num1 + work.num2;
                break;
            case Operation::SUBTRACT:
                val = work.num1 - work.num2;
                break;
            case Operation::MULTIPLY:
                val = work.num1 * work.num2;
                break;
            case Operation::DIVIDE:
                if (work.num2 == 0) {
                    InvalidOperation io;
                    io.whatOp = work.op;
                    io.why = "Cannot divide by 0";
                    throw io;
                }
                val = work.num1 / work.num2;
                break;
            default:
                InvalidOperation io;
                io.whatOp = work.op;
                io.why = "Invalid Operation";
                throw io;
        }

        SharedStruct ss;
        ss.key = logid;
        ss.value = to_string(val);

        log[logid] = ss;

        return val;
    }

    void getStruct(SharedStruct &ret, const int32_t logid) override {
        cout << "getStruct(" << logid << ")" << endl;
        ret = log[logid];
    }

    void zip() override { cout << "zip()" << endl; }

protected:
    map<int32_t, SharedStruct> log;
};


class CalculatorCloneFactory : virtual public CalculatorIfFactory {
public:
    ~CalculatorCloneFactory() override = default;

    CalculatorIf *getHandler(const ::apache::thrift::TConnectionInfo &connInfo) override {
        std::shared_ptr<TSocket> sock = std::dynamic_pointer_cast<TSocket>(connInfo.transport);
        cout << "Incoming connection\n";
        cout << "\tSocketInfo: " << sock->getSocketInfo() << "\n";
        cout << "\tPeerHost: " << sock->getPeerHost() << "\n";
        cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
        cout << "\tPeerPort: " << sock->getPeerPort() << "\n";
        return new CalculatorHandler;
    }

    void releaseHandler(::shared::SharedServiceIf *handler) override {
        delete handler;
    }
};

int main() {
    TThreadedServer server(
            std::make_shared<CalculatorProcessorFactory>(std::make_shared<CalculatorCloneFactory>()),
            std::make_shared<TServerSocket>(9999),
            std::make_shared<TBufferedTransportFactory>(),
            std::make_shared<TBinaryProtocolFactory>());

    /*
    // if you don't need per-connection state, do the following instead
    TThreadedServer server(
      std::make_shared<CalculatorProcessor>(std::make_shared<CalculatorHandler>()),
      std::make_shared<TServerSocket>(9090), //port
      std::make_shared<TBufferedTransportFactory>(),
      std::make_shared<TBinaryProtocolFactory>());
    */

    /**
     * Here are some alternate server types...

    // This server only allows one connection at a time, but spawns no threads
    TSimpleServer server(
      std::make_shared<CalculatorProcessor>(std::make_shared<CalculatorHandler>()),
      std::make_shared<TServerSocket>(9090),
      std::make_shared<TBufferedTransportFactory>(),
      std::make_shared<TBinaryProtocolFactory>());

    const int workerCount = 4;

    std::shared_ptr<ThreadManager> threadManager =
      ThreadManager::newSimpleThreadManager(workerCount);
    threadManager->threadFactory(
      std::make_shared<ThreadFactory>());
    threadManager->start();

    // This server allows "workerCount" connection at a time, and reuses threads
    TThreadPoolServer server(
      std::make_shared<CalculatorProcessorFactory>(std::make_shared<CalculatorCloneFactory>()),
      std::make_shared<TServerSocket>(9090),
      std::make_shared<TBufferedTransportFactory>(),
      std::make_shared<TBinaryProtocolFactory>(),
      threadManager);
    */

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
    return 0;
}