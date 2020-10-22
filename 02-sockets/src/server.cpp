#include <asio.hpp>
#include <ctime>
#include <iostream>
#include <string>

using asio::ip::tcp;

int main() {
  asio::io_context io_context;
  tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 3000));

  int counter = 0;

  while (true) {
    // Wait for client (acceptor.accept is blocking)
    std::cout << "Blocked for read" << std::endl;
    tcp::socket socket(io_context);
    acceptor.accept(socket);

    // Create a buf and read into it
    std::array<char, 128> buf;
    asio::error_code error;
    size_t len = socket.read_some(asio::buffer(buf), error);

    // Get the first byte from buf, convert to int, and add to counter
    int x = int(buf[0]);
    counter += x;

    // Convert counter to std::string message and send back
    std::string message = std::to_string(counter) + "\n";
    asio::write(socket, asio::buffer(message), error);

    // Example of error handling
    // if (error != asio::error::eof)
    //   throw asio::system_error(error);
  }

  return 0;
}