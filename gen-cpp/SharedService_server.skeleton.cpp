// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "SharedService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::shared;

class SharedServiceHandler : virtual public SharedServiceIf {
 public:
  SharedServiceHandler() {
    // Your initialization goes here
  }

  void getStruct(SharedStruct& _return, const int32_t key) {
    // Your implementation goes here
    printf("getStruct\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  ::std::shared_ptr<SharedServiceHandler> handler(new SharedServiceHandler());
  ::std::shared_ptr<TProcessor> processor(new SharedServiceProcessor(handler));
  ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

