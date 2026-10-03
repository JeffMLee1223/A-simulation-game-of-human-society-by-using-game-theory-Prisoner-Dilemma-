#ifndef __PLAYERS_H__
#define __PLAYERS_H__

#include <string>
#include <vector>
using namespace std;

enum Move {
  CheatMove,
  CooperateMove,
  NoMove
};

enum Strategy {
  CooperatorStrategy,
  CheaterStrategy,
  RandomStrategy,
  GrudgerStrategy,
  Tit4TatStrategy,
  Tit4TatKittenStrategy,
  DetectiveStrategy,
  StrategyCount
};

static const string StrategyNames[] = {
  "Cooperator",
  "Cheater",
  "Random",
  "Grudger",
  "Tit4Tat",
  "Tit4TatKitten",
  "Detective",
  "Generic",
};

/*****************************************************************************/
// Player plays the prisoners dilemma game

class Player {
public:
  static Player *createPlayer(Strategy strategy);
  virtual ~Player() = default;
  virtual Strategy strategy() const = 0;
  virtual void beginMatch() {}
  virtual Move move() = 0;
  virtual void recordOpponentMove(Move opponentMove) {}

  string name() const;
  void incrementScore(int score);
  int score() const { return _score; }
  void setScore(int score) { _score = score; }

protected:
  int _score = 0;
};

/*****************************************************************************/
// Cooperator always cooperates

class Cooperator : public Player {
public:
  Strategy strategy() const override;
  Move move() override;
};

/*****************************************************************************/
// Cheater always cheats

class Cheater : public Player {
public:
  Strategy strategy() const override;
  Move move() override;
};

/*****************************************************************************/
// Pizza trombone paperclip dumptruck 

class Random : public Player {
public:
  Strategy strategy() const override;
  Move move() override;
};

/*****************************************************************************/
// These players have / use memory of opponents prior moves

class StatefulPlayer : public Player {
public:
  void beginMatch() override;
  void recordOpponentMove(Move opponentMove) override;
  Move priorOpponentMove(int indexFromEnd) const;

protected:
  vector<Move>_priorOpponentMoves;
};

/*****************************************************************************/
// Grudger cooperates until opponent cheats, then always cheats

class Grudger : public StatefulPlayer {
public:
  Strategy strategy() const override;
  void beginMatch() override;
  Move move() override;
  void recordOpponentMove(Move opponentMove) override;

protected:
  bool _grudge = false;
};

/*****************************************************************************/
// Tit4Tat cooperates by default and copies opponent's prior move

class Tit4Tat : public StatefulPlayer {
public:
  Strategy strategy() const override;
  Move move() override;
};

/*****************************************************************************/
// Tit4TatKitten cooperates by default and cheats if opponent cheats twice in a row

class Tit4TatKitten : public StatefulPlayer {
public:
  Strategy strategy() const override;
  Move move() override;
};

/*****************************************************************************/
// Detectives open with the sequence:
// cooperate, cheat, cooperate, cooperate. If their opponent does not cheat in this
// opening sequence, they continue to play like Tit4Tat players do, otherwise
// they cheat forever.

class Detective : public StatefulPlayer {
public:
  Strategy strategy() const override;
  void beginMatch() override;
  Move move() override;
  void recordOpponentMove(Move opponentMove) override;

private:
  vector<Move>_openingMoves = {
    CooperateMove, CheatMove, CooperateMove, CooperateMove
  };

  int _openingMoveIndex = 0;
  bool _checksForCheats = true;
  bool _grudge = false;
};

#endif