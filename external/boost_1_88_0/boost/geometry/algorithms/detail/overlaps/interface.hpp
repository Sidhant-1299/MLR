// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2015 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2015 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2015 Mateusz Loskot, London, UK.

// This file was modified by Oracle on 2014-2022.
// Modifications copyright (c) 2014-2022 Oracle and/or its affiliates.

// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_DETAIL_OVERLAPS_INTERFACE_HPP
#define BOOST_GEOMETRY_ALGORITHMS_DETAIL_OVERLAPS_INTERFACE_HPP


#include <cstddef>

#include <boost/geometry/algorithms/not_implemented.hpp>

#include <boost/geometry/geometries/adapted/boost_variant.hpp>
#include <boost/geometry/geometries/concepts/check.hpp>

#include <boost/geometry/algorithms/detail/relate/relate_impl.hpp>

#include <boost/geometry/strategies/default_strategy.hpp>
#include <boost/geometry/strategies/detail.hpp>
#include <boost/geometry/strategies/relate/services.hpp>


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{


template
<
    typename Geometry1,
    typename Geometry2,
    typename Tag1 = tag_t<Geometry1>,
    typename Tag2 = tag_t<Geometry2>
>
struct overlaps
    : detail::relate::relate_impl
        <
            detail::de9im::static_mask_overlaps_type,
            Geometry1,
            Geometry2
        >
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


namespace resolve_strategy
{

template
<
    typename Strategy,
    bool IsUmbrella = strategies::detail::is_umbrella_strategy<Strategy>::value
>
struct overlaps
{
    template <typename Geometry1, typename Geometry2>
    static inline bool apply(Geometry1 const& geometry1,
                             Geometry2 const& geometry2,
                             Strategy const& strategy)
    {
        return dispatch::overlaps
            <
                Geometry1, Geometry2
            >::apply(geometry1, geometry2, strategy);
    }
};

template <typename Strategy>
struct overlaps<Strategy, false>
{
    template <typename Geometry1, typename Geometry2>
    static inline bool apply(Geometry1 const& geometry1,
                             Geometry2 const& geometry2,
                             Strategy const& strategy)
    {
        using strategies::relate::services::strategy_converter;
        return dispatch::overlaps
            <
                Geometry1, Geometry2
            >::apply(geometry1, geometry2,
                     strategy_converter<Strategy>::get(strategy));
    }
};

template <>
struct overlaps<default_strategy, false>
{
    template <typename Geometry1, typename Geometry2>
    static inline bool apply(Geometry1 const& geometry1,
                             Geometry2 const& geometry2,
                             default_strategy)
    {
        typedef typename strategies::relate::services::default_strategy
            <
                Geometry1, Geometry2
            >::type strategy_type;

        return dispatch::overlaps
            <
                Geometry1, Geometry2
            >::apply(geometry1, geometry2, strategy_type());
    }
};

} // namespace resolve_strategy


namespace resolve_dynamic
{

template
<
    typename Geometry1, typename Geometry2,
    typename Tag1 = geometry::tag_t<Geometry1>,
    typename Tag2 = geometry::tag_t<Geometry2>
>
struct overlaps
{
    template <typename Strategy>
    static inline bool apply(Geometry1 const& geometry1,
                             Geometry2 const& geometry2,
                             Strategy const& strategy)
    {
        return resolve_strategy::overlaps
            <
                Strategy
            >::apply(geometry1, geometry2, strategy);
    }
};


template <typename DynamicGeometry1, typename Geometry2, typename Tag2>
struct overlaps<DynamicGeometry1, Geometry2, dynamic_geometry_tag, Tag2>
{
    template <typename Strategy>
    static inline bool apply(DynamicGeometry1 const& geometry1,
                             Geometry2 const& geometry2,
                             Strategy const& strategy)
    {
        bool result = false;
        traits::visit<DynamicGeometry1>::apply([&](auto const& g1)
        {
            result = resolve_strategy::overlaps
                <
                    Strategy
                >::apply(g1, geometry2, strategy);
        }, geometry1);
        return result;
    }
};


template <typename Geometry1, typename DynamicGeometry2, typename Tag1>
struct overlaps<Geometry1, DynamicGeometry2, Tag1, dynamic_geometry_tag>
{
    template <typename Strategy>
    static inline bool apply(Geometry1 const& geometry1,
                             DynamicGeometry2 const& geometry2,
                             Strategy const& strategy)
    {
        bool result = false;
        traits::visit<DynamicGeometry2>::apply([&](auto const& g2)
        {
            result = resolve_strategy::overlaps
                <
                    Strategy
                >::apply(geometry1, g2, strategy);
        }, geometry2);
        return result;
    }
};


template <typename DynamicGeometry1, typename DynamicGeometry2>
struct overlaps<DynamicGeometry1, DynamicGeometry2, dynamic_geometry_tag, dynamic_geometry_tag>
{
    template <typename Strategy>
    static inline bool apply(DynamicGeometry1 const& geometry1,
                             DynamicGeometry2 const& geometry2,
                             Strategy const& strategy)
    {
        bool result = false;
        traits::visit<DynamicGeometry1, DynamicGeometry2>::apply([&](auto const& g1, auto const& g2)
        {
            result = resolve_strategy::overlaps
                <
                    Strategy
                >::apply(g1, g2, strategy);
        }, geometry1, geometry2);
        return result;
    }
};


} // namespace resolve_dynamic


/*!
\brief \brief_check2{overlap}
\ingroup overlaps
\tparam Geometry1 \tparam_geometry
\tparam Geometry2 \tparam_geometry
\tparam Strategy \tparam_strategy{Overlaps}
\param geometry1 \param_geometry
\param geometry2 \param_geometry
\param strategy \param_strategy{overlaps}
\return \return_check2{overlap}

\qbk{distinguish,with strategy}
\qbk{[include reference/algorithms/overlaps.qbk]}
*/
template <typename Geometry1, typename Geometry2, typename Strategy>
inline bool overlaps(Geometry1 const& geometry1,
                     Geometry2 const& geometry2,
                     Strategy const& strategy)
{
    concepts::check<Geometry1 const>();
    concepts::check<Geometry2 const>();

    return resolve_dynamic::overlaps
        <
            Geometry1, Geometry2
        >::apply(geometry1, geometry2, strategy);
}

/*!
\brief \brief_check2{overlap}
\ingroup overlaps
\tparam Geometry1 \tparam_geometry
\tparam Geometry2 \tparam_geometry
\param geometry1 \param_geometry
\param geometry2 \param_geometry
\return \return_check2{overlap}

\qbk{[include reference/algorithms/overlaps.qbk]}
\qbk{
[heading Examples]
[overlaps]
[overlaps_output]
}
*/
template <typename Geometry1, typename Geometry2>
inline bool overlaps(Geometry1 const& geometry1, Geometry2 const& geometry2)
{
    concepts::check<Geometry1 const>();
    concepts::check<Geometry2 const>();

    return resolve_dynamic::overlaps
        <
            Geometry1, Geometry2
        >::apply(geometry1, geometry2, default_strategy());
}

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_ALGORITHMS_DETAIL_OVERLAPS_INTERFACE_HPP
