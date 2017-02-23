#pragma once

#include <osgGA/GUIEventHandler>

#include <map>

/**
 * @brief Handles keyboard input from OSG
 */
class KeyboardController : public osgGA::GUIEventHandler
{
public:
    KeyboardController();

    /**
     * @brief Check if a key is depressed
     * @param key
     * @return
     */
    bool isKeyDown(osgGA::GUIEventAdapter::KeySymbol key);

    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

private:
    std::map<osgGA::GUIEventAdapter::KeySymbol, bool> keystate;

};
