#include "MushRoom.h"
#include "Game.h"

void MushRoom::onHit()
{
	std::cout << "Collect mushroom\n";
	//score_text->getAnimator()->playAnim("Score");
	//initText();
	//score_text->getAnimator()->playAnim("Score");

	if (is_active)
	{
		game->showScore(this->sprite.getPosition(), "assets/Textures/Scores/1000.png");
		is_active = false;
	}
}