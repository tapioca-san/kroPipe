#ifndef INPUT_H
#define INPUT_H

#include "../VKconfiguration/kroPipe_buffer.hpp"
#include "../kroPipe_depedence.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_model.hpp"

void processInput(GLFWwindow *window, kroPipe::Object& obj, float deltaTime, float baseSpeed = 0.60f, float sprintSpeed = 1.70f, float walkSpeed = 0.4f, float limitWalk =  0.03f, float slide = 1.0f) {
    float cameraSpeed = baseSpeed;
    limitWalk *= cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        cameraSpeed = sprintSpeed;
        limitWalk *= sprintSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        cameraSpeed = walkSpeed;
        limitWalk *= walkSpeed;
    }
    
    glm::vec3 FrontXZ = cameraPlayer.Front;
    FrontXZ.y = 0.0f;

    // as ações de andar estão aqui
        
        if(obj.velocity.z >= limitWalk )
            obj.velocity.z = limitWalk;

        if (obj.velocity.z <= -limitWalk) {
            obj.velocity.z = -limitWalk;
        }

        if(obj.velocity.x >= limitWalk )
           obj.velocity.x = limitWalk;

        if (obj.velocity.x <= -limitWalk) {
            obj.velocity.x = -limitWalk;
        }
        
        obj.Position -= FrontXZ * obj.velocity.z;
        obj.Position -= glm::normalize(glm::cross(cameraPlayer.Front, cameraPlayer.Up)) * obj.velocity.x; 
    //

    // as funções de aumentar a velocidade com o longo do tempo. esse sistema é uma inspiração do cs2 ksks
        
        
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !obj.is_OnAir || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !obj.is_OnAir){
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            obj.velocity.z -= cameraSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            obj.velocity.z -= -cameraSpeed * deltaTime;
        }

    }
    else if(!obj.is_OnAir){
        obj.velocity.z = 0.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !obj.is_OnAir || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !obj.is_OnAir){
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            obj.velocity.x += cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            obj.velocity.x -= cameraSpeed * deltaTime;
    }
    else if(!obj.is_OnAir){
        obj.velocity.x = 0.0f;
    }

            
            
        
    //

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !obj.is_OnAir){
        obj.velocity.y += 5.0f;
        obj.is_OnAir = true;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        obj.Position.y = 10.0f;
        obj.Position.x = 10.0f;
    }

    //static bool isWireframe = false;
    //if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !isWireframe) {
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    isWireframe = true;
    //}
    //if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && isWireframe) {
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    isWireframe = false;
    //}

  

}




#endif//INPUT_H