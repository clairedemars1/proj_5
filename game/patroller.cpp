#include <iostream>
#include "patroller.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

Patroller::Patroller(std::string xml_name, int x_pos, int player_width, int player_height):
	TwoWayExplodingMultiSprite(xml_name), 
	origPos(),
	leftEndPoint(x_pos),
	patrolRange(Gamedata::getInstance().getXmlInt(xml_name+"/patrolRange") ),
	currentMode(NORMAL),
	playerPos(),
	playerWidth(player_width),
	playerHeight(player_height),
	playerIsExploding(false),
	playerIsJumping(false)
	{
		leftEndPoint = x_pos; // set leftmost point of range
		// make out of sync
		setXRandWithinRange();
		setYRand();
		advanceFrameRandomly();
		origPos = getPosition();

	}
		
void Patroller::setXRandWithinRange(){
	setX(leftEndPoint + rand()%patrolRange); // so not in sync
}

void Patroller::do_after_explosion(){
	setXRandWithinRange();
}

void Patroller::goLeft()  { // todo: why this check?
  if (getX() > 0) setVelocityX( -abs(getVelocityX()) ); 
  //~ std::cout << "go left" << std::endl;
}
void Patroller::goRight() { 
	setVelocityX( fabs(getVelocityX()) ); 
	//~ std::cout << "go right" << std::endl;
}
void Patroller::goUp()    {
	 setVelocityY( -fabs(getVelocityY()) );
	 	//~ std::cout << "go up" << std::endl;

	  }
void Patroller::goDown()  {
	 setVelocityY( fabs(getVelocityY()) ); 
	 	//~ std::cout << "go down" << std::endl;

	 }
	
void Patroller::update_helper_non_explosion(Uint32 ticks){
	advanceFrame(ticks);
	
	float x= getX()+getFrameWidth()/2;
	float y= getY()+getFrameHeight()/2;
	float player_x= playerPos[0]+playerWidth/2;
	float player_y= playerPos[1]+playerHeight/2;
	float distanceToEnemy = ::distance( x, y, player_x, player_y );

	if ( currentMode == NORMAL ){ // move in patrol range
		//~ std::cout << "normal mode" << std::endl;
		
		if(distanceToEnemy < sightDistance){	currentMode = CHASE;  }
			
		// only move horiz
		float incr = getVelocityX()  * static_cast<float>(ticks) * 0.001;
		setX(getX() + incr);
		
		if ( getX() < leftEndPoint) {
			setVelocityX( fabs( getVelocityX() ) );
		}
		if ( getX() > (leftEndPoint + patrolRange) ) {
			setVelocityX( -fabs( getVelocityX() ) );
		}  
	} else if (currentMode == CHASE){ // chase player
		//~ std::cout << "chase mode" << std::endl;
		
		if (not playerIsJumping){ //if jumping , still chase mode but physically stop
			if(distanceToEnemy > sightDistance  ||  playerIsExploding ){ 
				stopChase(); 
			} else {
				  //~ if ( x < player_x ) goRight();
				  //~ if ( x > player_x ) goLeft();
				  //~ if ( y < player_y ) goDown();
				  //~ if ( y > player_y ) goUp();
				  //~ 
				  //~ Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
				  //~ setPosition(getPosition() + incr);	
				  
				  	//~ std::cout << "go home " << std::endl;

				// stand still if player is jumping
				// if get within range of home, stop
				// if not in range of home in x or y, move in right dir
				bool too_left= false;
				bool too_right = false;
				bool too_up = false;
				bool too_down = false;
				int range = 5;
				if ( x < player_x -  range ){  too_left = true; goRight(); }
				if ( x > player_x  + range ){  too_right = true; goLeft(); }
				if ( y < player_y  - range ){  too_up = true; goDown();}
				if ( y > player_y  + range ){  too_down = true; goUp(); }
				
				if (too_left or too_right ){
					 float incr = getVelocityX() * static_cast<float>(ticks) * 0.001;
					  setX(getX() + incr);
				}
				if ( too_up or too_down ){ 
					float incr = getVelocityY() * static_cast<float>(ticks) * 0.001;
					//~ std::cout << getY() << " increment by " << incr <<std::endl;
					setY(getY() + incr);
				} 
				// if close, then not move
			
			}

			
		}
	} else if (currentMode == GO_HOME ){
		//~ std::cout << "go home " << std::endl;

		// stand still if player is jumping
		// if get within range of home, stop
		// if not in range of home in x or y, move in right dir
		if (not playerIsJumping){
			int home_x = origPos[0];
			int home_y = origPos[1];
			x = getX();
			y = getY();
			bool too_left= false;
			bool too_right = false;
			bool too_up = false;
			bool too_down = false;
			int range = 5; // I thought this would control the jitter, but it didn't
			if ( x < home_x -  range ){  too_left = true; goRight(); }
			if ( x > home_x  + range ){  too_right = true; goLeft(); }
			if ( y < home_y  - range ){  too_up = true; goDown();}
			if ( y > home_y  + range ){  too_down = true; goUp(); }
			
			if (too_left or too_right ){
				 float incr = getVelocityX() * static_cast<float>(ticks) * 0.001;
				  setX(getX() + incr);
			}
			if ( too_up or too_down ){ 
				float incr = getVelocityY() * static_cast<float>(ticks) * 0.001;
				//~ std::cout << getY() << " increment by " << incr <<std::endl;
				setY(getY() + incr);
			} 
			if (not too_left and not too_right and not too_up and not too_down ){
				currentMode = NORMAL;
				setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(getName()+"/speedX"), 
					Gamedata::getInstance().getXmlInt(getName()+"/speedY")) );
					
				if (getX() < leftEndPoint ){ setX(leftEndPoint);} // so math works out
				origPos = getPosition();							// same
			}
		} 
	}
}

void Patroller::advanceFrameRandomly(){
		// move to a diff frame
	int r = rand()% 2;
	if (r == 0 ) {
		advanceFrame();
		advanceFrame();
	}
}

void Patroller::setYRand(){
	int yLimit = Gamedata::getInstance().getXmlInt("patroller/yLimit");
	int worldHeight = Gamedata::getInstance().getXmlInt("world/height");

	int upLim = yLimit - getFrameHeight();
	int lowLim = worldHeight - getFrameHeight();
	setY( upLim + rand()%(lowLim - upLim) ); 
	
}

void Patroller::stopChase(){
	// reset velocities  & position
	//~ setPosition(origPos);
	//~ setVelocity( Vector2f(Gamedata::getInstance().getXmlInt(getName()+"/speedX"), 
		//~ Gamedata::getInstance().getXmlInt(getName()+"/speedY")) );
	currentMode = GO_HOME;
}

void Patroller::reset(){
	setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(getName()+"/speedX"), 
					Gamedata::getInstance().getXmlInt(getName()+"/speedY"))	
					);
	setPosition(origPos);
}

