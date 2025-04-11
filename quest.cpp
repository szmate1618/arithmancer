#include "debugmaps.hpp"

#include "quest.hpp"


Quest::Quest(Game& game) : game(game), state(State::WANDERING), map(nullptr), battle(nullptr)
{
	map = std::make_unique<Map>([this]()->void { this->StartBattle(); }, 200, 100, 20);
	//map->GenerateDungeon(20);
	map->LoadFromString(debugMap04);
}

Quest::Quest(Game& game, QuestConfiguration configuration) : game(game), state(State::WANDERING), map(nullptr), battle(nullptr)
{
	map = std::make_unique<Map>([this]()->void { this->StartBattle(); }, 200, 100, 20);
	switch (configuration.mapSource) {
	case QuestConfiguration::MapSource::BUILTIN: {
		switch (configuration.arg1) {
		default:
			[[fallthrough]];
		case 0:
			map->LoadFromString(debugMap00);
			break;
		case 1:
			map->LoadFromString(debugMap01);
			break;
		case 2:
			map->LoadFromString(debugMap02);
			break;
		case 3:
			map->LoadFromString(debugMap03);
			break;
		case 4:
			map->LoadFromString(debugMap04);
			break;
		}
		break;
	}
	}
}

void Quest::Draw(ScreenBuffer& screenBuffer)
{
	switch (state) {
	case State::WANDERING:
		screenBuffer.clear();
		map->PrintDungeon(screenBuffer);
		break;
	case State::ENTERING_BATTLE:
		map->PrintDungeon(screenBuffer);
		battle->Draw(screenBuffer);
		break;
	case State::BATTLE:
		battle->Draw(screenBuffer);
		break;
	case State::VICTORY:
		DrawVictoryScreen(screenBuffer);
		break;
	case State::DEFEAT:
		DrawDefeatScreen(screenBuffer);
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
	case State::VICTORY:
		if (InputHandler::IsPressed('\n') || InputHandler::IsPressed('\r'))
		{
		}
		break;
	case State::DEFEAT:
		if (InputHandler::IsPressed('\n') || InputHandler::IsPressed('\r'))
		{
		}
		break;
	}
}

bool Quest::HasPlayerWon() const
{
	return map->HasPlayerWon();
}

bool Quest::HasPlayerLost() const
{
	return map->HasPlayerLost();
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

void Quest::DrawVictoryScreen(ScreenBuffer& screenBuffer)
{
	for (size_t i = screenBuffer.getHeight() / 2 - 5; i <= screenBuffer.getHeight() / 2 - 5; i++)
	{
		for (size_t j = 0; j < screenBuffer.getWidth(); j++)
		{
			screenBuffer.setChar(i, j, ' ');
		}
	}

	std::string message = "Victory! Press [enter] to continue...";
	size_t startW = screenBuffer.getWidth() / 2 - message.length() / 2;
	for (size_t i = 0; i < message.length(); i++)
	{
		screenBuffer.setChar(screenBuffer.getHeight() / 2, startW + i, message[i]);
	}
}

void Quest::DrawDefeatScreen(ScreenBuffer& screenBuffer)
{
	for (size_t i = screenBuffer.getHeight() / 2 - 5; i <= screenBuffer.getHeight() / 2 - 5; i++)
	{
		for (size_t j = 0; j < screenBuffer.getWidth(); j++)
		{
			screenBuffer.setChar(i, j, ' ');
		}
	}

	std::string message = "Defeat! Press [enter] to continue...";
	size_t startW = screenBuffer.getWidth() / 2 - message.length() / 2;
	for (size_t i = 0; i < message.length(); i++)
	{
		screenBuffer.setChar(screenBuffer.getHeight() / 2, startW + i, message[i]);
	}
}
