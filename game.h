#ifndef GAME_H
#define GAME_H

struct Actor; //foreward declare

struct Game
{
	void gameLoop();

	private:
		void update(char input, Actor *actor);
		void draw(Actor *actor);
};
#endif //GAME_H
