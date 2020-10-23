#include <chrono>
#include <iostream>
#include "../include/bakery.hpp"
using namespace std::chrono;



int main() {
  auto start = high_resolution_clock::now();
  auto bakery = text_deserializer("../data/small.txt");
  auto deser_time = high_resolution_clock::now() - start;
  std::cout << "Text deserializer took: "
            << duration_cast<milliseconds>(deser_time).count() << "ms"
            << std::endl
            << std::endl;
  //print_bakery(bakery);

  
  //text_serializer
  start = high_resolution_clock::now();
  text_serializer(bakery, "hello");
  deser_time = high_resolution_clock::now() - start;
  std::cout << "Text serializer took: "
            << duration_cast<milliseconds>(deser_time).count() << "ms"
            << std::endl
            << std::endl;

  //binary serializer
  start = high_resolution_clock::now();
  binary_serializer(bakery, "hello2");
  deser_time = high_resolution_clock::now() - start;
  std::cout << "binary text serializer took: "
            << duration_cast<milliseconds>(deser_time).count() << "ms"
            << std::endl
            << std::endl;

  binary_deserializer("../build/hello2");

  return 0;
}