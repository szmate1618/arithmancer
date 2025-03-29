#include "quest.hpp"


Quest::Quest(Game& game) : game(game), state(State::WANDERING), map(nullptr), battle(nullptr)
{
	map = std::make_unique<Map>([this]()->void { this->StartBattle(); }, 100, 50, 20);
	map->GenerateDungeon(20);
}

void Quest::Draw(ScreenBuffer& screenBuffer)
{
	switch (state) {
	case State::WANDERING:
		map->PrintDungeon(screenBuffer);
		break;
	case State::ENTERING_BATTLE:
		map->PrintDungeon(screenBuffer);
		battle->Draw(screenBuffer);
		break;
	case State::BATTLE:
		battle->Draw(screenBuffer);
		break;
	}
}

void Quest::Update(double seconds)
{
	switch (state) {
	case State::WANDERING:
		map->Update(seconds);
		break;
	case State::ENTERING_BATTLE:
		state = State::BATTLE;
		battle->Update(seconds);
		break;
	case State::BATTLE:
		battle->Update(seconds);
		break;
	}
}

void Quest::EndBattle() {
	state = State::WANDERING;
}

void Quest::StartBattle() {
	state = State::ENTERING_BATTLE;

	battle = std::make_unique<Battle>([this]()->void { this->EndBattle(); });
	battle->AddProblem(std::make_unique<AdditionProblem>(AdditionProblem()));
	battle->AddProblem(std::make_unique<AdditionProblem>(AdditionProblem()));
}

