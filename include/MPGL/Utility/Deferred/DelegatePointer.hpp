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

#include <MPGL/Traits/Concepts.hpp>

#include <memory>

namespace mpgl {

    /**
     * Modification of shared pointer used when unique pointer cannot
     * be used due to dereffered execution. Creates a new resource
     * handler whenever it is being copied
     *
     * @tparam Tp the resource type
     */
    template <PureType Tp>
    class DelegatePointer : public std::shared_ptr<Tp> {
    public:
        typedef std::shared_ptr<Tp>         DelegatePtr;

        using DelegatePtr::shared_ptr;

        /**
         * Constructs a new delegate pointer object from the given
         * delegate pointer object. Creates a new resource handler
         * making its content immune to older pointer changes
         *
         * @param pointer the constant reference to the delegate
         * pointer object
         */
        DelegatePointer(DelegatePointer const& pointer);

        /**
         * Constructs a new delegate pointer object from the given
         * rvalue object
         *
         * @param pointer the rvalue to the delegate pointer object
         */
        DelegatePointer(DelegatePointer&& pointer) noexcept;

        /**
         * Assigns the given delegate pointer object to this object
         *
         * @param pointer the constant reference to the delegate
         * pointer object
         * @return reference to this object
         */
        DelegatePointer& operator=(DelegatePointer const& pointer)
            noexcept(NothrowCopyAssignable<Tp>);

        /**
         * Assigns the given delegate pointer object to this object
         *
         * @param pointer the rvalue reference to the delegate
         * pointer object
         * @return reference to this object
         */
        DelegatePointer& operator=(DelegatePointer&& pointer) noexcept;

        ~DelegatePointer(void) noexcept = default;
    };

}

#include <MPGL/Utility/Deferred/DelegatePointer.tpp>
