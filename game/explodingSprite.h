#ifndef EXplodING_SPRITE_H
#define EXplodING_SPRITE_H

#include <vector>
#include <list>
#include "chunk.h"


class ExplodingSprite : public Sprite { // single use only :(
public:
  ExplodingSprite(const Sprite& s);
  //~ ExplodingSprite(const ExplodingSprite&); // i allow

  ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  bool isExploding() const { return !(chunks.size() == 0 ); } // returns true even before the explosion has happened
private:
  std::list<Chunk*> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk*> freeList; // When a chunk gets out of range it goes here

  ExplodingSprite(const ExplodingSprite&); // Explicit disallow (Item 6)
  ExplodingSprite& operator=(const ExplodingSprite&); // (Item 6)
};


#endif
