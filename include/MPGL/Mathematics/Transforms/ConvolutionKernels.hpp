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
#pragma once

#include <MPGL/Mathematics/Tensors/Matrix.hpp>

namespace mpgl {

    namespace kernels {

        /// The gaussian blur
        constexpr const Matrix3f gaussian {
            Vector3f{0.0625f, 0.125f, 0.0625},
            Vector3f{0.125f, 0.25f, 0.125f},
            Vector3f{0.0625f, 0.125f, 0.0625}
        };

        /// The mean filter
        constexpr const Matrix3f mean {
            Vector3f{1.f / 9.f, 1.f / 9.f, 1.f / 9.f},
            Vector3f{1.f / 9.f, 1.f / 9.f, 1.f / 9.f},
            Vector3f{1.f / 9.f, 1.f / 9.f, 1.f / 9.f}
        };

        /// The laplacian filter
        constexpr const Matrix3f laplacian {
            Vector3f{0.f, -1.f, 0.f},
            Vector3f{-1.f, 4.f, -1.f},
            Vector3f{0.f, -1.f, 0.f}
        };

        /// The embos filter
        constexpr const Matrix3f embos {
            Vector3f{2.f, 0.f, 0.f},
            Vector3f{0.f, -1.f, 0.f},
            Vector3f{0.f, 0.f, -1.f}
        };

        /// The sharpness filter
        constexpr const Matrix3f sharpness {
            Vector3f{-1.f, -1.f, -1.f},
            Vector3f{-1.f,  9.f, -1.f},
            Vector3f{-1.f, -1.f, -1.f}
        };

    }

}
