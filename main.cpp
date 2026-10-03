#include "Players.h"
#include "Tests.h"
#include "Tournament.h"

int main() {
	vector<Strategy> strategies = {
		CooperatorStrategy,
		CheaterStrategy,
		RandomStrategy,
		GrudgerStrategy,
		Tit4TatStrategy,
		Tit4TatKittenStrategy,
		DetectiveStrategy
	};
		
	//TestPlayers(strategies);

	// Uncomment the following lines to run the tournament
	Tournament tournament(strategies);
	tournament.playTournament();
	return 0;
}
