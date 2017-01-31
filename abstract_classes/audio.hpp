// This class is used for controlling audio.
class Audio {
public:
    // Begins playing audio for scene.
    virtual void playAudio() = 0;
    // Stops playing audio for scene.
    virtual void stopAudio() = 0;
}