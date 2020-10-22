#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

int main() {
  // Open a socket to localhost (your machine) on port 3000
  asio::io_context io_context;
  tcp::resolver resolver(io_context);
  tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "3000");
  tcp::socket socket(io_context);
  asio::connect(socket, endpoints);

  // Used to handle the many possible errors (we are ignoring them here..)
  asio::error_code error;

  // Create a buffer used for sending and receiving data
  std::array<char, 128> buf;

  // Set the 0th byte in buf to 2
  buf[0] = 0b10;

  // Send the buf to the server
  asio::write(socket, asio::buffer(buf), error);

  // Wait for a response. len will be the number of bytes in the response
  size_t len = socket.read_some(asio::buffer(buf), error);

  // cout the buf up to len (the rest is garbage)
  std::cout.write(buf.data(), len);

  return 0;
}