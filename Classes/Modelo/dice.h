#ifndef  _DICE_H_
#define  _DICE_H_

/* 
 * Class Dice:
 * Pseudo Random 0 - 5
 */

#include <random>
#include <unordered_map>


using namespace std;

class dice{
int x;
default_random_engine generator;
uniform_int_distribution<int> d1;
public:
  dice(int s = 0){
	  generator = default_random_engine(rand());
	  d1 = uniform_int_distribution<int>(0,5);
	  next();
  }
  int recent(){
    return x;
  }
  int next(){
    return x = d1(generator);
  }  
};

#endif
