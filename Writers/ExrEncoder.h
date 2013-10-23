//  Powiter
//
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
*Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012. 
*contact: immarespond at gmail dot com
*
*/

#ifndef POWITER_WRITERS_WRITEEXR_H_
#define POWITER_WRITERS_WRITEEXR_H_

#include <string>
#include <vector>

#include "Writers/Encoder.h"
#include "Global/Macros.h"

class ComboBox_Knob;
class Separator_Knob;
class Writer;

/*This class is used by Writer to load the filetype-specific knobs.
 Due to the lifetime of Write objects, it's not possible to use the
 createKnobDynamically interface there. We must use an extra class.
 Class inheriting*/
class ExrEncoderKnobs : public EncoderKnobs{
    Separator_Knob* sepKnob;
    ComboBox_Knob *compressionCBKnob;
    ComboBox_Knob *depthCBKnob ;
    
public:
    
    std::string _dataType;
    std::string _compression;
    
    ExrEncoderKnobs(Writer* op):EncoderKnobs(op){}
    virtual ~ExrEncoderKnobs(){}
    
    virtual void initKnobs(const std::string& fileType) OVERRIDE;
    
    virtual void cleanUpKnobs() OVERRIDE;
    
    virtual bool allValid() OVERRIDE;
};


class ExrEncoder :public Encoder{
public:
    
    static Encoder* BuildWrite(Writer* writer){return new ExrEncoder(writer);}
    
    ExrEncoder(Writer* writer);
    virtual ~ExrEncoder();
    
    virtual EncoderKnobs* initSpecificKnobs() OVERRIDE {return new ExrEncoderKnobs(_writer);}
    
    /*Should return the name of the write handle : "ffmpeg", "OpenEXR" ...*/
    virtual std::string encoderName() const OVERRIDE {return "OpenEXR";}
    
    /*Should return the list of file types supported by the encoder: "png","jpg", etc..*/
    virtual std::vector<std::string> fileTypesEncoded() const OVERRIDE;
    
    /*Must be implemented to tell whether this file type supports stereovision*/
	virtual bool supports_stereo() const OVERRIDE {return true;}
    
    
    /*Must implement it to initialize the appropriate colorspace  for
     the file type. You can initialize the _lut member by calling the
     function Powiter::Color::getLut(datatype) */
    virtual void initializeColorSpace() OVERRIDE;
    
    /*This must be implemented to do the output colorspace conversion*/
	virtual void renderRow(SequenceTime time,int left,int right,int y,const Powiter::ChannelSet& channels) OVERRIDE;
    
    /*This function initialises the output file/output storage structure and put necessary info in it, like
     meta-data, channels, etc...This is called on the main thread so don't do any extra processing here,
     otherwise it would stall the GUI.*/
    virtual void setupFile(const QString& filename,const Box2D& rod) OVERRIDE;
    
    /*This function must fill the pre-allocated structure with the data calculated by engine.
     This function must close the file as writeAllData is the LAST function called before the
     destructor of Write.*/
    virtual void writeAllData() OVERRIDE;
    
    /*Doesn't throw any exception since OpenEXR can write all channels*/
    virtual void supportsChannelsForWriting(Powiter::ChannelSet&) const OVERRIDE {}
    
    void debug();
    
private:
    struct Implementation;
    boost::scoped_ptr<Implementation> _imp; // hide implementation details
};

#endif /* defined(POWITER_WRITERS_WRITEEXR_H_) */