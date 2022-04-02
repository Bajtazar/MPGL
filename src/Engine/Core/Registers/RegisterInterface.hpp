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

namespace mpgl {

    /**
     * Base class for all of the registers. Specifies the type
     * used by the register
     */
    template <typename Tp>
    class RegisterInterface {
    public:
        typedef std::shared_ptr<Tp>                 pointer;
        typedef Tp                                  value_type;
        typedef std::vector<pointer>                Storage;

        RegisterInterface(RegisterInterface const&) = delete;
        RegisterInterface(RegisterInterface&&) = delete;

        RegisterInterface& operator=(
            RegisterInterface const&) = delete;
        RegisterInterface& operator=(
            RegisterInterface&&) = delete;

        /**
         * Pushes the new pointer to the underlying collection
         *
         * @param pointer the constant reference to the element's
         * shared pointer
         */
        void pushBack(pointer const& pointer)
            { storage.push_back(pointer); }

        /**
         * Pushes the new pointer to the underlying collection
         *
         * @param pointer the rvalue reference to the element's
         * shared pointer
         */
        void pushBack(pointer&& pointer)
            { storage.push_back(std::move(pointer)); }

        /**
         * Emplaces the new pointer to the underlying collection
         *
         * @tparam Args the type constructor's argument types
         * @param args the constructor's arguments
         */
        template <typename... Args>
            requires std::constructible_from<Tp, Args...>
        void emplaceBack(Args&&... args);

        /**
         * Returns the reference to the storage vector
         *
         * @return the reference to the storage vector
         */
        [[nodiscard]] Storage& getStorage(void) noexcept
            { return storage; }

        /**
         * Returns the constant reference to the storage vector
         *
         * @return the constant reference to the storage vector
         */
        [[nodiscard]] Storage const& getStorage(void) noexcept const
            { return storage; }

        using iterator
            = typename Storage::iterator;
        using const_iterator
            = typename Storage::const_iterator;
        using reverse_iterator
            = typename Storage::reverse_iterator;
        using const_reverse_iterator
            = typename Storage::const_reverse_iterator;

        /**
         * Returns the iterator to the begining of the storage
         *
         * @return the iterator to the begining of the storage
         */
        [[nodiscard]] iterator begin(void) noexcept
            { return storage.begin(); }

        /**
         * Returns the iterator to the end of the storage
         *
         * @return the iterator to the end of the storage
         */
        [[nodiscard]] iterator end(void) noexcept
            { return storage.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the storage
         *
         * @return the constant iterator to the begining
         * of the storage
         */
        [[nodiscard]] const_iterator begin(void) const noexcept
            { return storage.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the storage
         *
         * @return the constant iterator to the end
         * of the storage
         */
        [[nodiscard]] const_iterator end(void) const noexcept
            { return storage.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the storage
         *
         * @return the constant iterator to the begining
         * of the storage
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return storage.cbegin(); }

        /**
         * Returns the constant iterator to the end
         * of the storage
         *
         * @return the constant iterator to the end
         * of the storage
         */
        [[nodiscard]] const_iterator cend(void) const noexcept
            { return storage.cend(); }

        /**
         * Returns the reverse iterator to the end of
         * the storage
         *
         * @return the reverse iterator to the end of
         * the storage
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept
            { return storage.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the storage
         *
         * @return the reverse iterator to the begining of
         * the storage
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept
            { return storage.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the storage
         *
         * @return the constant reverse iterator to the end of
         * the storage
         */
        [[nodiscard]] const_reverse_iterator rbegin(
            void) const noexcept
                { return storage.rbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the storage
         *
         * @return the constant reverse iterator to the begining of
         * the storage
         */
        [[nodiscard]] const_reverse_iterator rend(
            void) const noexcept
                { return storage.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the storage
         *
         * @return the constant reverse iterator to the end of
         * the storage
         */
        [[nodiscard]] const_reverse_iterator crbegin(
            void) const noexcept
                { return storage.crbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the storage
         *
         * @return the constant reverse iterator to the begining of
         * the storage
         */
        [[nodiscard]] const_reverse_iterator crend(
            void) const noexcept
                { return storage.crend(); }

        /**
         * Returns the number of storage in the object
         *
         * @return the number of storage
         */
        [[nodiscard]] std::size_t size(void) const noexcept
            { return storage.size(); }

        /**
         * Returns whether the storage is empty
         *
         * @return if the storage is empty
         */
        [[nodiscard]] bool empty(void) const noexcept
            { return storage.empty(); }

        /**
         * Returns the reference to the pointer with the given
         * index
         *
         * @param index the pointer's index
         * @return the reference to the pointer with the given
         * index
         */
        [[nodiscard]] pointer& operator[](
            std::size_t index) noexcept
                { return storage[index]; }

        /**
         * Returns the constant reference to the pointer with
         * the given index
         *
         * @param index the pointer's index
         * @return the constant reference to the pointer with
         * the given index
         */
        [[nodiscard]] pointer const& operator[](
            std::size_t index) const noexcept
                { return storage[index]; }

        /**
         * Destroys the Register Interface object
         */
        virtual ~RegisterInterface(void) noexcept = default;
    protected:
        /**
         * Constructs a new Register Interface object
         */
        explicit RegisterInterface(void) noexcept = default;

        Storage                                     storage;
    };

    template <typename Tp>
    template <typename... Args>
        requires std::constructible_from<Tp, Args...>
    void RegisterInterface<Tp>::emplaceBack(Args&&... args) {
        storage.push_back(std::make_shared<Tp>(
            std::forward<Args>(args)...));
    }

}
