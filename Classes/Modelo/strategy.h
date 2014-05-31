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

class moveStrategy:public strategy{
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
	moveStrategy(){
		init();
	}
	void init(){
		c_l = 1;
		c_b = 1;
		c_z = 1;
		c_p = 0;
		c_h = 1;
		c_d = -1;
	}
	double f(map<int,int> ml,map<int,int>mb,map<int,int>mz,
			map<int,int>mp, map<int,int>mh, map<int,int> md){
		return c_l*d(ml) + c_b*d(mb) + c_z*d(mz) + c_p*d(mp) + c_h*d(mh) + c_d*d(md);
	}

};



class strategyBotherPlayer: public moveStrategy{
	void init(){
		c_l = 10;
		c_b = 10;
		c_z = 2;
		c_p = 20;
		c_h = 50;
		c_d = 0;
	}
};

class strategyGetZombie: public moveStrategy{
	void init(){
		c_l = 10;
		c_b = 10;
		c_z = 20;
		c_p = -1;
		c_h = 1;
		c_d = -5;
	}
};

class strategyGetResources: public moveStrategy{
	void init(){
		c_l = 20;
		c_b = 20;
		c_z = -3;
		c_p = -3;
		c_h = 100;
		c_d = 0;
	}
};

class strategyGetBullets: public moveStrategy{
	void init(){
		c_l = 3;
		c_b = 20;
		c_z = -3;
		c_p = -3;
		c_h = 100;
		c_d = 0;
	}
};

#endif /* STRATEGY_H_ */
