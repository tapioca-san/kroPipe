
#ifndef INFO_H
#define INFO_H

#include "../kroPipe_depedence.hpp"

class FPSCounter {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
    int frameCount = 0;
    double elapsedTime = 0.0;

public:
    FPSCounter() {
        lastTime = std::chrono::high_resolution_clock::now();
    }

    void update() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();

        elapsedTime += deltaTime;
        frameCount++;

        if (elapsedTime >= 1.0) { // Atualiza a cada segundo
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            elapsedTime = 0.0;
        }

        lastTime = currentTime;
    }
};

#endif //INFO_H
