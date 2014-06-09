#include "strategy.h"

double strategy::fac(int i){
	int siz = dv.size();
	if(siz == 0){
		dv.push_back(1);
	}

	for(int k = dv.size(); k <= i; k++){
		dv.push_back(dv[k-1]/2);
	}


	if(dv.size() > i){
		return dv[i];
	}
}




double strategy::d(map<int,int> ma){
	double res = 0;
	for(map<int,int>::iterator it = ma.begin(); it != ma.end(); it++){
		res = res + fac(it->first)*it->second;
	}
	return res;
}
/*
strategy& strategy::operator=(strategy const &b){
  c_l = b.c_l;
  c_b = b.c_b;
  c_z = b.c_z;
  c_p = b.c_p;
  c_h = b.c_h;
  c_d = b.c_d;
  return *this;
}*/
strategy::strategy(int x){
	val = x;
	init(x);
}
void strategy::init(int x){
	if(x == 0){ // neutra
		c_l = 1;
		c_b = 1;
		c_z = 1;
		c_p = 0;
		c_h = 1;
		c_d = -1;

		c_zi = 0;
		c_zu = 1;
		c_zh = 0;

	}else if(x == 1){ // bother player
		c_l = 20;
		c_b = 20;
		c_z = 30;
		c_p = 20;
		c_h = 1000;
		c_d = 0;

		c_zi = -40;
		c_zu = 60;
		c_zh = -50;

	}else if(x == 2){ // kill zombies
		c_l = 20;
		c_b = 20;
		c_z = 50;
		c_p = -1;
		c_h = 5;
		c_d = -5;

		c_zi = 50;
		c_zu = -30;
		c_zh = 50;
	}else if(x == 3){ // Get Resources
		c_l = 20;
		c_b = 20;
		c_z = 2;
		c_p = -1;
		c_h = 1000;
		c_d = -2;

		c_zi = -50;
		c_zu = 30;
		c_zh = -30;
	}else if(x == 4){ // Get Bullets
		c_l = 3;
		c_b = 20;
		c_z = 2;
		c_p = -1;
		c_h = 1000;
		c_d = -2;

		c_zi = -50;
		c_zu = 30;
		c_zh = -30;
	}
}
double strategy::f(map<int,int> ml,map<int,int>mb,map<int,int>mz,
		map<int,int>mp, map<int,int>mh, map<int,int> md){
	return c_l*d(ml) + c_b*d(mb) + c_z*d(mz) + c_p*d(mp) + c_h*d(mh) + c_d*d(md);
}

double strategy::g(map<int,int> mi,map<int,int>mu,map<int,int>mh){
	return c_zi*d(mi) + c_zu*d(mu) + c_zh*d(mh);
}

