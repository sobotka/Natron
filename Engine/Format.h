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

#ifndef NATRON_ENGINE_FORMAT_H_
#define NATRON_ENGINE_FORMAT_H_

#include <string>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

CLANG_DIAG_OFF(deprecated)
#include <QtCore/QMetaType>
CLANG_DIAG_ON(deprecated)

#include "Engine/Rect.h"

/*This class is used to hold the format of a frame (its resolution).
 *Some formats have a name , e.g : 1920*1080 is full HD, etc...
 *It also holds a pixel aspect ratio so the viewer can display the
 *frame accordingly*/
class Format : public RectI {
	
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version;
        boost::serialization::void_cast_register<Format,RectI>(
                                                                static_cast<Format *>(NULL),
                                                                static_cast<RectI *>(NULL)
                                                               );
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RectI);
        ar & boost::serialization::make_nvp("Pixel_aspect_ratio",_pixel_aspect);
        ar & boost::serialization::make_nvp("Name",_name);
        
    }
	
public:
    Format(int l, int b, int r, int t, const std::string& name, double pa)
    : RectI(l,b,r,t)
    , _pixel_aspect(pa)
    , _name(name)
    {}

    Format(const RectI& rect)
        :RectI(rect)
        ,_pixel_aspect(1.)
        ,_name()
    {

    }
    
    Format(const Format& other)
    :RectI(other.left(),other.bottom(),other.right(),other.top())
    ,_pixel_aspect(other.getPixelAspect())
    ,_name(other.getName())
    {}
    
    Format()
    : RectI()
    , _pixel_aspect(1.0)
    , _name()
    {
    }
    
    virtual ~Format(){}
    
    const std::string& getName() const {return _name;}
    
    void setName(const std::string& n) {_name = n;}
    
    double getPixelAspect() const {return _pixel_aspect;}

    void setPixelAspect( double p) {_pixel_aspect = p;}
	
	Format operator=(const Format& other){
        set(other.left(), other.bottom(), other.right(), other.top());
        setName(other.getName());
        setPixelAspect(other.getPixelAspect());
        return *this;
    }
    bool operator==(const Format& other) const {
        return _pixel_aspect == other.getPixelAspect() &&
        left() == other.left() &&
        bottom() == other.bottom() &&
        right() == other.right() &&
        top() == other.top();
    }
    
  
    
private:
	double _pixel_aspect;
    std::string _name;
    
};

Q_DECLARE_METATYPE(Format);


#endif // NATRON_ENGINE_FORMAT_H_
