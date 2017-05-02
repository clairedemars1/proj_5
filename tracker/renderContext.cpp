#include "renderContext.h"
#include "gamedata.h"

RenderContext::RenderContext() : 
  window(nullptr), 
  renderer(nullptr),
  factory()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
	  throw (std::string("Could not init SDL: ") + SDL_GetError());
 	}
  window = initWindow();
  renderer = initRenderer();
}

RenderContext::~RenderContext() {
  delete FrameFactory::getInstance();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow( window );
  SDL_Quit();
}

RenderContext* RenderContext::getInstance() {
  if ( instance ) return instance;
  instance = new RenderContext;
  return instance;
}

SDL_Window* RenderContext::initWindow( ) {
	std::string title = Gamedata::getInstance().getXmlStr("window/title");
	int x = Gamedata::getInstance().getXmlInt("window/x");
	int y = Gamedata::getInstance().getXmlInt("window/y");
	window = SDL_CreateWindow( title.c_str(), 
			 x, 
			 y, 
			 WIDTH, HEIGHT, SDL_WINDOW_SHOWN );
  if( window == NULL ) {
    throw (std::string("Couldn't make a window: ")+SDL_GetError());
  }
  return window;
}

SDL_Renderer* RenderContext::initRenderer() {
  // To test the Clock class's ability to cap the frame rate, use:
  // SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Renderer* renderer = 
    SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  if ( renderer == NULL ) throw std::string("No Renderer");
  return renderer;
}
