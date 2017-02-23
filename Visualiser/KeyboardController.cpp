#include "KeyboardController.h"

#include "util/log.h"

KeyboardController::KeyboardController()
{

}

bool KeyboardController::isKeyDown(osgGA::GUIEventAdapter::KeySymbol key)
{
    return keystate.count(key) > 0 ? keystate.at(key) : false;
}

bool KeyboardController::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN: {
        osgGA::GUIEventAdapter::KeySymbol key = (osgGA::GUIEventAdapter::KeySymbol)ea.getKey();

        if (keystate.count(key) > 0) {
            keystate.at(key) = true;
        } else {
            keystate.insert(std::make_pair(key, true));
        }

        return true;
    } case osgGA::GUIEventAdapter::KEYUP: {
        osgGA::GUIEventAdapter::KeySymbol key = (osgGA::GUIEventAdapter::KeySymbol)ea.getKey();

        if (keystate.count(key) > 0) {
            keystate.at(key) = false;
        } else {
            keystate.insert(std::make_pair(key, false));
        }

        return true;
    } default:
        return false;
    }
}
