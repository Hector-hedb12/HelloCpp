/*
 * decision.h
 *
 *  Created on: May 30, 2014
 *      Author: jose
 */

#ifndef DECISION_H_
#define DECISION_H_


#include "state.h"
#include "strategy.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>

class decision{

	map<position,int> getmap(set<position> se, map<position, int> dist){
		map<position, int> res;
		for(set<position>::iterator it = se.begin(); it != se.end(); it++){
			res[dist[*it]]++;
		}
		return res;
	}

public:
	position movement(state gs){
		position res;
		machine &us;
		map<position,int> dist, dor;
		map<int,int> dl,db,dz,dp, dh, dd;
		bool endCard;
		pair<vector<position>,vector<position> > pa;

		assert(gs.getCurrentPlayerInstance().isMachine());
		us  = (machine &)gs.getCurrentPlayerInstance();

		endCard = gs.world.hasEndCard();


		pa = gs.getPossibleMoves();
		pa.first.push_back(gs.getCurrentPlayerPosition());

		strategy st = us.getMoveStrategy();

		res = pa.first[0];
		double maxf = -1e9;

		dor =getDistances(gs.getCurrentPlayerPosition());

		for(int i = 0; i < pa.first.size(); i++){
			/* Calculate distances */
			dp.clear(); dl.clear(); db.clear(); dz.clear(); dh.clear(); dd.clear();
			dist = getDistances(pa.first[i]);
			for(int i = 0; i < gs.world.playerVector.size(); i++){
				if(i == gs.getCurrentPlayer()) continue;
				dp[dist[gs.world.playerVector[i]]]++;
			}
			dl = getmap(gs.world.lifeSet, dist);
			db = getmap(gs.world.bulletSet, dist);
			dz = getmap(gs.world.zombieSet, dist);
			if(endCard){
				dh[dist[gs.world.endPosition]]++;
			}
			dd[dor[pa.first[i]]]++;
			/* End Calculate distances */
			if(maxf < st.f(dl, db, dz, dp, dh, dd)){
				res = pa.first[i];
			}
		}
		return res;
	}


	position putmapcard(state gs, int player = -1){
		if(player == -1) player = gs.getCurrentPlayer();

		vector<vector<position> > ve = gs.getAllPosibleMapCard(gs.lastMapCard);
		vector<int>vi;
		for(int i = 0; i < 4; i++) vi.push_back(i);
		random_sort(vi.begin(), vi.end());
		for(int i = 0; i < 4; i++){
			if(ve[vi[i]].empty()) continue;

		}



	}


};




#endif /* DECISION_H_ */
