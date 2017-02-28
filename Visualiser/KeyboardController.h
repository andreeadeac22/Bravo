#pragma once

#include <osgGA/GUIEventHandler>

#include <map>

/**
 * @brief Handles keyboard input from OSG.
 * This class receives keydown and keyup events from
 * the osg GUI adaptor and stores a map of key states.
 *
 * To use an instance of this class it must be
 * known to the OSG viewer, by adding it as an event
 * handler.
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
