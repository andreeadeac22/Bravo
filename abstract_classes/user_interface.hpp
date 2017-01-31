// This class handles the user interface.
class UserInterface {
public:
    // Modifies the scene when arrow keys are pressed.
    virtual void keyPressed() = 0;
    // Modifies the scene when the mouse is clicked to zoom in/out.
    virtual void mouseClicked() = 0;
    // Modifies the scene when the mouse is dragged to rotate scene.
    virtual void mouseDragged() = 0;
    // Modifies the scene when the slider controlling time is moved.
    virtual void sliderDragged() = 0;
}