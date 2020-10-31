#include <inttypes.h>
#include <asio.hpp>
#include <ctime>
#include <iostream>
#include <string>


#include "bakery.hpp"

using asio::ip::tcp;


std::array<uint8_t, 2> respon(std::array<uint8_t, 3> buf, Bakery& bakery){
  std::array<uint8_t, 2> res;
  uint8_t command = buf[0]>>6;
  uint8_t e =buf[0]&0b00110000;
  std::string emp;
  if(e == 0b00)
    emp = "Brad";
  else if(e == 0b01)
    emp = "Claudia";
  else if(e == 0b10)  
    emp = "Simone";


  //get total number of orders 
  if(command == 0b00){
    if(bakery.orders.size() <= 511) {
      res[1] = bakery.orders.size();
    }
    else{
      uint16_t temp = bakery.orders.size();
      res[1] = 0b11111111;
      res[0] = (uint8_t)temp>>8;
    }
  }
  
  //get number of orders for employee
  else if(command == 0b01){
    int counter = 0;
    for(Order o : bakery.orders){
      if(o.employee == emp)
        counter ++;
    }
    if(counter <= 511) {
      res[1] = bakery.orders.size();
    }
    else{
      uint16_t temp = counter;
      res[1] = 0b11111111;
      res[0] = (uint8_t)temp>>8;
    }
  }

  //add an order 
  else if(command == 0b10){
    Order neworder;
    neworder.employee = emp;

    auto quantity = buf[0] & 00001111;
    if(quantity!=0)
      neworder.items.push_back(std::make_pair("Biscuit", std::to_string(quantity)));

    quantity = buf[1] >> 4;
    if(quantity!=0)
      neworder.items.push_back(std::make_pair("Bun", std::to_string(quantity)));

    quantity = buf[1] & 0b00001111;
    if(quantity!=0)
      neworder.items.push_back(std::make_pair("Brownie", std::to_string(quantity)));
      
    quantity = buf[2] >> 4;
    if(quantity!=0)
      neworder.items.push_back(std::make_pair("White Loaf", std::to_string(quantity)));

    quantity = buf[2] & 0b00001111;
    if(quantity!=0)
      neworder.items.push_back(std::make_pair("Wheat Loaf", std::to_string(quantity)));

    bakery.orders.push_back(neworder);
    res[0] = 0;
    res[1] = 0;
  }

  else{//unused
    res[0] = 0;
    res[1] = 0;
  }
  return res;
}



int main() {
  asio::io_context io_context;
  tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 3000));

  // Use this bakery to handle queries from the client
  Bakery bakery = text_deserializer("../data/bakery.txt");


  while (true) {
    // Wait for client
    std::cout << "Blocked for read" << std::endl;
    tcp::socket socket(io_context);
    acceptor.accept(socket);
    std::array<uint8_t, 3> buf;
    asio::error_code error;
    size_t len = socket.read_some(asio::buffer(buf), error);
    // Example of error handling
    // if (error != asio::error::eof)
    //   throw asio::system_error(error);
    std::array<uint8_t, 2> resp = respon(buf, bakery);
    asio::write(socket, asio::buffer(resp), error);
  }

  return 0;
}