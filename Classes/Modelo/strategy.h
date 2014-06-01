/*
 * strategy.h
 *
 *  Created on: May 30, 2014
 *      Author: jose
 */

#ifndef STRATEGY_H_
#define STRATEGY_H_


#include <map>
#include <vector>

using namespace std;

class strategy{
protected:
	vector<double> dv;
	double c_l,c_b,c_z,c_p,c_h,c_d;

	double c_zi, c_zu, c_zh;

	int val;
	double fac(int i);
	double d(map<int,int> ma);

public:
	//strategy& operator=(strategy const &b);
	strategy(int x = 0);
	void init(int x);
	double f(map<int,int> ml,map<int,int>mb,map<int,int>mz,
			map<int,int>mp, map<int,int>mh, map<int,int> md);
	double g(map<int,int> mi,map<int,int>mu,map<int,int>mh);
};





#endif /* STRATEGY_H_ */
