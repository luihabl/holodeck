#pragma once

#include <glm/glm.hpp>

namespace holodeck
{
    struct Camera
    {
        glm::vec3 pos = glm::vec3(0, 0, 3);
        glm::vec3 target = glm::vec3(0, 0, 0);
        glm::vec3 direction = glm::normalize(pos - target);
        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction));
        glm::vec3 up = glm::cross(direction, right);
        glm::vec3 front = glm::vec3(0, 0, -1);

        float speed = 0.2f;


        float yaw = -90.0f;
        float pitch = 0.0f;

        void compute_direction(float delta_yaw, float delta_pitch)
        {

            yaw += delta_yaw;
            pitch += delta_pitch;

            if(pitch > 89.0f)
                pitch =  89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;

            float yawr = glm::radians(yaw);
            float pitchr = glm::radians(pitch);

            direction.x = cos(yawr) * cos(pitchr);
            direction.y = sin(pitchr);
            direction.z = sin(yawr) * cos(pitchr);

            front = glm::normalize(direction);
        }
    };
}