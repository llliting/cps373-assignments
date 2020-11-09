#include <asio.hpp>
#include <iostream>
#include <chrono>
#include <fstream>

#include "../include/sender.hpp"

using asio::ip::tcp;

int main() {
  asio::io_context io_context;
  Sender sender(io_context, "127.0.0.1", "3000");
  std::string* ptr = new std::string[852];
  std::chrono::system_clock::time_point* timeRec = new std::chrono::system_clock::time_point[852];
  
  int i = 0;

  while(i<=852){
    //send every other five packets 
    if(i == 852){
      sender.request_msg(852);
      timeRec[852] = std::chrono::system_clock::now();
      i = 853;
    }
    else{
      sender.request_msg(i);
      timeRec[i] = std::chrono::system_clock::now();
      sender.request_msg(i+1);
      timeRec[i+1] = std::chrono::system_clock::now();
      i+=2;
    }

    std::this_thread::sleep_for (std::chrono::milliseconds(10)); 
    //you told me to use 1 millisecond in the email, but I still cannot get all messages so I changed this to 10 
    
    //read msg
    int successCount = 0;
    for(int k = 0; k < 2; k ++){
      if (sender.data_ready()) {
        auto msg = sender.get_msg();
        auto receive = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(receive - timeRec[msg.msg_id]).count();
        //timeout
        if(duration < 1000){
          auto data_str = std::string(msg.data.data(), CHUNK_SIZE);
          ptr[(int)msg.msg_id] = data_str;   
          successCount++;    
        }
        else{
          i = (int)msg.msg_id;
          break;
        }
      }
    }
    if(successCount!= 2){//resend the five packets if any one of them dropped
      i -=2;
    } 
  }

  // std::ofstream output;
  // output.open ("output.txt");
  // for(int i = 0; i < 853; i ++){
  //   output << ptr[i];
  // }
  // output.close();
  delete [] ptr;

  return 0;
}
