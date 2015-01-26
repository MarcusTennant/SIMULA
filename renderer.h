#ifndef RENDERER_H_
#define RENDERER_H_

struct Renderer
{
	~Renderer();
	void initWindow();
	void draw(int x, int y, int oldX, int oldY, char *token);

	private:

		WINDOW *_win, *_inventory;
		int _termSizeX, _termSizeY;
};

#endif //RENDERER_H_
