//  Natron
//
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
*Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012.
*contact: immarespond at gmail dot com
*
*/
#ifndef CURVEPRIVATE_H
#define CURVEPRIVATE_H

#include <boost/shared_ptr.hpp>

#include "Engine/Variant.h"


class Curve;
struct KeyFramePrivate{

    Variant _value; /// the value held by the key
    double _time; /// a value ranging between 0 and 1

    Variant _leftTangent,_rightTangent;
    Natron::KeyframeType _interpolation;
    Curve* _curve;


    KeyFramePrivate()
    : _value()
    , _time(0)
    , _interpolation(Natron::KEYFRAME_LINEAR)
    , _curve(NULL)
    {}


    KeyFramePrivate(double time, const Variant& initialValue,Curve* curve)
        : _value(initialValue)
        , _time(time)
        , _leftTangent()
        , _rightTangent()
        , _interpolation(Natron::KEYFRAME_SMOOTH)
        , _curve(curve)
    {
        _leftTangent = initialValue;
        _rightTangent = initialValue;
    }

    KeyFramePrivate(const KeyFramePrivate& other): _curve(other._curve)
    {
        clone(other._value,other._time,other._leftTangent,other._rightTangent,other._interpolation);
    }

    void clone(const Variant& oValue,double oTime,const Variant& oLeftTan,const Variant& oRightTan,
               Natron::KeyframeType oInterp){
        _value = oValue;
        _time = oTime;
        _rightTangent = oRightTan;
        _leftTangent = oLeftTan;
        _interpolation = oInterp;
    }
};


class KeyFrame;
class AnimatingParam;
struct CurvePrivate{

    std::list< KeyFrame* >  _keyFrames;
    AnimatingParam* _owner;

    CurvePrivate()
    : _keyFrames()
    , _owner(NULL)
    {}

};

struct AnimatingParamPrivate{

     /* A variant storing all the values in any dimension. <dimension,value>*/
    std::map<int,Variant> _value;

    int _dimension;

    /* the keys for a specific dimension*/
    std::map<int, boost::shared_ptr<Curve> > _curves;

    AnimatingParamPrivate(int dimension = 1):
        _value()
      , _dimension(dimension)
      , _curves()
    {

    }
};


#endif // CURVEPRIVATE_H