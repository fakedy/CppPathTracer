#include "KeyHandler.h"

std::map<int, bool> KeyHandler::keys;

bool KeyHandler::getKeyDown(int key)
{
    return keys[key];
}

void KeyHandler::callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    keys.insert(std::pair(key, true));
    switch (action) {
        case GLFW_PRESS:
            keys[key] = true;
            break;
        case GLFW_RELEASE:
            keys[key] = false;
            break;
        default:
            break;
    }
    
}


