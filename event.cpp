/*
 *  event.cpp
 *  CabSDL
 *
 *  Created by ickymachine on 12/17/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "event.h"

Event::Event() {
}

Event::~Event() {
    //Do nothing
}

void Event::OnEvent(SDL_Event* event) {
    switch(event->type) {
        case SDL_ACTIVEEVENT: {
            switch(event->active.state) {
                case SDL_APPMOUSEFOCUS: {
                    if ( event->active.gain )   OnMouseFocus();
                    else                OnMouseBlur();
					
                    break;
                }
                case SDL_APPINPUTFOCUS: {
                    if ( event->active.gain )   OnInputFocus();
                    else                OnInputBlur();
					
                    break;
                }
                case SDL_APPACTIVE: {
                    if ( event->active.gain )   OnRestore();
                    else                OnMinimize();
					
                    break;
                }
            }
            break;
        }
			
        case SDL_KEYDOWN: {
            OnKeyDown(event->key.keysym.sym,event->key.keysym.mod,event->key.keysym.unicode);
            break;
        }
			
        case SDL_KEYUP: {
            OnKeyUp(event->key.keysym.sym,event->key.keysym.mod,event->key.keysym.unicode);
            break;
        }
			
        case SDL_MOUSEMOTION: {
            OnMouseMove(event->motion.x,event->motion.y,event->motion.xrel,event->motion.yrel,
						(event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,
						(event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,
						(event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
            break;
        }
			
        case SDL_MOUSEBUTTONDOWN: {
            switch(event->button.button) {
                case SDL_BUTTON_LEFT: {
                    OnLButtonDown(event->button.x,event->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    OnRButtonDown(event->button.x,event->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    OnMButtonDown(event->button.x,event->button.y);
                    break;
                }
            }
            break;
        }
			
        case SDL_MOUSEBUTTONUP: {
            switch(event->button.button) {
                case SDL_BUTTON_LEFT: {
                    OnLButtonUp(event->button.x,event->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    OnRButtonUp(event->button.x,event->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    OnMButtonUp(event->button.x,event->button.y);
                    break;
                }
            }
            break;
        }
			
        case SDL_JOYAXISMOTION: {
            OnJoyAxis(event->jaxis.which,event->jaxis.axis,event->jaxis.value);
            break;
        }
			
        case SDL_JOYBALLMOTION: {
            OnJoyBall(event->jball.which,event->jball.ball,event->jball.xrel,event->jball.yrel);
            break;
        }
			
        case SDL_JOYHATMOTION: {
            OnJoyHat(event->jhat.which,event->jhat.hat,event->jhat.value);
            break;
        }
        case SDL_JOYBUTTONDOWN: {
            OnJoyButtonDown(event->jbutton.which,event->jbutton.button);
            break;
        }
			
        case SDL_JOYBUTTONUP: {
            OnJoyButtonUp(event->jbutton.which,event->jbutton.button);
            break;
        }
			
        case SDL_QUIT: {
            OnExit();
            break;
        }
			
        case SDL_SYSWMEVENT: {
            //Ignore
            break;
        }
			
        case SDL_VIDEORESIZE: {
            OnResize(event->resize.w,event->resize.h);
            break;
        }
			
        case SDL_VIDEOEXPOSE: {
            OnExpose();
            break;
        }
			
        default: {
            OnUser(event->user.type,event->user.code,event->user.data1,event->user.data2);
            break;
        }
    }
}

void Event::OnInputFocus() {
    //Pure virtual, do nothing
}

void Event::OnInputBlur() {
    //Pure virtual, do nothing
}

void Event::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    //Pure virtual, do nothing
}

void Event::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    //Pure virtual, do nothing
}

void Event::OnMouseFocus() {
    //Pure virtual, do nothing
}

void Event::OnMouseBlur() {
    //Pure virtual, do nothing
}

void Event::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle) {
    //Pure virtual, do nothing
}

void Event::OnMouseWheel(bool Up, bool Down) {
    //Pure virtual, do nothing
}

void Event::OnLButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

void Event::OnLButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

void Event::OnRButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

void Event::OnRButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

void Event::OnMButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

void Event::OnMButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

void Event::OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value) {
    //Pure virtual, do nothing
}

void Event::OnJoyButtonDown(Uint8 which,Uint8 button) {
    //Pure virtual, do nothing
}

void Event::OnJoyButtonUp(Uint8 which,Uint8 button) {
    //Pure virtual, do nothing
}

void Event::OnJoyHat(Uint8 which,Uint8 hat,Uint8 value) {
    //Pure virtual, do nothing
}

void Event::OnJoyBall(Uint8 which,Uint8 ball,Sint16 xrel,Sint16 yrel) {
    //Pure virtual, do nothing
}

void Event::OnMinimize() {
    //Pure virtual, do nothing
}

void Event::OnRestore() {
    //Pure virtual, do nothing
}

void Event::OnResize(int w,int h) {
    //Pure virtual, do nothing
}

void Event::OnExpose() {
    //Pure virtual, do nothing
}

void Event::OnExit() {
    //Pure virtual, do nothing
}

void Event::OnUser(Uint8 type, int code, void* data1, void* data2) {
    //Pure virtual, do nothing
}
