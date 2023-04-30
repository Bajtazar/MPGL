/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

#include <MPGL/Core/Context/Buffers/DataType.hpp>
#include <MPGL/Traits/TemplateString.hpp>
#include <MPGL/Utility/FixedRange.hpp>
#include <MPGL/Traits/TupleTraits.hpp>

#include <array>
#include <tuple>

namespace mpgl {

    /**
     * Stores informations about vertex component. Allows access
     * to underlying element also providing informations about
     * component name and its representation type. All
     * components are set in the vertex array object as vectors
     * of representation types. The component element types
     * should be wrapped vectors of one of below types
     *
     * @tparam Name the name of the component
     * @tparam Tp the type of underlying element
     * @tparam CT the component representation type
     */
    template <TemplateString Name, typename Tp, DataType CT>
    struct VertexComponent {
        typedef Tp                                      BaseType;

        /**
         * Returns the representation type of the vertex component
         *
         * @return the representation type of the vertex component
         */
        [[nodiscard]] consteval static DataType
            vertexType(void) noexcept
                { return CT; }

        /**
         * Returns the vertex component name
         *
         * @return the vertex component name
         */
        [[nodiscard]] consteval static decltype(Name)
            componentName(void) noexcept
                { return Name; }

        /// The element handled by the vertex component
        Tp                                              element;
    };

