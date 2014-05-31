/*
 * strategy.h
 *
 *  Created on: May 30, 2014
 *      Author: jose
 */

#ifndef STRATEGY_H_
#define STRATEGY_H_


#include <map>

class strategy{

};

class move_strategy:public strategy{
protected:
	static vector<double> dv;

	static double fac(int i){
		int siz = dv.size();
		if(siz == 0){
			dv.push_back(1);
		}
		for(int k = siz; k < i; k++){
			dv[k] = dv[k-1]/2;
		}
		if(dv.size() > i){
			return dv[i];
		}
	}

	double c_l;
	double c_b;
	double c_z;
	double c_p;
	double c_h;
	double c_d;

	double d(map<int,int> ma){
		double res = 0;
		for(map<int,int>::iterator it = ma.begin(); it != ma.end(); it++){
			res = res + fac(it->first)*it->second;
		}
		return res;
	}

public:
	move_strategy(int l = 1, int b= 1, int z= 1, int p = 0, int h = 1, int d = -1){
		c_l = l;
		c_b = b;
		c_z = z;
		c_p = p;
		c_h = h;
		c_d = d;
	}


	void init();
	double f(map<int,int> ml,map<int,int>mb,map<int,int>mz,
			map<int,int>mp, map<int,int>mh, map<int,int> md){
		return c_l*d(ml) + c_b*d(mb) + c_z*d(mz) + c_p*d(mp) + c_h*d(mh) + c_d*d(md);
	}

};



#endif /* STRATEGY_H_ */
