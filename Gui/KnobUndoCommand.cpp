//  Natron
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Gui/KnobUndoCommand.h"

#include "Global/GlobalDefines.h" // SequenceTime
#include "Global/AppManager.h" // AppInstance

#include "Engine/TimeLine.h"
#include "Engine/Curve.h"
#include "Engine/Knob.h"

#include "Gui/KnobGui.h"

//================================================================


KnobMultipleUndosCommand::KnobMultipleUndosCommand(KnobGui *knob, const std::map<int, Variant> &oldValue, const std::map<int, Variant> &newValue, QUndoCommand *parent)
: QUndoCommand(parent)
, _oldValue(oldValue)
, _newValue(newValue)
, _knob(knob)
{
}

void KnobMultipleUndosCommand::undo()
{
    for (std::map<int, Variant>::const_iterator it = _oldValue.begin(); it != _oldValue.end(); ++it) {
        _knob->setValue(it->first, it->second);
        //maybe we should also attempt to remove the keyframe if we added one in the redo() function ?
        // Anyway the user can remove it by pressing CTRL-Z in the curve editor
    }
    setText(QObject::tr("Set value of %1")
            .arg(_knob->getKnob()->getDescription().c_str()));
}

void KnobMultipleUndosCommand::redo()
{

    SequenceTime time = _knob->getKnob()->getHolder()->getApp()->getTimeLine()->currentFrame();

    for (std::map<int, Variant>::const_iterator it = _newValue.begin(); it != _newValue.end(); ++it) {
        boost::shared_ptr<Curve> c = _knob->getKnob()->getCurve(it->first);

        _knob->setValue(it->first, it->second);

        if (c->keyFramesCount() >= 1) {
            _knob->setKeyframe(time, it->first);
        }

    }
    setText(QObject::tr("Set value of %1")
            .arg(_knob->getKnob()->getDescription().c_str()));
}


KnobUndoCommand::KnobUndoCommand(KnobGui *knob, int dimension, const Variant &oldValue, const Variant &newValue, QUndoCommand *parent)
    : QUndoCommand(parent),
      _dimension(dimension),
      _oldValue(oldValue),
      _newValue(newValue),
      _knob(knob)
{
}

void KnobUndoCommand::undo()
{

    _knob->setValue(_dimension, _oldValue);

    //maybe we should also attempt to remove the keyframe if we added one in the redo() function ?
    // Anyway the user can remove it by pressing CTRL-Z in the curve editor

    if (_knob->getKnob()->getDimension() > 1) {
        setText(QObject::tr("Set value of %1.%2")
                .arg(_knob->getKnob()->getDescription().c_str()).arg(_knob->getKnob()->getDimensionName(_dimension).c_str()));
    } else {
        setText(QObject::tr("Set value of %1")
                .arg(_knob->getKnob()->getDescription().c_str()));
    }
}

void KnobUndoCommand::redo()
{

    boost::shared_ptr<Curve> c = _knob->getKnob()->getCurve(_dimension);
    SequenceTime time = _knob->getKnob()->getHolder()->getApp()->getTimeLine()->currentFrame();
    _knob->setValue(_dimension, _newValue);

    //the curve is animated, attempt to set a keyframe
    if (c->keyFramesCount() >= 1) {
        _knob->setKeyframe(time, _dimension);
    }

    if (_knob->getKnob()->getDimension() > 1) {
        setText(QObject::tr("Set value of %1.%2")
                .arg(_knob->getKnob()->getDescription().c_str()).arg(_knob->getKnob()->getDimensionName(_dimension).c_str()));
    } else {
        setText(QObject::tr("Set value of %1")
                .arg(_knob->getKnob()->getDescription().c_str()));
    }
}

int KnobUndoCommand::id() const
{
    return kKnobUndoChangeCommandCompressionID;
}

bool KnobUndoCommand::mergeWith(const QUndoCommand *command)
{
    const KnobUndoCommand *knobCommand = dynamic_cast<const KnobUndoCommand *>(command);
    if (!knobCommand || command->id() != id()) {
        return false;
    }

    KnobGui *knob = knobCommand->_knob;
    if (_knob != knob) {
        return false;
    }
    _newValue = knobCommand->_newValue;
    return true;
}
