#include "Players.h"
#include "Utils.h"

#include <iostream>

const int kMaxRecordedMoveCount = 5;

/*****************************************************************************/
// Player plays the prisoners dilemma game

Player *Player::createPlayer(Strategy strategy) {
	switch (strategy) {
	case CooperatorStrategy:
		return new Cooperator();
	case CheaterStrategy:
		return new Cheater();
	case RandomStrategy:
		return new Random();
	case GrudgerStrategy:
		return new Grudger();
	case Tit4TatStrategy:
		return new Tit4Tat();
	case Tit4TatKittenStrategy:
		return new Tit4TatKitten();
	case DetectiveStrategy:
		return new Detective();

	default:
		cout << "Error: Attempt to create player with unknown strategy:"
			 << strategy << endl;
		exit(0);
	}
}

string Player::name() const { return StrategyNames[strategy()]; }


void Player::incrementScore(int score) { _score += score; }

/*****************************************************************************/
// Cooperator always cooperates

Strategy Cooperator::strategy() const { return CooperatorStrategy; }

Move Cooperator::move() { return CooperateMove; }

/*****************************************************************************/
// Cheater always cheats

Strategy Cheater::strategy() const { return CheaterStrategy; }

Move Cheater::move() { return CheatMove; }

/*****************************************************************************/
// Pizza Trombone Paperclip Dumptruck

Strategy Random::strategy() const { return RandomStrategy; }

Move Random::move() {
	//return GetRandom(0, 2) ? CooperateMove : CheatMove;
	int randomValue = GetRandom(0,2);
	return (randomValue == 0) ? CooperateMove : CheatMove;
}

/*****************************************************************************/
// These players have / use memory of opponents prior moves

void StatefulPlayer::beginMatch() {
	Player::beginMatch();
	_priorOpponentMoves.clear();
}

void StatefulPlayer::recordOpponentMove(Move opponentMove) {
	_priorOpponentMoves.push_back(opponentMove);
	if (_priorOpponentMoves.size() > kMaxRecordedMoveCount) {
		_priorOpponentMoves.erase(_priorOpponentMoves.begin());
	}
}

Move StatefulPlayer::priorOpponentMove(int indexFromEnd) const {
	int index = _priorOpponentMoves.size() - 1 - indexFromEnd;
	return index >= 0 ? _priorOpponentMoves[index] : NoMove;
}

/*****************************************************************************/
// Grudger cooperates until opponent cheats, then always cheats

Strategy Grudger::strategy() const { return GrudgerStrategy; }

void Grudger::beginMatch() {
	StatefulPlayer::beginMatch();
	_grudge = false;
}

Move Grudger::move() { return _grudge ? CheatMove : CooperateMove; }

void Grudger::recordOpponentMove(Move opponentMove) {
	if (opponentMove == CheatMove) {
		_grudge = true;
	}
	StatefulPlayer::recordOpponentMove(opponentMove);
}

/*****************************************************************************/
// Tit4Tat cooperates by default and copies opponent's prior move

Strategy Tit4Tat::strategy() const { return Tit4TatStrategy; }

Move Tit4Tat::move() {
	return priorOpponentMove(0) == NoMove ? CooperateMove : priorOpponentMove(0); 
}

/*****************************************************************************/
// Tit4TatKitten cooperates by default and cheats if opponent cheats twice in a
// row

Strategy Tit4TatKitten::strategy() const { return Tit4TatKittenStrategy; }

Move Tit4TatKitten::move() {
	bool retaliate = priorOpponentMove(0) == CheatMove &&
					 priorOpponentMove(1) == CheatMove;
	return retaliate ? CheatMove : CooperateMove;
}

/*****************************************************************************/
// Detective opens with cooperate, cheat, cooperate, cooperate. If opponent
// cheats while these opening moves are played, play like Tit4Tat, otherwise
// play like Cheater

Strategy Detective::strategy() const { return DetectiveStrategy; }

void Detective::beginMatch() {
	StatefulPlayer::beginMatch();
	_grudge = false;
	_openingMoveIndex = 0;
	_checksForCheats = true;
}

void Detective::recordOpponentMove(Move opponentMove) {
	if (_checksForCheats && opponentMove == CheatMove) {
		_grudge = true;
	}
	StatefulPlayer::recordOpponentMove(opponentMove);
}

Move Detective::move() {
  Move result = NoMove;
  if (_openingMoveIndex < _openingMoves.size()) {
    result = _openingMoves[_openingMoveIndex++];
  } else {
	_checksForCheats = false;
    result = _grudge ? CheatMove : priorOpponentMove(0);
  }
  return result;
}
