#include "Players.h"
#include "Tests.h"

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

/*****************************************************************************/

string MoveName(Move move) {
	switch (move) {
	case CheatMove:
		return "CheatMove";
	case CooperateMove:
		return "CooperateMove";
	default:
		return "NoMove";
	}
}

struct MovePair {
	Move playerMove = NoMove;
	Move opponentMove = NoMove;
};

int TestPlayer(Strategy strategy, string xName, vector<MovePair> moves) {
	int passedTestCount = 0;
	Player *player = Player::createPlayer(strategy);
	cout << "Testing: " << player->name() << endl;
	player->beginMatch();

	cout << "Should return strategy " << strategy;
	Strategy responseStrat = player->strategy();
	if (responseStrat == strategy) {
		cout << " ...confirmed" << endl;
	} else {
		cout << " ...assertion failed with " << responseStrat << endl;
		assert(false);
	}
	++passedTestCount;

	cout << "Should return name " << xName;
	string responseName = player->name();
	if (responseName == xName) {
		cout << " ...confirmed" << endl;
	} else {
		cout << " ...assertion failed with " << responseName << endl;
		assert(false);
	}
	++passedTestCount;

	for (int r = 0; r < moves.size(); r++) {
		Move playerMove = moves[r].playerMove;
		Move opponentMove = moves[r].opponentMove;
		Move actualPlayerMove = player->move();
		cout << r << " : player-" << MoveName(actualPlayerMove)
				<< ", opponent-" << MoveName(opponentMove);
		if (playerMove == actualPlayerMove) {
			cout << " ...confirmed" << endl;
		} else {
			cout << " ...assertion failed - expected player move: "
					<< MoveName(playerMove) << endl;
			assert(false);
		}
		++passedTestCount;

		player->recordOpponentMove(opponentMove);

		if (opponentMove == NoMove) {
			break;
		}
	}
	
	const int aScore = 4;
	player->incrementScore(aScore);

	cout << "Should increment score ";
	if (player->score() == aScore) {
		cout << " ...confirmed" << endl;
	} else {
		cout << " ...assertion failed" << endl;
		assert(false);
	}
	++passedTestCount;

	cout << "Should _not_ reset score ";
	player->beginMatch();
	if (player->score() == aScore) {
		cout << " ...confirmed" << endl;
	} else {
		cout << " ...assertion failed" << endl;
		assert(false);
	}
	++passedTestCount;


	cout << xName << " tests passed." << endl << endl;
	delete player;
	return passedTestCount;
}

/*****************************************************************************/


int TestCooperator() {
	vector<MovePair> moves = {
		{.playerMove = CooperateMove, .opponentMove = CooperateMove},
		{.playerMove = CooperateMove, .opponentMove = CheatMove},
		{.playerMove = CooperateMove, .opponentMove = NoMove},
	};

	return TestPlayer(CooperatorStrategy, "Cooperator", moves);
}

int TestCheater() {
	vector<MovePair> moves = {
		{.playerMove = CheatMove, .opponentMove = CooperateMove},
		{.playerMove = CheatMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = NoMove},
	};

	return TestPlayer(CheaterStrategy, "Cheater", moves);
}

int TestRandom() {
	vector<MovePair> moves = {};

	return TestPlayer(RandomStrategy, "Random", moves);
}


int TestGrudger() {
	vector<MovePair> moves = {
		{.playerMove = CooperateMove, .opponentMove = CooperateMove},
		{.playerMove = CooperateMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = CooperateMove},
		{.playerMove = CheatMove, .opponentMove = CooperateMove},
		{.playerMove = CheatMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = NoMove},
	};

	return TestPlayer(GrudgerStrategy, "Grudger", moves);
}

int TestTit4Tat() {
	vector<MovePair> moves = {
		{.playerMove = CooperateMove, .opponentMove = CooperateMove},
		{.playerMove = CooperateMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = CooperateMove},
		{.playerMove = CooperateMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = NoMove},
	};

	return TestPlayer(Tit4TatStrategy, "Tit4Tat", moves);
}

