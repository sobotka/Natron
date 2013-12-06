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

#ifndef NATRON_GUI_KNOBGUITYPES_H_
#define NATRON_GUI_KNOBGUITYPES_H_

#include <vector> // Int_KnobGui

#include "Global/Macros.h"
#include "Global/GlobalDefines.h"

#include "Engine/Singleton.h"
#include "Engine/Knob.h"

#include "Gui/KnobGui.h"

// Qt
class QString;
class QFrame;
class QLabel;
class QGridLayout;
class QTextEdit;

// Engine
class Knob;

// Gui
class DockablePanel;
class LineEdit;
class Button;
class SpinBox;
class ComboBox;
class ScaleSlider;
class GroupBoxLabel;

// private classes, defined in KnobGuiTypes.cpp
class ClickableLabel;
class AnimatedCheckBox;

namespace Natron
{
class Node;
}

//================================

class Int_KnobGui : public KnobGui
{
    Q_OBJECT
public:

    static KnobGui *BuildKnobGui(Knob *knob, DockablePanel *container) {
        return new Int_KnobGui(knob, container);
    }


    Int_KnobGui(Knob *knob, DockablePanel *container);

    virtual ~Int_KnobGui() OVERRIDE FINAL;

public slots:

    void onSpinBoxValueChanged();

    void onSliderValueChanged(double);

    void onMinMaxChanged(int mini, int maxi, int index = 0);

    void onIncrementChanged(int incr, int index = 0);

private:

    virtual void createWidget(QGridLayout *layout, int row) OVERRIDE FINAL;

    void setMaximum(int);

    void setMinimum(int);

    virtual void _hide() OVERRIDE FINAL;

    virtual void _show() OVERRIDE FINAL;

    virtual void setEnabled() OVERRIDE FINAL;

    virtual void addToLayout(QHBoxLayout *layout) OVERRIDE FINAL;

    virtual void updateGUI(int dimension, const Variant &variant) OVERRIDE FINAL;

private:
    std::vector<std::pair<SpinBox *, QLabel *> > _spinBoxes;
    QLabel *_descriptionLabel;
    ScaleSlider *_slider;

};



//================================
class Bool_KnobGui : public KnobGui
{
    Q_OBJECT
public:

    static KnobGui *BuildKnobGui(Knob *knob, DockablePanel *container) {
        return new Bool_KnobGui(knob, container);
    }


    Bool_KnobGui(Knob *knob, DockablePanel *container): KnobGui(knob, container) {}

    virtual ~Bool_KnobGui() OVERRIDE FINAL;

public slots:

    void onCheckBoxStateChanged(bool);

private:

    virtual void createWidget(QGridLayout *layout, int row) OVERRIDE FINAL;

    virtual void _hide() OVERRIDE FINAL;

    virtual void _show() OVERRIDE FINAL;

    virtual void setEnabled() OVERRIDE FINAL;

    virtual void addToLayout(QHBoxLayout *layout) OVERRIDE FINAL;

    virtual void updateGUI(int dimension, const Variant &variant) OVERRIDE FINAL;

private:

    AnimatedCheckBox *_checkBox;
    ClickableLabel *_descriptionLabel;
};


//================================
class Double_KnobGui : public KnobGui
{
    Q_OBJECT
public:

    static KnobGui *BuildKnobGui(Knob *knob, DockablePanel *container) {
        return new Double_KnobGui(knob, container);
    }


    Double_KnobGui(Knob *knob, DockablePanel *container);

    virtual ~Double_KnobGui() OVERRIDE FINAL;

public slots:
    void onSpinBoxValueChanged();
    void onSliderValueChanged(double);

    void onMinMaxChanged(double mini, double maxi, int index = 0);
    void onIncrementChanged(double incr, int index = 0);
    void onDecimalsChanged(int deci, int index = 0);

private:

    virtual void createWidget(QGridLayout *layout, int row) OVERRIDE FINAL;

    void setMaximum(int);
    void setMinimum(int);

    virtual void _hide() OVERRIDE FINAL;

    virtual void _show() OVERRIDE FINAL;

