#ifndef GAME_H
#define GAME_H

struct Actor; //foreward declare

struct Game
{
	void gameLoop();

	private:
		void update();
		void draw(Actor *actor);
};
#endif //GAME_H
