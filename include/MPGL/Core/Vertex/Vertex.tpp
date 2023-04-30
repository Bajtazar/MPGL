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

namespace mpgl {

    template <VertexComponents... Components>
    [[nodiscard]] constexpr Vertex<Components...>::MetaArray
        Vertex<Components...>::constructMemoryLayout(void) noexcept
    {
        MetaArray array;
        constructComponentsLayout(array);
        constructComponentsOffsets(array);
        return array;
    }

    template <VertexComponents... Components>
    constexpr void Vertex<Components...>::constructComponentsLayout(
        MetaArray& array) noexcept
    {
        [&array]<std::size_t... Index> (std::index_sequence<Index...>) {
            ([&array]<std::size_t ID>(
                std::integral_constant<std::size_t, ID>)
            {
                std::get<2>(array[ID]) = VertexElement<ID, Vertex
                    >::ComponentType::vertexType();
                std::get<0>(array[ID]) = sizeof(
                    VertexElementT<ID, Vertex>) / typeSize(
                        std::get<2>(array[ID]));
            }(std::integral_constant<std::size_t, Index>{}), ...);
        }(std::make_index_sequence<sizeof...(Components)>{});
    }

    template <VertexComponents... Components>
    constexpr void Vertex<Components...>::constructComponentsOffsets(
        MetaArray& array) noexcept
    {
        for (std::size_t i = 1; i < array.size(); ++i)
            std::get<1>(array[i]) = std::get<1>(array[i - 1])
                + std::get<0>(array[i - 1])
                    * typeSize(std::get<2>(array[i - 1]));
    }

    template <VertexComponents... Components>
    [[nodiscard]] constexpr uint8 Vertex<Components...>::typeSize(
        DataType type) noexcept
    {
        switch (type) {
            case DataType::Int8:
            case DataType::UInt8:
                return 1;
            case DataType::Int16:
            case DataType::UInt16:
            case DataType::Float16:
                return 2;
            case DataType::Int32:
            case DataType::UInt32:
            case DataType::Float32:
            case DataType::Fixed:
                return 4;
            case DataType::Float64:
                return 8;
        }
        return 0;
    }

    template <TemplateString Name, VertexComponents... Components>
    template <typename Element>
    constexpr decltype(auto)
        VertexElement<Name, Vertex<Components...>>::deduceType(
            Element element) noexcept
    {
        static_assert(Name == Element::componentName(),
            "Invalid field name");
        return element;
    }

    template <TemplateString Name, VertexComponents... Components>
    template <typename Element, typename... Rest>
    constexpr decltype(auto)
        VertexElement<Name, Vertex<Components...>>::deduceType(
            Element element, Rest... rest) noexcept
    {
        if constexpr (Name == Element::componentName())
            return element;
        else
            return deduceType(rest...);
    }

    template <VertexComponents... Components>
    template <TemplateString Name>
    [[nodiscard]] constexpr
        VertexElementT<Name, Vertex<Components...>>&
            Vertex<Components...>::get(void) & noexcept
    {
        using Under = VertexElement<Name, Vertex>::ComponentType;

        return std::get<Under>(static_cast<
            BaseTuple&>(*this)).element;
    }

    template <VertexComponents... Components>
    template <TemplateString Name>
    [[nodiscard]] constexpr
        VertexElementT<Name, Vertex<Components...>> const&
            Vertex<Components...>::get(void) const& noexcept
    {
        using Under = VertexElement<Name, Vertex>::ComponentType;

        return std::get<Under>(static_cast<
            BaseTuple const&>(*this)).element;
    }

    template <VertexComponents... Components>
    template <TemplateString Name>
    [[nodiscard]] constexpr
        VertexElementT<Name, Vertex<Components...>>&
            Vertex<Components...>::get(void) && noexcept
    {
        using Under = VertexElement<Name, Vertex>::ComponentType;

        return std::get<Under>(static_cast<
            BaseTuple&&>(*this)).element;
    }

    template <VertexComponents... Components>
    template <TemplateString Name>
    [[nodiscard]] constexpr
        VertexElementT<Name, Vertex<Components...>> const&
            Vertex<Components...>::get(void) const&& noexcept
    {
        using Under = VertexElement<Name, Vertex>::ComponentType;

        return std::get<Under>(static_cast<
            BaseTuple const&&>(*this)).element;
    }

    template <TemplateString Name, VertexComponents... Comps>
    [[nodiscard]] constexpr
        VertexElementT<Name, Vertex<Comps...>>&
            get(Vertex<Comps...>&& vertex) noexcept
    {
        using Under = VertexElement<Name,
            Vertex<Comps...>>::ComponentType;

        return std::get<Under>(static_cast<
            Vertex<Comps...>::BaseTuple&&>(vertex)).element;
    }

    template <TemplateString Name, VertexComponents... Comps>
    [[nodiscard]] constexpr
        VertexElementT<Name, Vertex<Comps...>> const&
            get(Vertex<Comps...> const&& vertex) noexcept
    {
        using Under = VertexElement<Name,
            Vertex<Comps...>>::ComponentType;

        return std::get<Under>(static_cast<
            Vertex<Comps...>::BaseTuple const&&>(vertex)).element;
    }

    template <VertexComponents... Components>
    template <std::size_t Index>
        requires (Index < sizeof...(Components))
    [[nodiscard]] constexpr
        VertexElementT<Index, Vertex<Components...>>&
            Vertex<Components...>::get(void) & noexcept
    {
        using Under = VertexElement<Index, Vertex>::ComponentType;

        return std::get<Under>(static_cast<
            BaseTuple&>(*this)).element;
    }

    template <VertexComponents... Components>
    template <std::size_t Index>
        requires (Index < sizeof...(Components))
    [[nodiscard]] constexpr
        VertexElementT<Index, Vertex<Components...>> const&
            Vertex<Components...>::get(void) const& noexcept
    {
        using Under = VertexElement<Index, Vertex>::ComponentType;

        return std::get<Under>(static_cast<
            BaseTuple const&>(*this)).element;
    }

    template <VertexComponents... Components>
    template <std::size_t Index>
        requires (Index < sizeof...(Components))
    [[nodiscard]] constexpr
        VertexElementT<Index, Vertex<Components...>>&
            Vertex<Components...>::get(void) && noexcept
    {
        using Under = VertexElement<Index, Vertex>::ComponentType;

        return std::get<Under>(static_cast<
            BaseTuple&&>(*this)).element;
    }

    template <VertexComponents... Components>
    template <std::size_t Index>
        requires (Index < sizeof...(Components))
    [[nodiscard]] constexpr
        VertexElementT<Index, Vertex<Components...>> const&
            Vertex<Components...>::get(void) const&& noexcept
    {
        using Under = VertexElement<Index, Vertex>::ComponentType;

        return std::get<Under>(static_cast<
            BaseTuple const&&>(*this)).element;
    }

}
