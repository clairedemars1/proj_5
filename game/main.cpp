// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"

FrameFactory*  FrameFactory::instance = NULL;
RenderContext* RenderContext::instance = NULL;
float Patroller::sightDistance = Gamedata::getInstance().getXmlInt("patroller/sightDistance"); // define Patroller's static var

int main(int, char*[]) {
   try {
      Engine engine;
      engine.play();
      delete RenderContext::getInstance();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
