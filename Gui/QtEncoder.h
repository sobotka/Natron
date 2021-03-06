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

#ifndef NATRON_WRITERS_WRITEQT_H_
#define NATRON_WRITERS_WRITEQT_H_


#include "Engine/EffectInstance.h"

namespace Natron {
    namespace Color {
        class Lut;
    }
}

class OutputFile_Knob;
class Choice_Knob;
class Button_Knob;
class Int_Knob;
class Bool_Knob;

class QtWriter :public Natron::OutputEffectInstance{
    
public:
    static Natron::EffectInstance* BuildEffect(boost::shared_ptr<Natron::Node> n){
        return new QtWriter(n);
    }
    
    QtWriter(boost::shared_ptr<Natron::Node> node);
    
    virtual ~QtWriter();
    
    virtual bool isWriter() const OVERRIDE FINAL WARN_UNUSED_RETURN { return true; }
    
    static void supportedFileFormats_static(std::vector<std::string>* formats);
    
    virtual std::vector<std::string> supportedFileFormats() const OVERRIDE FINAL;
    
    virtual bool isInputOptional(int /*inputNb*/) const OVERRIDE {return false;}
    
    virtual int majorVersion() const OVERRIDE { return 1; }
    
    virtual int minorVersion() const OVERRIDE { return 0;}
    
    virtual std::string pluginID() const OVERRIDE;
    
    virtual std::string pluginLabel() const OVERRIDE;

    virtual void pluginGrouping(std::list<std::string>* grouping) const OVERRIDE FINAL;

    virtual std::string description() const OVERRIDE;
    
    virtual void getFrameRange(SequenceTime *first,SequenceTime *last) OVERRIDE;

    virtual int maximumInputs() const OVERRIDE {return 1;}
    
    void knobChanged(KnobI* k,Natron::ValueChangedReason reason,const RectI& rod,int view,SequenceTime time) OVERRIDE FINAL;

    virtual Natron::Status render(SequenceTime /*time*/, RenderScale /*scale*/, const RectI& /*roi*/, int /*view*/,
                                  bool /*isSequentialRender*/,bool /*isRenderResponseToUserInteraction*/,
                                  boost::shared_ptr<Natron::Image> /*output*/) OVERRIDE;

    virtual void addAcceptedComponents(int inputNb,std::list<Natron::ImageComponents>* comps) OVERRIDE FINAL;

    virtual void addSupportedBitDepth(std::list<Natron::ImageBitDepth>* depths) const OVERRIDE FINAL;
protected:
    
    
	virtual void initializeKnobs() OVERRIDE;
    
    virtual Natron::EffectInstance::RenderSafety renderThreadSafety() const OVERRIDE {return Natron::EffectInstance::INSTANCE_SAFE;}

private:

    const Natron::Color::Lut* _lut;

    boost::shared_ptr<Bool_Knob> _premultKnob;
    boost::shared_ptr<OutputFile_Knob> _fileKnob;
    boost::shared_ptr<Choice_Knob> _frameRangeChoosal;
    boost::shared_ptr<Int_Knob> _firstFrameKnob;
    boost::shared_ptr<Int_Knob> _lastFrameKnob;
    boost::shared_ptr<Button_Knob> _renderKnob;

};

#endif /* defined(NATRON_WRITERS_WRITEQT_H_) */
