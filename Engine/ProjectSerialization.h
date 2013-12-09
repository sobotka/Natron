//  Natron
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
 *Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012.
 *contact: immarespond at gmail dot com
 *
 */


#ifndef PROJECTSERIALIZATION_H
#define PROJECTSERIALIZATION_H

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/scoped_ptr.hpp>

#include "Engine/ProjectPrivate.h"
#include "Engine/Project.h"
#include "Engine/TimeLine.h"
#include "Engine/Node.h"
#include "Engine/NodeSerialization.h"


class ProjectSerialization{
    
    std::vector< boost::shared_ptr<NodeSerialization> > _serializedNodes;
    std::vector<Format> _availableFormats;
    std::map< std::string,AnimatingParam> _projectKnobs;
    SequenceTime _timelineLeft,_timelineRight,_timelineCurrent;
    
public:
    
    ProjectSerialization(){}
    
    ~ProjectSerialization(){ _serializedNodes.clear(); }
    
    void initialize(const Natron::Project* project);
    
    SequenceTime getCurrentTime() const { return _timelineCurrent; }
    
    SequenceTime getLeftBoundTime() const { return _timelineLeft; }
        
    SequenceTime getRightBoundTime() const { return _timelineRight; }
    
    const std::map< std::string,AnimatingParam>& getProjectKnobsValues() const { return _projectKnobs; }
    
    const std::vector<Format>& getProjectFormats() const { return _availableFormats; }
    
    const std::vector< boost::shared_ptr<NodeSerialization> >& getNodesSerialization() const { return _serializedNodes; }
    
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version;
        ar & boost::serialization::make_nvp("Nodes",_serializedNodes);
        ar & boost::serialization::make_nvp("ProjectKnobs",_projectKnobs);
        ar & boost::serialization::make_nvp("All_formats",_availableFormats);
        ar & boost::serialization::make_nvp("Timeline_current_time",_timelineCurrent);
        ar & boost::serialization::make_nvp("Timeline_left_bound",_timelineLeft);
        ar & boost::serialization::make_nvp("Timeline_right_bound",_timelineRight);
    }
    
};

#endif // PROJECTSERIALIZATION_H