int TestTit4TatKitten() {
	vector<MovePair> moves = {
		{.playerMove = CooperateMove, .opponentMove = CooperateMove},
		{.playerMove = CooperateMove, .opponentMove = CheatMove},
		{.playerMove = CooperateMove, .opponentMove = CooperateMove},
		{.playerMove = CooperateMove, .opponentMove = CheatMove},
		{.playerMove = CooperateMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = CooperateMove},
		{.playerMove = CooperateMove, .opponentMove = NoMove},
	};

	return TestPlayer(Tit4TatKittenStrategy, "Tit4TatKitten", moves);
}


int TestDetective() {
	int testCount = 0;
	vector<MovePair> movesA = {
		{ .playerMove = CooperateMove, .opponentMove = CooperateMove},
		{ .playerMove = CheatMove, .opponentMove = CooperateMove},
		{ .playerMove = CooperateMove, .opponentMove = CooperateMove},
		{ .playerMove = CooperateMove, .opponentMove = CooperateMove},
		// opponent cooperated throughout the opening sequence. expect tit4tat
		{.playerMove = CooperateMove, .opponentMove = CooperateMove},
		{.playerMove = CooperateMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = CooperateMove},
		{.playerMove = CooperateMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = NoMove},
	};
	
	vector<MovePair> movesB = {
		{ .playerMove = CooperateMove, .opponentMove = CheatMove},
		{ .playerMove = CheatMove, .opponentMove = CooperateMove},
		{ .playerMove = CooperateMove, .opponentMove = CooperateMove},
		{ .playerMove = CooperateMove, .opponentMove = CooperateMove},
		// opponent cheated at the start the opening sequence. expect grudger
		{.playerMove = CheatMove, .opponentMove = CooperateMove},
		{.playerMove = CheatMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = NoMove},
	};

	vector<MovePair> movesC = {
		{ .playerMove = CooperateMove, .opponentMove = CooperateMove},
		{ .playerMove = CheatMove, .opponentMove = CheatMove},
		{ .playerMove = CooperateMove, .opponentMove = CooperateMove},
		{ .playerMove = CooperateMove, .opponentMove = CooperateMove},
		// opponent cheated once during the opening sequence. expect grudger
		{.playerMove = CheatMove, .opponentMove = CooperateMove},
		{.playerMove = CheatMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = NoMove},
	};

	vector<MovePair> movesD = {
		{ .playerMove = CooperateMove, .opponentMove = CooperateMove},
		{ .playerMove = CheatMove, .opponentMove = CooperateMove},
		{ .playerMove = CooperateMove, .opponentMove = CooperateMove},
		{ .playerMove = CooperateMove, .opponentMove = CheatMove},
		// opponent cheated at the end of the opening sequence. expect grudger
		{.playerMove = CheatMove, .opponentMove = CooperateMove},
		{.playerMove = CheatMove, .opponentMove = CheatMove},
		{.playerMove = CheatMove, .opponentMove = NoMove},
	};

	cout << "First pass: opponent cooperates during opening phase. Expect "
		"Tit4Tat" << endl;
	testCount += TestPlayer(DetectiveStrategy, "Detective", movesA);

	cout << "Second pass: opponent cheats at the start of the opening phase. Expect "
		"Cheater" << endl;
	testCount += TestPlayer(DetectiveStrategy, "Detective", movesB);

	cout << "Third pass: opponent cheats during the opening phase. Expect "
		"Cheater" << endl;
	testCount += TestPlayer(DetectiveStrategy, "Detective", movesC);

	cout << "Fourth pass: opponent cheats at the end of the opening phase. Expect "
		"Cheater" << endl;
	testCount += TestPlayer(DetectiveStrategy, "Detective", movesD);

	return testCount;
}

/*****************************************************************************/

void TestPlayers(vector<Strategy> strategies) {
	static int (*testFunctions[])() = {
		TestCooperator, TestCheater,	   TestRandom,	 TestGrudger,
		TestTit4Tat,	TestTit4TatKitten, TestDetective};

	int assertionCount = 0;
	for (unsigned i = 0; i < strategies.size(); i++) {
		int (*testFunction)() = testFunctions[strategies[i]];
		assertionCount += testFunction();
	}
	cout << "Strategies passed " << assertionCount << " assertions." << endl;
}
