#include <asio.hpp>
#include <iostream>
#include <queue>

#include "../include/sender.hpp"

using asio::ip::tcp;

int main() {
  asio::io_context io_context;
  Sender sender(io_context, "127.0.0.1", "3000");

  // An explanation of the API
  // data_ready(): returns true if a message is available, otherwise false
  // get_msg(): gets an available message
  // request_msg(id): requests a message at id [0, NUM_MSGS)

  // You will need to make significant modifications to the logic below
  // As a starting point, run the server as follows:
  // ./server --no-delay --no-packet-drops which makes the below code work

  // As an example, you could start by requesting the first 10 messages
  std::string poem = "";
  int32_t curr_msg = 0;
  std::queue <int32_t> q;
  for (int i = 0; i < 852; i+=2) {
    //std::cout << "semt " << std::endl;
    sender.request_msg(i);
    sender.request_msg(i+1);
    q.push(i);
    q.push(i+1);
    curr_msg+=2;
    if(sender.data_ready()){
      auto msg0 = sender.get_msg();
      auto msg1 = sender.get_msg();
      if(q.front() == msg0.msg_id){
        auto data_str0 = std::string(msg0.data.data(), CHUNK_SIZE);
        auto data_str1 = std::string(msg1.data.data(), CHUNK_SIZE);
        poem += data_str0 + data_str1;
        std::cout << "msg_id(" << msg0.msg_id << ")::" << data_str0 << std::endl;
        std::cout << "msg_id(" << msg1.msg_id << ")::" << data_str1 << std::endl;
      }
      else{
        auto data_str0 = std::string(msg1.data.data(), CHUNK_SIZE);
        auto data_str1 = std::string(msg0.data.data(), CHUNK_SIZE);
        poem += data_str0 + data_str1;
        std::cout << "msg_id(" << msg1.msg_id << ")::" << data_str0 << std::endl;
        std::cout << "msg_id(" << msg0.msg_id << ")::" << data_str1 << std::endl;
      }

      

      
    }
  }


/*
  while (true) {
    if (sender.data_ready()) {
      // Get a response Msg:
      // A Msg has a msg_id (corresponds to id in request_msg) and
      // a char array of CHUNK_SIZE (128) storing the data
      auto msg = sender.get_msg();

      // Eventually, you will combine these chunks to write the file
      auto data_str = std::string(msg.data.data(), CHUNK_SIZE);

      // Print the msg id and message recieved (may be out of order)
      std::cout << "msg_id(" << msg.msg_id << ")::" << data_str << std::endl;
    }
  }
  */

  return 0;
}
