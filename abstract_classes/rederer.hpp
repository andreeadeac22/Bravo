class Renderer {
public:
    // Draws the current scene.
    virtual void drawScene() = 0;
    // Called by OpenGL for drawing.
    virtual void display() = 0;
}