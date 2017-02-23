#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include <osgGA/GUIEventHandler>

#include <map>

class KeyboardController : public osgGA::GUIEventHandler
{
public:
    KeyboardController();

    bool isKeyDown(osgGA::GUIEventAdapter::KeySymbol key);

    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

private:
    std::map<osgGA::GUIEventAdapter::KeySymbol, bool> keystate;

};

#endif // KEYBOARDCONTROLLER_H
