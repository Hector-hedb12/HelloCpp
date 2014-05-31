#include "position.h"
#include <cmath>
position::position(){
  x = y = 0;
}
position::position(int a, int b){
  x = a; y = b;
}
position position::operator+(const position &b){
  position res;
  res.x = x + b.x;
  res.y = y + b.y;
  return res;
}

position& position::operator=(position const &b){
  x = b.x;
  y = b.y;
  return *this;
}
bool position::operator<(const position &p) const{
  return x < p.x || (x == p.x && y < p.y);
}
bool position::operator==(const position &p) const{
  return x == p.x && y == p.y;
}
bool position::operator!=(const position &p) const{
	return !(*this == p);
}
void position::p(){
  cout << "(" << x << "," << y << ")" << endl; 
}
position position::next(int a){
  position res;
  assert(a >= 0);
  res.x = x + vx1[a%4];
  res.y = y + vy1[a%4];
  return res;
}

int position::distManhattan(const position &p) const{
	return abs(x-p.x)+ abs(y-p.y);
}


void position::t(){
  x += MAPMID+1;
  y += MAPMID+1;
}

void position::invT(){
  x -= (MAPMID+1);
  y -= (MAPMID+1);
}

ostream &operator<<(ostream &os, const position &p) {
  os << "(" << p.x << "," << p.y << ")"; 
}
