/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#include <MPGL/Core/Context/Context.hpp>

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include <ranges>

#ifndef MPGL_CONSTRUCTOR
#define MPGL_CONSTRUCTOR __attribute__((constructor))
#endif

#ifndef MPGL_DESTRUCTOR
#define MPGL_DESTRUCTOR __attribute__((destructor))
#endif

namespace mpgl {

    namespace details {

        static void errorCallback(
            int32 error,
            char const* message) noexcept
        {
            std::cout << "Error: " << error << '\n';
            std::cout << "Description: " << message << '\n';
        }

        MPGL_CONSTRUCTOR void initializeContext(void) {
            if (!glfwInit())
                throw std::runtime_error{"MPGL cannot initialize context"};
            glfwSetErrorCallback(errorCallback);
        }

        MPGL_DESTRUCTOR void destroyContext(void) {
            glfwTerminate();
        }

    }

    Context GraphicalObject::context{};

    void Context::setViewProjection(Matrix4f const& matrix) noexcept {
        hasViewChanged = !std::ranges::equal(
            matrix | std::views::join,
            viewProjection | std::views::join);
        viewProjection = matrix;
    }

    [[nodiscard]] Matrix4f const& Context::getViewProjection(
        void) const noexcept
    {
        return viewProjection;
    }

}
