#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

int main() {
  asio::io_context io_context;
  tcp::resolver resolver(io_context);
  tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "3000");

  tcp::socket socket(io_context);
  asio::connect(socket, endpoints);

  asio::error_code error;
  std::array<uint8_t, 3> buf;
  
  // get the number of orders for claudia
  buf.fill(0);
  buf[2] = 00000101;
  asio::write(socket, asio::buffer(buf), error);
  size_t len = socket.read_some(asio::buffer(buf), error);
  uint16_t response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
  std::cout << response << std::endl;

  //add an order E=01, Q1=2, Q2=5, Q3=0, Q4=1, Q5=0
  buf.fill(0);
  buf[2] = 00100110;
  buf[1] = 00000101;
  buf[0] = 00000001;
  asio::write(socket, asio::buffer(buf), error);
  len = socket.read_some(asio::buffer(buf), error);
  response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
  std::cout << response << std::endl;

  //Get the number of orders for E=01
  buf.fill(0);
  buf[2] = 00000101;
  asio::write(socket, asio::buffer(buf), error);
  len = socket.read_some(asio::buffer(buf), error);
  response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
  std::cout << response << std::endl;

  //Get the total number of orders
  buf.fill(0);
  buf[2] = 00001100;
  asio::write(socket, asio::buffer(buf), error);
  len = socket.read_some(asio::buffer(buf), error);
  response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
  std::cout << response << std::endl;
  

  return 0;
}