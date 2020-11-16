#include <chrono>
#include <fstream>
#include <iostream>

#include "../include/manager.hpp"
#include "../include/router.hpp"

int main() {
  void client_server(Manager &m);
  void ring(Manager &m);
  void complex(Manager &m);
  Manager m1,m2,m3;
  client_server(m1);
  ring(m2);
  complex(m3);
  return 0;
}

void client_server(Manager &m){
    Router r1(1, &m, {{2, 1000},{3, 1000},{4, 1000},{5, 1000}});
    Router r2(2, &m, {{1, 1000}});
    Router r3(3, &m, {{1, 1000}});
    Router r4(4, &m, {{1, 1000}});
    Router r5(5, &m, {{1, 1000}});

    r1.set_forwarding_tables({{2, 2}, {3, 3}, {4, 4}, {5, 5}});
    r2.set_forwarding_tables({{1, 1}, {3, 1}, {4, 1}, {5, 1}});
    r3.set_forwarding_tables({{1, 1}, {2, 1}, {4, 1}, {5, 1}});
    r4.set_forwarding_tables({{1, 1}, {2, 1}, {3, 1}, {5, 1}});
    r5.set_forwarding_tables({{1, 1}, {2, 1}, {3, 1}, {4, 1}});

    m.routers.push_back(r1);
    m.routers.push_back(r2);
    m.routers.push_back(r3);
    m.routers.push_back(r4);
    m.routers.push_back(r5);

    auto start = std::chrono::system_clock::now();
    m.add_message(2, 1);
    m.add_message(5, 1);
    m.add_message(4, 1);
    while (!m.finished()) {
      m.tick();
    }
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 
    std::cout << "time for client_server is " << duration << " milliseconds\n";
};

void ring(Manager &m){
    Router r1(1, &m, {{2, 100},{8, 100}});
    Router r2(2, &m, {{1, 100},{3, 100}});
    Router r3(3, &m, {{2, 100},{4, 100}});
    Router r4(4, &m, {{3, 100},{5, 100}});
    Router r5(5, &m, {{4, 100},{6, 100}});
    Router r6(6, &m, {{5, 100},{7, 5000}});
    Router r7(7, &m, {{6, 5000},{8, 100}});
    Router r8(8, &m, {{7, 100},{1, 100}});

    r1.set_forwarding_tables({{2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 8}, {8, 8}});
    r2.set_forwarding_tables({{1, 1}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 1}, {8, 1}});
    r3.set_forwarding_tables({{1, 2}, {2, 2}, {4, 4}, {5, 4}, {6, 4}, {7, 2}, {8, 2}});
    r4.set_forwarding_tables({{1, 3}, {2, 3}, {3, 3}, {5, 5}, {6, 5}, {7, 3}, {8, 3}});
    r5.set_forwarding_tables({{1, 4}, {2, 4}, {3, 4}, {4, 4}, {6, 6}, {7, 4}, {8, 4}});
    r6.set_forwarding_tables({{1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {7, 5}, {8, 5}});
    r7.set_forwarding_tables({{1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {8, 8}});
    r8.set_forwarding_tables({{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 7}});
    
    m.routers.push_back(r1);
    m.routers.push_back(r2);
    m.routers.push_back(r3);
    m.routers.push_back(r4);
    m.routers.push_back(r5);
    m.routers.push_back(r6);
    m.routers.push_back(r7);
    m.routers.push_back(r8);


    auto start = std::chrono::system_clock::now();
    m.add_message(1, 3);
    m.add_message(8, 4);
    m.add_message(6, 7);
    while (!m.finished()) {
      m.tick();
    }
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 
    std::cout << "time for ring topology is " << duration << " milliseconds\n";
  };


void complex(Manager &m){
    Router r1(1, &m, {{5, 1200},{2, 300}});
    Router r2(2, &m, {{1, 300},{3, 800},{7, 500}});
    Router r3(3, &m, {{2, 800},{6, 1500}, {7, 2000}, {8, 900}});
    Router r4(4, &m, {{7, 300}});
    Router r5(5, &m, {{1, 1200},{6, 600}});
    Router r6(6, &m, {{3, 1500},{5, 600},{9, 200}});
    Router r7(7, &m, {{2, 500},{3, 2000}, {4, 300}});
    Router r8(8, &m, {{3, 900},{9, 400}});
    Router r9(9, &m, {{6, 200},{8, 400}});

    r1.set_forwarding_tables({{2, 2}, {3, 2}, {4, 2}, {5, 5}, {6, 5}, {7, 2}, {8, 3}, {9, 5}});
    r2.set_forwarding_tables({{1, 1}, {3, 3}, {4, 7}, {5, 1}, {6, 1}, {7, 7}, {8, 3}, {9, 3}});
    r3.set_forwarding_tables({{1, 2}, {2, 2}, {4, 2}, {5, 6}, {6, 6}, {7, 2}, {8, 8}, {9, 8}});
    r4.set_forwarding_tables({{1, 7}, {2, 7}, {3, 7}, {5, 7}, {6, 7}, {7, 7}, {8, 7}, {9, 7}});
    r5.set_forwarding_tables({{1, 1}, {2, 1}, {3, 6}, {4, 1}, {6, 6}, {7, 1}, {8, 6}, {9, 6}});
    r6.set_forwarding_tables({{1, 5}, {2, 5}, {3, 3}, {4, 5}, {5, 5}, {7, 5}, {8, 9}, {9, 9}});
    r7.set_forwarding_tables({{1, 2}, {2, 2}, {3, 2}, {4, 4}, {5, 2}, {6, 2}, {8, 2}, {9, 2}});
    r8.set_forwarding_tables({{1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 9}, {6, 9}, {7, 3}, {9, 9}});
    r9.set_forwarding_tables({{1, 6}, {2, 8}, {3, 8}, {4, 6}, {5, 6}, {6, 6}, {7, 6}, {8, 8}});

    m.routers.push_back(r1);
    m.routers.push_back(r2);
    m.routers.push_back(r3);
    m.routers.push_back(r4);
    m.routers.push_back(r5);
    m.routers.push_back(r6);
    m.routers.push_back(r7);
    m.routers.push_back(r8);
    m.routers.push_back(r9);

    auto start = std::chrono::system_clock::now();
    m.add_message(8, 6);
    m.add_message(1, 9);
    m.add_message(4, 5);
    while (!m.finished()) {
      m.tick();
    }
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 
    std::cout << "time for complex topology is " << duration << " milliseconds\n";
}