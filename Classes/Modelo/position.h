#ifndef  _POSITION_H_
#define  _POSITION_H_

#include <iostream>
#include <cassert>
#include "constant.h"
using namespace std;

/*
 * Class Position
 */
class position{
public:
  int x, y;
  /*
   * return (0,0)
   */
  position();
  /*
   * return pair (a,b)
   */
  position(int a, int b);
  /*
   * Sum operator
   */
  position operator+(const position &b);
  /*
   * Asign operator
   */
  position& operator=(position const &b);
  /*
   * Print in the stdout the pair
   */
  void p();
  /*
   * Get the next position
   */
  position next(int a);
  /*
   * Operator <
   */
  bool operator<(const position &p) const;
  /*
   * Return true if the pairs are equals
   */
  bool operator==(const position &p) const;
  /*
   * Return true if the pairs are equals
   */
  bool operator!=(const position &p) const;
  /*
   * Transform the pair
   */
  void t();
  
  void invT();

};

#endif
