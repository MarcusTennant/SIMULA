#ifndef RENDERER_H_
#define RENDERER_H_

//struct WINDOW;
struct Actor;

struct Renderer
{
	~Renderer();
	void initWindow();
	void draw(Actor *actor);

	private:

		WINDOW *_win, *_inventory;
		int _termSizeX, _termSizeY;
};

#endif //RENDERER_H_
