#ifndef __TOURNAMENT_H__
#define __TOURNAMENT_H__

#include "Players.h"
#include "ConsoleGrid.h"
#include <vector>

using namespace std;


class Tournament {
  public:
	Tournament(vector<Strategy> strategies) : _strategies(strategies), _grid(0, 0) {}
	~Tournament();

	void playTournament();

  private:
	vector<Strategy> _strategies;
	vector<Player *> _players;
	vector<double> _maxProportions;
	int _cumulativeScores[StrategyCount] = {};
	ConsoleGrid _grid;

	void playRound(int roundNumber);
	void printCurrentPopulations(int roundNumber);
	void playMatch(Player *playerA, Player *playerB);

	Move distortedMove(Move move);
};

#endif