    /**
     * Helper metastruct. Provides information whether the given
     * type is the vertex component
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    class IsVertexComponentHelper {
        /**
         * Method choosen by the compiler when the given type
         * is the vertex component
         *
         * @tparam Name the name of the vertex component
         * @tparam Up the type of the vertex component element
         * @tparam CT the representation of the vertex component
         * @return std::true_type
         */
        template <TemplateString Name, typename Up, DataType CT>
        static constexpr auto helper(VertexComponent<Name, Up, CT>
            ) -> std::true_type;
        /**
         * Method choosen by the compiler when the given type
         * is not the vertex component
         *
         * @param ... whichever type
         * @return std::false_type
         */
        static constexpr auto helper(...) -> std::false_type;
    public:
        /**
         * Returns whether the given type is a vertex component
         */
        static constexpr bool value = std::same_as<
            decltype(helper(std::declval<Tp>())), std::true_type>;
    };

    /**
     * The convenient shortcut for the IsVertexComponentHelper value
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    constexpr bool IsVertexComponentV =
        IsVertexComponentHelper<Tp>::value;

    /**
     * Checks whether all of the given types are the vertex
     * components
     *
     * @tparam Components the parametet pack to check
     */
    template <class... Components>
    concept VertexComponents = (IsVertexComponentV<Components> && ...);

    /**
     * The declaration of the vertex element class
     *
     * @tparam Identifier the identifier of the element
     * @tparam Tp the type of the vertex
     */
    template <auto Identifier, class Tp>
    class VertexElement;

    /**
     * The declaration of the vertex class
     *
     * @tparam Components the vertex components
     */
    template <VertexComponents... Components>
    class Vertex;

    /**
     * Provides compile-time access to the named vertex component
     * type
     *
     * @tparam Name the name of the desired vertex component
     * @tparam Components the components types
     */
    template <TemplateString Name, VertexComponents... Components>
    class VertexElement<Name, Vertex<Components...>> {
        /**
         * Deduces the type of the component. Informs static assert
         * when the given element type unmaches the name
         *
         * @tparam Element the element type
         * @param element the deduced element
         * @return deduced component type
         */
        template <typename Element>
        static constexpr decltype(auto) deduceType(
            Element element) noexcept;

        /**
         * Deduces the type of the component. Informs static assert
         * when the given element type unmaches the name
         *
         * @tparam Element the egzamined component type
         * @tparam Rest the rest of the components types
         * @param element the egzamined component object
         * @param rest  the rest of the components objects
         * @return deduced component type
         */
        template <typename Element, typename... Rest>
        static constexpr decltype(auto) deduceType(
            Element element, Rest... rest) noexcept;
    public:
        /// The type of the elements vertex component
        typedef decltype(deduceType(
            std::declval<Components>()...))             ComponentType;

        /// The type handled by the vertex component
        typedef typename ComponentType::BaseType        Type;
    };

    /**
     * Provides compile-time access to the indexed vertex component
     * type
     *
     * @tparam Index the index of the vertex component
     * @tparam Components the types of the vertex components
     */
    template <std::size_t Index, VertexComponents... Components>
        requires (Index < sizeof...(Components))
    struct VertexElement<Index, Vertex<Components...>> {
        /// The type of the elements vertex component
        typedef std::remove_reference_t<decltype(std::get<Index>(
            std::declval<std::tuple<Components...>>()
        ))>                                             ComponentType;

        /// The type handled by the vertex component
        typedef typename ComponentType::BaseType        Type;
    };

    /**
     * The convenient shortcut for the Vertex Element type
     *
     * @tparam Identifier the element identifier
     * @tparam VT the type of the vertex
     */
    template <auto Identifier, class VT>
    using VertexElementT =
        typename VertexElement<Identifier, VT>::Type;

    #pragma pack(push, 1)

    /**
     * Represents the vertex as the metastruct which allows to
     * compute at compile time the memory layout of the vertex
     *
     * @tparam Components the vertex components types
     */
    template <VertexComponents... Components>
    class Vertex : private ReversedTuple<Components...> {
    private:
        typedef ReversedTuple<Components...>            BaseTuple;
        typedef std::tuple<Components...>               ArgTuple;
    public:
        /**
         * The tuple containing the size [in represent type] of the
         * component, the offset from the begining of the layout
         * [in bits] to the component and the represented type
         */
        typedef std::tuple<uint16, uint32, DataType>    MetaTuple;
        typedef FixedRange<sizeof...(Components),
            std::array<MetaTuple,
            sizeof...(Components)>>                     MetaArray;

        /**
         * Constructs the Vertex object from the given arguments.
         * Passes the arguments to the element constructors
         *
         * @tparam Args the types of the arguments
         * @param args the passed arguments
         */
        template <typename... Args>
            requires std::constructible_from<ArgTuple, Args...>
        constexpr explicit Vertex(Args&&... args)
            : BaseTuple{tupleReverser(std::forward<Args>(args)...)} {}

        constexpr Vertex(Vertex const&) = default;
        constexpr Vertex(Vertex&&) = default;

        constexpr Vertex& operator=(Vertex const&) = default;
        constexpr Vertex& operator=(Vertex&&) = default;

        /**
         * Returns the reference to element handled by the vertex
         * component with the given name
         *
         * @tparam Name the name of the component
         * @return The reference to the element handled in the
         * vertex component
         */
        template <TemplateString Name>
        [[nodiscard]] constexpr VertexElementT<Name, Vertex>&
            get(void) & noexcept;

        /**
         * Returns the constant reference to element handled by
         * the vertex component with the given name
         *
         * @tparam Name the name of the component
         * @return The constant reference to the element handled
         * in the vertex component
         */
        template <TemplateString Name>
        [[nodiscard]] constexpr VertexElementT<Name, Vertex> const&
            get(void) const& noexcept;

        /**
         * Returns the reference to element handled by the vertex
         * component with the given name
         *
         * @tparam Name the name of the component
         * @return The reference to the element handled in the
         * vertex component
         */
        template <TemplateString Name>
        [[nodiscard]] constexpr VertexElementT<Name, Vertex>&
            get(void) && noexcept;

        /**
         * Returns the constant reference to element handled by
         * the vertex component with the given name
         *
         * @tparam Name the name of the component
         * @return The constant reference to the element handled in the
         * vertex component
         */
        template <TemplateString Name>
        [[nodiscard]] constexpr VertexElementT<Name, Vertex> const&
            get(void) const&& noexcept;

        /**
         * Returns the reference to element handled by the vertex
         * component with the given index
         *
         * @tparam Index the index of the component
         * @return The reference to the element handled in the
         * vertex component
         */
        template <std::size_t Index>
            requires (Index < sizeof...(Components))
        [[nodiscard]] constexpr VertexElementT<Index, Vertex>&
            get(void) & noexcept;

        /**
         * Returns the constant reference to element handled by
         * the vertex component with the given index
         *
         * @tparam Index the index of the component
         * @return The constant reference to the element handled
         * in the vertex component
         */
        template <std::size_t Index>
            requires (Index < sizeof...(Components))
        [[nodiscard]] constexpr VertexElementT<Index, Vertex> const&
            get(void) const& noexcept;

        /**
         * Returns the reference to element handled by the vertex
         * component with the given index
         *
         * @tparam Index the index of the component
         * @return The reference to the element handled in the
         * vertex component
         */
        template <std::size_t Index>
            requires (Index < sizeof...(Components))
        [[nodiscard]] constexpr VertexElementT<Index, Vertex>&
            get(void) && noexcept;

        /**
         * Returns the constant reference to element handled by
         * the vertex component with the given index
         *
         * @tparam Index the index of the component
         * @return The constant reference to the element handled in the
         * vertex component
         */
        template <std::size_t Index>
            requires (Index < sizeof...(Components))
        [[nodiscard]] constexpr VertexElementT<Index, Vertex> const&
            get(void) const&& noexcept;

        /**
         * Returns the reference to element handled by the vertex
         * component with the given name
         *
         * @tparam Name the name of the component
         * @param vertex the reference to the vertex
         * @return The reference to the element handled in the
         * vertex component
         */
        template <TemplateString Field>
        [[nodiscard]] friend constexpr VertexElementT<Field, Vertex>&
            get(Vertex& vertex) noexcept
        {
            using Under = VertexElement<Field,
                Vertex>::ComponentType;

            return std::get<Under>(static_cast<
                Vertex::BaseTuple&>(vertex)).element;
        }

        /**
         * Returns the constant reference to element handled by
         * the vertex component with the given name
         *
         * @tparam Name the name of the component
         * @param vertex the constant reference to the vertex
         * @return The constant reference to the element handled
         * in the vertex component
         */
        template <TemplateString Field>
        [[nodiscard]] friend constexpr
            VertexElementT<Field, Vertex> const&
                get(Vertex const& vertex) noexcept
        {
            using Under = VertexElement<Field,
                Vertex>::ComponentType;

            return std::get<Under>(static_cast<
                Vertex::BaseTuple const&>(vertex)).element;
        }

        /**
         * Returns the reference to element handled by the vertex
         * component with the given name
         *
         * @tparam Name the name of the component
         * @param vertex the rvalue to the vertex
         * @return The reference to the element handled in the
         * vertex component
         */
        template <TemplateString Field>
        [[nodiscard]] friend constexpr VertexElementT<Field, Vertex>&
            get(Vertex&& vertex) noexcept
        {
            using Under = VertexElement<Field,
                Vertex>::ComponentType;

            return std::get<Under>(static_cast<
                Vertex::BaseTuple&&>(vertex)).element;
        }

        /**
         * Returns the constant reference to element handled by
         * the vertex component with the given name
         *
         * @tparam Name the name of the component
         * @param vertex the rvalue to the vertex
         * @return The constant reference to the element handled in the
         * vertex component
         */
        template <TemplateString Field>
        [[nodiscard]] friend constexpr VertexElementT<Field, Vertex> const&
            get(Vertex const&& vertex) noexcept
        {
            using Under = VertexElement<Field,
                Vertex>::ComponentType;

            return std::get<Under>(static_cast<
                Vertex::BaseTuple const&&>(vertex)).element;
        }

        /**
         * Destroys the vertex object
         */
        constexpr ~Vertex(void) noexcept = default;
    private:
        /**
         * Constructs the array containing the memory layout of
         * the vertex
         *
         * @return the array with the memory layout
         */
        [[nodiscard]] static constexpr MetaArray
            constructMemoryLayout(void) noexcept;

        /**
         * Constructs the components layout in the meta array
         *
         * @param array the array with the memory layout
         */
        static constexpr void constructComponentsLayout(
            MetaArray& array) noexcept;

        /**
         * Constructs the components offsets in the meta array
         *
         * @param array the array with the memory layout
         */
        static constexpr void constructComponentsOffsets(
            MetaArray& array) noexcept;

        /**
         * Returns the size in bytes of the given vertex component
         * type
         *
         * @param type the given vertex component type
         * @return the size in bytes of the given type
         */
        [[nodiscard]] static constexpr uint8
            typeSize(DataType type) noexcept;
    public:
        /// contains the memory layout of the vertex metastruct
        static constexpr const MetaArray                memoryLayout
            = constructMemoryLayout();
    };

    #pragma pack(pop)

}

#include <MPGL/Core/Vertex/Vertex.tpp>

namespace std {

    template <mpgl::VertexComponents... Components>
    struct tuple_size<mpgl::Vertex<Components...>>
        : integral_constant<size_t, sizeof...(Components)> {};

    template <std::size_t Index, mpgl::VertexComponents... Components>
        requires (Index < sizeof...(Components))
    struct tuple_element<Index, mpgl::Vertex<Components...>> {
        using type = mpgl::VertexElementT<Index, mpgl::Vertex<
            Components...>>;
    };

}
