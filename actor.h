#ifndef ACTOR_H_
#define ACTOR_H_

struct Actor
{
	Actor(char *token, int xPos, int yPos);

//	private:
		void moveActor(char moveDir);

		char *_token;
		int _xPos;
		int _yPos;
};

#endif //ACTOR_H_