    virtual void setEnabled() OVERRIDE FINAL;

    virtual void addToLayout(QHBoxLayout *layout);

    virtual void updateGUI(int dimension, const Variant &variant) OVERRIDE FINAL;

private:
    std::vector<std::pair<SpinBox *, QLabel *> > _spinBoxes;
    QLabel *_descriptionLabel;
    ScaleSlider *_slider;
};

//================================

class Button_KnobGui : public KnobGui
{
    Q_OBJECT
public:
    static KnobGui *BuildKnobGui(Knob *knob, DockablePanel *container) {
        return new Button_KnobGui(knob, container);
    }


    Button_KnobGui(Knob *knob, DockablePanel *container): KnobGui(knob, container) {}

    virtual ~Button_KnobGui() OVERRIDE FINAL;

public slots:

    void emitValueChanged();

private:

    virtual void createWidget(QGridLayout *layout, int row) OVERRIDE FINAL;

    virtual void _hide() OVERRIDE FINAL;

    virtual void _show() OVERRIDE FINAL;

    virtual void setEnabled() OVERRIDE FINAL;

    virtual void addToLayout(QHBoxLayout *layout) OVERRIDE FINAL;

    virtual void updateGUI(int dimension, const Variant &variant) OVERRIDE FINAL {(void)dimension; Q_UNUSED(variant);}


private:
    Button *_button;
};

//================================
class ComboBox_KnobGui : public KnobGui
{
    Q_OBJECT
public:
    static KnobGui *BuildKnobGui(Knob *knob, DockablePanel *container) {
        return new ComboBox_KnobGui(knob, container);
    }


    ComboBox_KnobGui(Knob *knob, DockablePanel *container);

    virtual ~ComboBox_KnobGui() OVERRIDE FINAL;

public slots:

    void onCurrentIndexChanged(int i);

    void onEntriesPopulated();

private:

    virtual void createWidget(QGridLayout *layout, int row) OVERRIDE FINAL;

    virtual void _hide() OVERRIDE FINAL;

    virtual void _show() OVERRIDE FINAL;

    virtual void setEnabled() OVERRIDE FINAL;

    virtual void addToLayout(QHBoxLayout *layout) OVERRIDE FINAL;

    virtual void updateGUI(int dimension, const Variant &variant) OVERRIDE FINAL;

private:
    std::vector<std::string> _entries;
    ComboBox *_comboBox;
    QLabel *_descriptionLabel;
};

//=========================
class Separator_KnobGui : public KnobGui
{
public:
    static KnobGui *BuildKnobGui(Knob *knob, DockablePanel *container) {
        return new Separator_KnobGui(knob, container);
    }

    Separator_KnobGui(Knob *knob, DockablePanel *container): KnobGui(knob, container) {}

    virtual ~Separator_KnobGui() OVERRIDE FINAL;

private:

    virtual void createWidget(QGridLayout *layout, int row) OVERRIDE FINAL;

    virtual void _hide() OVERRIDE FINAL;

    virtual void _show() OVERRIDE FINAL;

    virtual void setEnabled() OVERRIDE FINAL {}

    virtual void addToLayout(QHBoxLayout *layout) OVERRIDE FINAL;

    virtual void updateGUI(int dimension, const Variant &variant) OVERRIDE FINAL {(void)dimension; (void)variant;}

private:
    QFrame *_line;
    QLabel *_descriptionLabel;
};

/******************************/
class Color_KnobGui : public KnobGui
{
    Q_OBJECT
public:
    static KnobGui *BuildKnobGui(Knob *knob, DockablePanel *container) {
        return new Color_KnobGui(knob, container);
    }


    Color_KnobGui(Knob *knob, DockablePanel *container);

    virtual ~Color_KnobGui() OVERRIDE FINAL;

public slots:

    void onColorChanged();

    void showColorDialog();

private:

    virtual void createWidget(QGridLayout *layout, int row) OVERRIDE FINAL;

    virtual void _hide() OVERRIDE FINAL;

    virtual void _show() OVERRIDE FINAL;

    virtual void setEnabled() OVERRIDE FINAL;

