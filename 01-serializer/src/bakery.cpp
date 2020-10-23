#include "bakery.hpp"
#include <fstream>
#include <iostream>
//#include <string>


using namespace std;



void print_bakery(const Bakery& bakery) {
  std::cout << "Employees: " << std::endl;
  for (auto employee : bakery.employees) {
    std::cout << employee << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Items: " << std::endl;
  for (auto item : bakery.items) {
    std::cout << item.name << ", " << item.price << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Orders: " << std::endl;
  for (auto order : bakery.orders) {
    std::cout << order.employee << ": ";
    auto j = 0;
    for (auto item : order.items) {
      std::cout << item.second << " " << item.first;
      j++;
      if (size_t(j) < order.items.size())
        std::cout << ", ";
    }
    std::cout << std::endl;
  }
}

// You shouldn't need to edit this function (unless you want!)
Bakery text_deserializer(std::string file_path) {
  std::ifstream infile(file_path);
  std::string line;
  Bakery bakery;

  while (!infile.eof()) {
    // Employees section
    if (line.compare("@employees") == 0) {
      std::getline(infile, line);
      while (line.size() > 0) {
        bakery.employees.push_back(line);
        std::getline(infile, line);
      }
    }

    // Items section
    if (line.compare("@items") == 0) {
      std::getline(infile, line);
      while (line.size() > 0) {
        auto end = line.find(", ");
        Item item;
        item.name = line.substr(0, end);
        item.price = line.substr(end + 2);
        bakery.items.push_back(item);
        std::getline(infile, line);
      }
    }

    // Orders section
    if (line.compare("@orders") == 0) {
      std::getline(infile, line);
      auto e = bakery.employees;
      while (line.size() > 0) {
        Order order;
        auto end = line.find(": ");
        order.employee = line.substr(0, end);

        // Find all the orders
        auto rest = line.substr(end + 2);
        size_t pos = 0;
        std::string token;
        while ((pos = rest.find(", ")) != std::string::npos) {
          token = rest.substr(0, pos);
          end = token.find(" ");
          auto quantity = token.substr(0, end);
          auto item_name = token.substr(end + 1);
          order.items.push_back(std::make_pair(item_name, quantity));
          rest.erase(0, pos + 2);
        }
        end = rest.find(" ");
        auto quantity = rest.substr(0, end);
        auto item_name = rest.substr(end + 1);
        order.items.push_back(std::make_pair(item_name, quantity));
        bakery.orders.push_back(order);

        // no more lines to read
        if (infile.eof())
          break;

        std::getline(infile, line);
      }
    }

    std::getline(infile, line);
  }

  return bakery;
}

// Implement these 3 functions!
// Hint: use print_bakery to help think about the text serializer
void text_serializer(const Bakery& bakery, std::string file_path){
  ofstream myfile;
  myfile.open (file_path);
  myfile << "Employees: " << std::endl;
  for (auto employee : bakery.employees) {
    myfile << employee << std::endl;
  }
  myfile << std::endl;
  myfile << "Items: " << std::endl;
  for (auto item : bakery.items) {
    myfile << item.name << ", " << item.price << std::endl;
  }
  myfile << std::endl;
  myfile << "Orders: " << std::endl;
  for (auto order : bakery.orders) {
    myfile << order.employee << ": ";
    auto j = 0;
    for (auto item : order.items) {
      myfile << item.second << " " << item.first;
      j++;
      if (size_t(j) < order.items.size())
        myfile << ", ";
    }
    myfile << std::endl;
  }
  myfile.close();
}




Bakery binary_deserializer(std::string file_path){
  ifstream rf(file_path, ifstream::binary);  
  string line;
  unordered_map<short, string> m;
  Bakery bakery;


  while (!rf.eof()) {
    // Employees section
    getline(rf, line);
    short i = 0;
    if (line.compare("Employees: ") == 0) {
      while (true) {
        getline(rf, line); 
        if(line.size()==0)
          break;
        m[i] = line;
        bakery.employees.push_back(line);
        i++;
      }
    }



/*
    // Items section
    if (line.compare("Items: ") == 0) {
      while (true){
        
        
        //cout << line.size() << "SIZE" << endl;
        if(line.size()==0)
          break;
        string na;
        string pr;
        Item item;
        rf.read(reinterpret_cast<char*>(&na), 11);
        rf.read(reinterpret_cast<char*>(&pr), 5);
        item.name = na;
        item.price = pr;
        bakery.items.push_back(item);
        m[i] = item.name;
        cout << na << ": " << pr << endl;
        getline(rf, line); 
        //cout <<  line << endl;
      }
    }
 */


    // Orders section
    if (line.compare("Orders: ") == 0) {
      while (true) {
        if(line.size()==0)
          break;
        Order order;
        short emp; 
        rf.read(reinterpret_cast<char*>(&emp), 2);
        cout << emp << " " << m[emp] <<  endl;
        
        
        order.employee = m[emp];
        
        
        auto quantity = token.substr(0, end);
        auto item_name = token.substr(end + 1);
        order.items.push_back(std::make_pair(item_name, quantity));

        getline(rf, line); 
        if (rf.eof())
          break;

        //std::getline(rf, line);
      }
    }

  
  }

  return bakery;






}


void binary_serializer(const Bakery& bakery, std::string file_path){
  ofstream os(file_path, ofstream::binary);
  unordered_map<string, short> m;
  //mapping the employees
  //char s[] = "Employees: \n";
  os << "Employees: \n";
  short i = 0;
  //string s;
  for (auto employee : bakery.employees) {
    m[employee] = i;
    os << employee; // employee's name
    os << endl;
    i++;
  }
  os << endl;
  //mapping the items
  os << "Items: \n";
  for (auto item : bakery.items) {
    m[item.name] = i;
    string n = item.name;
    string p = item.price;
    os.write(reinterpret_cast<char*>(&n),11);
    os.write(reinterpret_cast<char*>(&p), 5);
    //os.write(reinterpret_cast<char*>(&item), sizeof(Item));
    os << endl;
    i++;
  }
  os << endl;
  //writing orders
  char s2[] = "Orders: \n";
  os << s2;
  for (auto order : bakery.orders) {
    short idx = m[order.employee];
    os.write((char*)(&idx), sizeof (short));//first two bytes -- employee index 
    for (auto item : order.items) {
      short amount = (short)stoi(item.second); 
      os.write((char*)(&amount), sizeof (short));//first two bytes -- item amount 
      os.write((char*)(&(m[item.first])), sizeof (short));//next two bytes -- item name 
    }
    os << endl;
  }

  os.close();
}
