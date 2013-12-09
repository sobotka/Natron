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

#ifndef NODESERIALIZATION_H
#define NODESERIALIZATION_H

#include <map>
#include <string>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/version.hpp>

#include "Engine/CurveSerialization.h"
#include "Engine/Curve.h"

namespace Natron {
    class Node;
}

class NodeSerialization {
    
    
public:
    
    typedef std::map< std::string,AnimatingParam> KnobValues;
    
    NodeSerialization(){}
    
    ~NodeSerialization(){ _knobsValues.clear(); _inputs.clear(); }
    
    void initialize(const Natron::Node* n);
    
    const KnobValues& getKnobsValues() const {return _knobsValues;}
    
    const std::string& getPluginLabel() const {return _pluginLabel;}
    
    const std::string& getPluginID() const {return _pluginID;}
    
    const std::map<int,std::string>& getInputs() const {return _inputs;}
    
    
private:
    
    KnobValues _knobsValues;
    std::string _pluginLabel;
    std::string _pluginID;
    
    std::map<int,std::string> _inputs;

    
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version;
        ar & boost::serialization::make_nvp("Knobs_values_map", _knobsValues);
        ar & boost::serialization::make_nvp("Node_instance_unique_name",_pluginLabel);
        ar & boost::serialization::make_nvp("Node_class_name",_pluginID);
        ar & boost::serialization::make_nvp("Input_nodes_map",_inputs);
    }
    
    
};

BOOST_CLASS_VERSION(NodeSerialization, 1)


#endif // NODESERIALIZATION_H