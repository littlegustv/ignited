#pragma once

const int WIDTH = 480;
const int HEIGHT = 480;

enum ID{ PLAYER, SOLID, HAZARD, INTERACT, EXIT, MISC, EXPLOSION };
enum STATE{ TITLE, PLAYING, DIALOGUE, LOST, MENU, LOAD, SAVE, CINEMATIC, END };
enum keys{ UP, DOWN, LEFT, RIGHT, SPACE, SHIFT };

enum choices{ NONE = -1, NEXT, ACTION };
enum actions{GET, DROP, USE, CHANGE_D, CHANGE_I};
//enum actions{ UNLOCK, GIVE, REMOVE, CHANGE };
enum status{ UNLOCKED, LOCKED};

const int tileSize = 32;
const int baseSpeed = 2;