#include "Tournament.h"
#include "Utils.h"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>

static const int kMatchIterations = 20;
static const int kNoiseProbability = 3;
static const int kInitialPlayersPerStrategy = 50;
static const int kLowScorePopCount = 6;
static const int kNumberOfRounds = 80;

static const int kHistogramWidth = 70;

static const int kPayoutsAxelrod[2][2][2] = {{{1, 1}, {5, 0}},
											 {{0, 5}, {3, 3}}};

/*****************************************************************************/


Tournament::~Tournament() {
	for (unsigned i = 0; i < _players.size(); i++) {
		delete _players[i];
	}
}

void Tournament::playTournament() {
	if (_strategies.size() == 0)
		return;

	// create the initial population of players, kInitialPlayersPerStrategy
	for (unsigned i = 0; i < _strategies.size(); i++) {
		Strategy strategy = _strategies[i];
		// track the best normalized proportion for each strat
		_maxProportions.push_back(0);
		for (int j = 0; j < kInitialPlayersPerStrategy; j++) {
			_players.push_back(Player::createPlayer(strategy));
		}
	}

	clock_t startTime = clock();

	for (int round = 1; round <= kNumberOfRounds; round++) {
		playRound(round);

		for (unsigned i = 0; i < _players.size(); ++i) {
			Player *player = _players[i];
			_cumulativeScores[player->strategy()] += player->score();
		}
	}

	cout << "\nTournament took " << ((clock() - startTime) / CLOCKS_PER_SEC)
		 << " seconds.\n";

	cout << "\nCumulative scores:\n";
	for (int i = 0; i < StrategyCount; ++i) {
		cout << setw(15) << StrategyNames[i] << " " << setw(10) << right << _cumulativeScores[i] << endl;
	}
}


void Tournament::playRound(int roundNumber) {
	printCurrentPopulations(roundNumber);

	for (unsigned i = 0; i < _players.size(); i++) {
		_players[i]->setScore(0);
	}

	// in the round, every player plays every other, accumulating score
	for (unsigned i = 0; i < _players.size(); i++) {
		for (unsigned j = i; j < _players.size(); j++) {
			playMatch(_players[i], _players[j]);
		}
	}

	sort(_players.begin(), _players.end(),
		 [](Player *a, Player *b) { return a->score() > b->score(); });

	for (int i = 0; i < kLowScorePopCount; i++) {
		delete _players.back();
		_players.pop_back();
	}

	Strategy winningStrategy = _players[0]->strategy();
	for (int i = 0; i < kLowScorePopCount; i++) {
		Player *player = Player::createPlayer(winningStrategy);
		_players.push_back(player);
	}
}

void Tournament::printCurrentPopulations(int roundNumber) {
	int winningStrategy = 0;
	int strategyCounts[StrategyCount] = {};
	for (unsigned i = 0; i < _players.size(); i++) {
		Strategy playerStrategy = _players[i]->strategy();
		strategyCounts[playerStrategy] += 1;
		if (strategyCounts[playerStrategy] > strategyCounts[winningStrategy]) {
			winningStrategy = playerStrategy;
		}
	}
	int totalPopulation = kInitialPlayersPerStrategy * _strategies.size();

	_grid.eraseLog();
	cout << "Playing round " << roundNumber << endl << endl;

	for (int i = 0; i < StrategyCount; i++) {
		cout << setw(15) << StrategyNames[Strategy(i)];
		double proportion =
			totalPopulation ? (strategyCounts[i] / (double)totalPopulation) : 0;
		if (proportion > _maxProportions[i]) {
			_maxProportions[i] = proportion;
		}
		cout << std::setw(4) << static_cast<int>(round(proportion * 100))
			 << "%";

		int barCount = ceil(proportion * kHistogramWidth);
		int maxCount = ceil(_maxProportions[i] * kHistogramWidth);

		string bars(barCount, '=');
		string padding(max(0, maxCount - barCount), '-');

		cout << "  ";
		cout << bars;
		cout << _grid.vt100Color(blue);
		cout << padding + "]" << endl;
		cout << _grid.vt100Color(defaultColor);
	}
}

void Tournament::playMatch(Player *playerA, Player *playerB) {
	playerA->beginMatch();
	playerB->beginMatch();

	for (int i = 0; i < kMatchIterations; i++) {
		Move moveA = playerA->move();
		Move moveB = playerB->move();

		moveA = distortedMove(moveA);
		moveB = distortedMove(moveB);

		int payA = kPayoutsAxelrod[moveA][moveB][0];
		int payB = kPayoutsAxelrod[moveA][moveB][1];

		playerA->recordOpponentMove(moveB);
		playerA->incrementScore(payA);

		playerB->recordOpponentMove(moveA);
		playerB->incrementScore(payB);
	}
}

Move Tournament::distortedMove(Move move) {
	if (move != NoMove && GetRandom(0, 100) < kNoiseProbability) {
		return move == CooperateMove ? CheatMove : CooperateMove;
	}
	return move;
}
