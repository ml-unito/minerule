#include <iostream>

class A {
private:
  A() {}
  A(const A& b) {}
  static A a;
public:
  ~A() {}
  A& getA() {return a;}
};

A A::a;

int main() {
  
}

