// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2015 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2013-2024.
// Modifications copyright (c) 2013-2024 Oracle and/or its affiliates.
// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle
// Contributed and/or modified by Menelaos Karavelas, on behalf of Oracle
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_DETAIL_RELATE_TURNS_HPP
#define BOOST_GEOMETRY_ALGORITHMS_DETAIL_RELATE_TURNS_HPP


#include <boost/geometry/algorithms/detail/overlay/do_reverse.hpp>
#include <boost/geometry/algorithms/detail/overlay/get_turns.hpp>
#include <boost/geometry/algorithms/detail/overlay/get_turn_info.hpp>

#include <boost/geometry/geometries/helper_geometry.hpp>

#include <boost/geometry/strategies/cartesian/point_in_point.hpp>
#include <boost/geometry/strategies/spherical/point_in_point.hpp>
#include <boost/geometry/strategies/distance.hpp>


namespace boost { namespace geometry {

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace relate { namespace turns {

template <bool IncludeDegenerate = false>
struct assign_policy
    : overlay::assign_null_policy
{
    static bool const include_degenerate = IncludeDegenerate;
};

// turn retriever, calling get_turns

template
<
    typename Geometry1,
    typename Geometry2,
    typename GetTurnPolicy = detail::get_turns::get_turn_info_type
        <
            Geometry1, Geometry2, assign_policy<>
        >
>
struct get_turns
{
    using turn_point_type = typename helper_geometry
        <
            geometry::point_type_t<Geometry1>
        >::type;

    template
    <
        typename Strategy
    >
    struct turn_info_type
    {
        using ratio_type = typename segment_ratio_type<turn_point_type>::type;
        using type = overlay::turn_info
            <
                turn_point_type,
                ratio_type,
                typename detail::get_turns::turn_operation_type
                    <
                        Geometry1, Geometry2, turn_point_type, ratio_type
                    >::type
            >;
    };

    template <typename Turns, typename InterruptPolicy, typename Strategy>
    static inline void apply(Turns & turns,
                             Geometry1 const& geometry1,
                             Geometry2 const& geometry2,
                             InterruptPolicy & interrupt_policy,
                             Strategy const& strategy)
    {
        static const bool reverse1 = detail::overlay::do_reverse
            <
                geometry::point_order<Geometry1>::value
            >::value;

        static const bool reverse2 = detail::overlay::do_reverse
            <
                geometry::point_order<Geometry2>::value
            >::value;

        dispatch::get_turns
            <
                geometry::tag_t<Geometry1>,
                geometry::tag_t<Geometry2>,
                Geometry1,
                Geometry2,
                reverse1,
                reverse2,
                GetTurnPolicy
            >::apply(0, geometry1, 1, geometry2,
                     strategy,
                     turns, interrupt_policy);
    }
};

// TURNS SORTING AND SEARCHING

template <int N = 0, int U = 1, int I = 2, int B = 3, int C = 4, int O = 0>
struct op_to_int
{
    template <typename Operation>
    inline int operator()(Operation const& op) const
    {
        switch(op.operation)
        {
        case detail::overlay::operation_none : return N;
        case detail::overlay::operation_union : return U;
        case detail::overlay::operation_intersection : return I;
        case detail::overlay::operation_blocked : return B;
        case detail::overlay::operation_continue : return C;
        case detail::overlay::operation_opposite : return O;
        }
        return -1;
    }
};

template <std::size_t OpId, typename OpToInt>
struct less_op_xxx_linear
{
    template <typename Turn>
    inline bool operator()(Turn const& left, Turn const& right) const
    {
        static OpToInt op_to_int;
        return op_to_int(left.operations[OpId]) < op_to_int(right.operations[OpId]);
    }
};

template <std::size_t OpId>
struct less_op_linear_linear
    : less_op_xxx_linear< OpId, op_to_int<0,2,3,1,4,0> > // xuic
{};

template <std::size_t OpId>
struct less_op_linear_areal_single
{
    template <typename Turn>
    inline bool operator()(Turn const& left, Turn const& right) const
    {
        static const std::size_t other_op_id = (OpId + 1) % 2;
        static turns::op_to_int<0,2,3,1,4,0> op_to_int_xuic;
        static turns::op_to_int<0,3,2,1,4,0> op_to_int_xiuc;

        segment_identifier const& left_other_seg_id = left.operations[other_op_id].seg_id;
        segment_identifier const& right_other_seg_id = right.operations[other_op_id].seg_id;

        typedef typename Turn::turn_operation_type operation_type;
        operation_type const& left_operation = left.operations[OpId];
        operation_type const& right_operation = right.operations[OpId];

        if ( left_other_seg_id.ring_index == right_other_seg_id.ring_index )
        {
            return op_to_int_xuic(left_operation)
                 < op_to_int_xuic(right_operation);
        }
        else
        {
            return op_to_int_xiuc(left_operation)
                 < op_to_int_xiuc(right_operation);
        }
    }
};

template <std::size_t OpId>
struct less_op_areal_linear
    : less_op_xxx_linear< OpId, op_to_int<0,1,0,0,2,0> >
{};

template <std::size_t OpId>
struct less_op_areal_areal
{
    template <typename Turn>
    inline bool operator()(Turn const& left, Turn const& right) const
    {
        static const std::size_t other_op_id = (OpId + 1) % 2;
        static op_to_int<0, 1, 2, 3, 4, 0> op_to_int_uixc;
        static op_to_int<0, 2, 1, 3, 4, 0> op_to_int_iuxc;

        segment_identifier const& left_other_seg_id = left.operations[other_op_id].seg_id;
        segment_identifier const& right_other_seg_id = right.operations[other_op_id].seg_id;

        typedef typename Turn::turn_operation_type operation_type;
        operation_type const& left_operation = left.operations[OpId];
        operation_type const& right_operation = right.operations[OpId];

        if ( left_other_seg_id.multi_index == right_other_seg_id.multi_index )
        {
            if ( left_other_seg_id.ring_index == right_other_seg_id.ring_index )
            {
                return op_to_int_uixc(left_operation) < op_to_int_uixc(right_operation);
            }
            else
            {
                if ( left_other_seg_id.ring_index == -1 )
                {
                    if ( left_operation.operation == overlay::operation_union )
                        return false;
                    else if ( left_operation.operation == overlay::operation_intersection )
                        return true;
                }
                else if ( right_other_seg_id.ring_index == -1 )
                {
                    if ( right_operation.operation == overlay::operation_union )
                        return true;
                    else if ( right_operation.operation == overlay::operation_intersection )
                        return false;
                }

                return op_to_int_iuxc(left_operation) < op_to_int_iuxc(right_operation);
            }
        }
        else
        {
            return op_to_int_uixc(left_operation) < op_to_int_uixc(right_operation);
        }
    }
};

template <std::size_t OpId>
struct less_other_multi_index
{
    static const std::size_t other_op_id = (OpId + 1) % 2;

    template <typename Turn>
    inline bool operator()(Turn const& left, Turn const& right) const
    {
        return left.operations[other_op_id].seg_id.multi_index
             < right.operations[other_op_id].seg_id.multi_index;
    }
};

// sort turns by G1 - source_index == 0 by:
// seg_id -> distance and coordinates -> operation
template <std::size_t OpId, typename LessOp, typename Strategy>
struct less
{
    BOOST_STATIC_ASSERT(OpId < 2);

    template <typename Turn>
    static inline bool use_fraction(Turn const& left, Turn const& right)
    {
        using eq_pp_strategy_type = decltype(std::declval<Strategy>().relate(
            detail::dummy_point(), detail::dummy_point()));

        static LessOp less_op;

        // NOTE: Assuming fraction is more permissive and faster than
        //       comparison of points with strategy.
        return geometry::math::equals(left.operations[OpId].fraction,
                                      right.operations[OpId].fraction)
                && eq_pp_strategy_type::apply(left.point, right.point)
             ?
             less_op(left, right)
             :
             (left.operations[OpId].fraction < right.operations[OpId].fraction)
             ;
    }

    template <typename Turn>
    inline bool operator()(Turn const& left, Turn const& right) const
    {
        segment_identifier const& sl = left.operations[OpId].seg_id;
        segment_identifier const& sr = right.operations[OpId].seg_id;

        return sl < sr || ( sl == sr && use_fraction(left, right) );
    }
};

}}} // namespace detail::relate::turns
#endif // DOXYGEN_NO_DETAIL

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_ALGORITHMS_DETAIL_RELATE_TURNS_HPP
