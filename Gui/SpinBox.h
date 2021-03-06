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

#ifndef NATRON_GUI_FEEDBACKSPINBOX_H_
#define NATRON_GUI_FEEDBACKSPINBOX_H_

#include "Gui/LineEdit.h"
#include "Engine/Variant.h"

class QDoubleValidator;
class QIntValidator;
class QMenu;

class SpinBox : public LineEdit
{
    Q_OBJECT

    Q_PROPERTY(int animation READ getAnimation WRITE setAnimation)

    Q_PROPERTY(bool dirty READ getDirty WRITE setDirty)
public:
    
    enum SPINBOX_TYPE{INT_SPINBOX = 0,DOUBLE_SPINBOX};
    
    explicit SpinBox(QWidget* parent=0,SPINBOX_TYPE type = INT_SPINBOX);
    
    virtual ~SpinBox() OVERRIDE;
    
    void decimals(int d);
    
    void setMaximum(double t);
    
    void setMinimum(double b);
    
    double value(){return text().toDouble();}
    
    void setIncrement(double d){_increment=d;}
   
    void setAnimation(int i);

    int getAnimation() const { return animation; }
    
    QMenu* getRightClickMenu();
    
    void setDirty(bool d) ;

    bool getDirty() const { return dirty; }
private:
    
    virtual void wheelEvent(QWheelEvent* e) OVERRIDE FINAL;
    
    virtual void keyPressEvent(QKeyEvent* e) OVERRIDE FINAL;
    
    virtual void mousePressEvent(QMouseEvent* e) OVERRIDE FINAL;
    
    virtual void focusInEvent(QFocusEvent* event) OVERRIDE FINAL;

    virtual void focusOutEvent(QFocusEvent * event) OVERRIDE FINAL;
    
    bool validateText();
        
signals:
    
    void valueChanged(double d);

public slots:
    
    void setValue(double d);
    
    void setValue(int d){setValue((double)d);}
    
    /*Used internally when the user pressed enter*/
    void interpretReturn();
 

private:
    
    void setValue_internal(double d,bool ignoreDecimals);

    
    QString setNum(double cur);

    SPINBOX_TYPE _type;
    int _decimals; // for the double spinbox only
    double _increment;
    Variant _mini,_maxi;
    QDoubleValidator* _doubleValidator;
    QIntValidator* _intValidator;
    int animation; // 0 = no animation, 1 = interpolated, 2 = equals keyframe value
    double _valueWhenEnteringFocus;
    int _currentDelta; // accumulates the deltas from wheelevents
    bool _hasChangedSinceLastValidation;
    double _valueAfterLastValidation;
    bool dirty;
};

#endif /* defined(NATRON_GUI_FEEDBACKSPINBOX_H_) */