    virtual void addToLayout(QHBoxLayout *layout) OVERRIDE FINAL;

    virtual void updateGUI(int dimension, const Variant &variant) OVERRIDE FINAL;

    void updateLabel(const QColor &color);

private:
    QWidget *mainContainer;
    QHBoxLayout *mainLayout;

    QWidget *boxContainers;
    QHBoxLayout *boxLayout;

    QWidget *colorContainer;
    QHBoxLayout *colorLayout;

    QLabel *_descriptionLabel;

    QLabel *_rLabel;
    QLabel *_gLabel;
    QLabel *_bLabel;
    QLabel *_aLabel;

    SpinBox *_rBox;
    SpinBox *_gBox;
    SpinBox *_bBox;
    SpinBox *_aBox;

    QLabel *_colorLabel;
    Button *_colorDialogButton;

    int _dimension;
};

/*****************************/
class String_KnobGui : public KnobGui
{
    Q_OBJECT
public:
    static KnobGui *BuildKnobGui(Knob *knob, DockablePanel *container) {
        return new String_KnobGui(knob, container);
    }


    String_KnobGui(Knob *knob, DockablePanel *container): KnobGui(knob, container) {}

    virtual ~String_KnobGui() OVERRIDE FINAL;

public slots:
    void onStringChanged(const QString &str);

private:

    virtual void createWidget(QGridLayout *layout, int row) OVERRIDE FINAL;

    virtual void _hide() OVERRIDE FINAL;

    virtual void _show() OVERRIDE FINAL;

    virtual void setEnabled() OVERRIDE FINAL;

    virtual void addToLayout(QHBoxLayout *layout) OVERRIDE FINAL;

    virtual void updateGUI(int dimension, const Variant &variant) OVERRIDE FINAL;

private:
    LineEdit *_lineEdit;
    QLabel *_descriptionLabel;

};





class Group_KnobGui : public KnobGui
{
    Q_OBJECT
public:
    static KnobGui *BuildKnobGui(Knob *knob, DockablePanel *container) {
        return new Group_KnobGui(knob, container);
    }


    Group_KnobGui(Knob *knob, DockablePanel *container): KnobGui(knob, container), _checked(false) {}

    virtual ~Group_KnobGui() OVERRIDE FINAL;

    void addKnob(KnobGui *child, int row, int column);

    bool isChecked() const;

public slots:
    void setChecked(bool b);

private:
    virtual void createWidget(QGridLayout *layout, int row) OVERRIDE FINAL;

    virtual void _hide() OVERRIDE FINAL;

    virtual void _show() OVERRIDE FINAL;

    virtual void setEnabled()  OVERRIDE FINAL {}

    virtual void addToLayout(QHBoxLayout *layout);


    virtual void updateGUI(int dimension, const Variant &variant) OVERRIDE FINAL;


private:
    bool _checked;
    QGridLayout *_layout;
    GroupBoxLabel *_button;
    QLabel *_descriptionLabel;
    std::vector< std::pair< KnobGui *, std::pair<int, int> > > _children;
};




/*****************************/

/*****************************/
class RichText_KnobGui : public KnobGui
{
    Q_OBJECT
public:
    static KnobGui *BuildKnobGui(Knob *knob, DockablePanel *container) {
        return new RichText_KnobGui(knob, container);
    }


    RichText_KnobGui(Knob *knob, DockablePanel *container): KnobGui(knob, container) {}

    virtual ~RichText_KnobGui() OVERRIDE FINAL;

public slots:
    void onTextChanged();

private:

    virtual void createWidget(QGridLayout *layout, int row) OVERRIDE FINAL;

    virtual void _hide() OVERRIDE FINAL;

    virtual void _show() OVERRIDE FINAL;

    virtual void setEnabled() OVERRIDE FINAL;

    virtual void addToLayout(QHBoxLayout *layout) OVERRIDE FINAL;

    virtual void updateGUI(int dimension, const Variant &variant) OVERRIDE FINAL;

private:
    QTextEdit *_textEdit;
    QLabel *_descriptionLabel;

};



#endif // NATRON_GUI_KNOBGUITYPES_H_