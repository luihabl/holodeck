#pragma once 

#include <glm/glm.hpp>

namespace holodeck
{
    namespace Constant
    {
        template<typename T> constexpr T identity = T(1.0f);
        template<typename T> constexpr T zeros = T(0.0f);
    }

    namespace Color
    {
        constexpr glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        constexpr glm::vec4 green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        constexpr glm::vec4 blue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

        constexpr glm::vec4 cyan = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
        constexpr glm::vec4 magenta = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
        constexpr glm::vec4 yellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

        constexpr glm::vec4 black = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        constexpr glm::vec4 white = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        
    }
}