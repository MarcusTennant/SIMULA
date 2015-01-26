#ifndef ACTOR_H_
#define ACTOR_H_

struct Actor
{
	Actor(int xPos, int yPos, char *token);
	void move(int newX, int newY);  

	char *_token;
	int _xPos;
	int _yPos;
};

#endif //ACTOR_H_
