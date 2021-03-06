//  Natron
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
*Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012. 
*contact: immarespond at gmail dot com
*
*/

#include "Gui/ComboBox.h"

#include <cassert>
#include <algorithm>
#include <QLayout>
#include <QStyle>
#include <QFont>
#include <QFontMetrics>
#include <QTextDocument> // for Qt::convertFromPlainText
CLANG_DIAG_OFF(unused-private-field)
#include <QMouseEvent>
CLANG_DIAG_ON(unused-private-field)

#include "Gui/GuiApplicationManager.h"
#include "Gui/MenuWithToolTips.h"
#include "Gui/ClickableLabel.h"

using namespace Natron;

ComboBox::ComboBox(QWidget* parent)
: QFrame(parent)
, _currentIndex(0)
#if IS_MAXIMUMTEXTSIZE_USEFUL
, _maximumTextSize(0)
#endif
, _wasDirtyPriorToMousePress(false)
{
    
    _mainLayout = new QHBoxLayout(this);
    _mainLayout->setSpacing(0);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(_mainLayout);
    setFrameShape(QFrame::Box);
    
    _currentText = new ClickableLabel("",this);
    _currentText->setSunken(true);
    _currentText->setObjectName("ComboBoxLabel");
    setCurrentIndex(-1);
    _mainLayout->addWidget(_currentText);
    _currentText->setFixedHeight(fontMetrics().height() + 8);

    
    _dropDownIcon = new ClickableLabel("",this);
    _dropDownIcon->setSunken(true);
    _dropDownIcon->setObjectName("ComboBoxDropDownLabel");
    QPixmap pixC;
    appPTR->getIcon(NATRON_PIXMAP_COMBOBOX, &pixC);
    _dropDownIcon->setPixmap(pixC);
    _mainLayout->addWidget(_dropDownIcon);
    
    _menu = new MenuWithToolTips(this);
    
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    
}



void ComboBox::paintEvent(QPaintEvent *e)
{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QFrame::paintEvent(e);

}

void ComboBox::mousePressEvent(QMouseEvent* e){
    
    if (!e->buttons().testFlag(Qt::RightButton) && e->buttons().testFlag(Qt::LeftButton) && _currentText->isEnabled()) {
        QPixmap pixC;
        appPTR->getIcon(NATRON_PIXMAP_COMBOBOX_PRESSED, &pixC);
        _dropDownIcon->setPixmap(pixC);
        _wasDirtyPriorToMousePress = _dropDownIcon->getDirty();
        setDirty(true);
        createMenu();
        QFrame::mousePressEvent(e);

    }
}

void ComboBox::mouseReleaseEvent(QMouseEvent* e){
    QPixmap pixC;
    appPTR->getIcon(NATRON_PIXMAP_COMBOBOX, &pixC);
    _dropDownIcon->setPixmap(pixC);
    if (!_wasDirtyPriorToMousePress) {
        setDirty(false);
    }
    QFrame::mouseReleaseEvent(e);
}

void ComboBox::createMenu(){
    _menu->clear();
    for (U32 i = 0 ; i < _actions.size(); ++i) {
        for (U32 j = 0; j < _separators.size(); ++j) {
            if (_separators[j] == (int)i) {
                _menu->addSeparator();
                break;
            }
        }
        _actions[i]->setEnabled(!_currentText->isReadOnly());
        _menu->addAction(_actions[i]);
    }
    QAction* triggered = _menu->exec(this->mapToGlobal(QPoint(0,height())));
    for (U32 i = 0; i < _actions.size(); ++i) {
        if (triggered == _actions[i]) {
            setCurrentIndex(i);
            break;
        }
    }
    
    QPixmap pixC;
    appPTR->getIcon(NATRON_PIXMAP_COMBOBOX, &pixC);
    _dropDownIcon->setPixmap(pixC);
    if (!_wasDirtyPriorToMousePress) {
        setDirty(false);
    }
    
}




int ComboBox::count() const{
    return (int)_actions.size();
}
void ComboBox::insertItem(int index,const QString& item,QIcon icon,QKeySequence key,const QString& toolTip){
    assert(index >= 0);
    QAction* action =  new QAction(this);
    action->setText(item);
    if(!toolTip.isEmpty()){
        action->setToolTip(Qt::convertFromPlainText(toolTip, Qt::WhiteSpaceNormal));
    }
    if (!icon.isNull()) {
        action->setIcon(icon);
    }
    if (!key.isEmpty()) {
        action->setShortcut(key);
    }
#if IS_MAXIMUMTEXTSIZE_USEFUL
    if (item.size() > _maximumTextSize) {
        _maximumTextSize = item.size();
    }
#endif
    growMaximumWidthFromText(item);
    _actions.insert(_actions.begin()+index, action);
    /*if this is the first action we add, make it current*/
    if(_actions.size() == 1){
        setCurrentText_no_emit(itemText(0));
    }
 
}

void ComboBox::addItem(const QString& item,QIcon icon ,QKeySequence key,const QString& toolTip){
    QAction* action =  new QAction(this);
    
    action->setText(item);
    if (!icon.isNull()) {
        action->setIcon(icon);
    }
    if (!key.isEmpty()) {
        action->setShortcut(key);
    }
    if(!toolTip.isEmpty()){
        action->setToolTip(Qt::convertFromPlainText(toolTip, Qt::WhiteSpaceNormal));
    }
#if IS_MAXIMUMTEXTSIZE_USEFUL
    if (item.size() > _maximumTextSize) {
        _maximumTextSize = item.size();
    }
#endif
    growMaximumWidthFromText(item);
    _actions.push_back(action);
    
    /*if this is the first action we add, make it current*/
    if(_actions.size() == 1){
        setCurrentText_no_emit(itemText(0));
    }
}

void ComboBox::setCurrentText_no_emit(const QString& text) {
    setCurrentText_internal(text);
}

void ComboBox::setCurrentText(const QString& text) {
    int index = setCurrentText_internal(text);
    if (index != -1) {
        emit currentIndexChanged(index);
        emit currentIndexChanged(getCurrentIndexText());
    }
}

int ComboBox::setCurrentText_internal(const QString& text) {
    QString str(text);
    growMaximumWidthFromText(str);
    str.prepend("  ");
    str.append("  ");
    assert(_currentText);
    _currentText->setText_overload(str);
    QFontMetrics m = fontMetrics();
    _currentText->setMinimumWidth(m.width(str));
    // if no action matches this text, set the index to a dirty value
    int index = -1;
    for (U32 i = 0; i < _actions.size(); ++i) {
        if(_actions[i]->text() == text){
            index = i;
            break;
        }
    }
    if (_currentIndex != index && index != -1) {
        _currentIndex = index;
        return index;
    }
    return -1;
}

void ComboBox::setMaximumWidthFromText(const QString& str)
{
    int w = _currentText->fontMetrics().width(str+"    ");
    setMaximumWidth(w);
}

void ComboBox::growMaximumWidthFromText(const QString& str)
{
    int w = _currentText->fontMetrics().width(str+"    ");
    if (w > maximumWidth()) {
        setMaximumWidth(w);
    }
}

int ComboBox::activeIndex() const{
    return _currentIndex;
}

QString ComboBox::getCurrentIndexText() const {
    assert(_currentIndex < (int)_actions.size());
    return _actions[_currentIndex]->text();
}

bool ComboBox::setCurrentIndex_internal(int index) {
    QString str;
    QString text;
    if (0 <= index && index < (int)_actions.size()) {
        text = _actions[index]->text();
    }
    str = text;
    /*before displaying,prepend and append the text by some spacing.
     This is a dirty way to do this but QLayout::addSpacing() doesn't preserve
     the same style for the label.*/
#if IS_MAXIMUMTEXTSIZE_USEFUL
    int dsize = _maximumTextSize - str.size();
    dsize/=2;
    str.prepend("  ");
    for (int i = 0; i < dsize ; ++i) {str.prepend(" ");}
    str.append("  ");
    for (int i = 0; i < dsize ; ++i) {str.append(" ");}
#endif
    str.prepend("  ");
    str.append("  ");
    _currentText->setText_overload(str);
    QFontMetrics m = fontMetrics();
    _currentText->setMinimumWidth(m.width(str));

    if (_currentIndex != index && index != -1) {
        _currentIndex = index;
        return true;
    } else {
        return false;
    }

}

void ComboBox::setCurrentIndex(int index)
{
    if (setCurrentIndex_internal(index)) {
        ///emit the signal only if the entry changed
        emit currentIndexChanged(_currentIndex);
        emit currentIndexChanged(getCurrentIndexText());
    }
}

void ComboBox::setCurrentIndex_no_emit(int index) {
    setCurrentIndex_internal(index);
}

void ComboBox::addSeparator(){
    _separators.push_back(_actions.size()-1);
}
void ComboBox::insertSeparator(int index){
    assert(index >= 0);
    _separators.push_back(index);
}

QString ComboBox::itemText(int index) const{
    if(0 <= index && index < (int)_actions.size()) {
        assert(_actions[index]);
        return _actions[index]->text();
    } else {
        return "";
    }
}
int ComboBox::itemIndex(const QString& str) const{
    for (U32 i = 0; i < _actions.size(); ++i) {
        if (_actions[i]->text() == str) {
            return i;
        }
    }
    return -1;
}

void ComboBox::removeItem(const QString& item){
    for (U32 i = 0; i < _actions.size(); ++i) {
        assert(_actions[i]);
        if (_actions[i]->text() == item) {
            QString currentText = getCurrentIndexText();
            _actions.erase(_actions.begin()+i);
            assert(_currentText);
            if (currentText == item) {
                setCurrentIndex(i-1);
            }
            /*adjust separators that were placed after this item*/
            for (U32 j = 0; j < _separators.size(); ++j) {
                if (_separators[j] >= (int)i) {
                    --_separators[j];
                }
            }
        }
    }
#if IS_MAXIMUMTEXTSIZE_USEFUL
    /*we also need to re-calculate the maximum text size*/
    _maximumTextSize = 0;
    for (U32 i = 0; i < _actions.size(); ++i) {
        assert(_actions[i]);
        if (_actions[i]->text().size() > _maximumTextSize) {
            _maximumTextSize = _actions[i]->text().size();
        }
    }
#endif
}

void ComboBox::clear(){
    _actions.clear();
    _menu->clear();
    _separators.clear();
    _currentIndex = 0;
#if IS_MAXIMUMTEXTSIZE_USEFUL
    _maximumTextSize = 0;
#endif
}


void ComboBox::setItemText(int index,const QString& item){
    assert(0 <= index && index < (int)_actions.size());
    assert(_actions[index]);
    _actions[index]->setText(item);
    growMaximumWidthFromText(item);
    if (index == _currentIndex) {
        setCurrentText_internal(item);
    }
#if IS_MAXIMUMTEXTSIZE_USEFUL
    /*we also need to re-calculate the maximum text size*/
    _maximumTextSize = 0;
    for (U32 i = 0; i < _actions.size(); ++i) {
        assert(_actions[i]);
        if (_actions[i]->text().size() > _maximumTextSize) {
            _maximumTextSize = _actions[i]->text().size();
        }
    }
#endif
}

void ComboBox::setItemShortcut(int index,const QKeySequence& sequence){
    assert(0 <= index && index < (int)_actions.size());
    assert(_actions[index]);
    _actions[index]->setShortcut(sequence);
}

void ComboBox::setItemIcon(int index,const QIcon& icon){
    assert(0 <= index && index < (int)_actions.size());
    assert(_actions[index]);
    _actions[index]->setIcon(icon);
}
void ComboBox::disableItem(int index){
    assert(0 <= index && index < (int)_actions.size());
    assert(_actions[index]);
    _actions[index]->setEnabled(false);
}

void ComboBox::enableItem(int index){
    assert(0 <= index && index < (int)_actions.size());
    assert(_actions[index]);
    _actions[index]->setEnabled(true);
}

void ComboBox::setReadOnly(bool readOnly)
{
    
    _currentText->setEnabled(!readOnly);
    //   _dropDownIcon->setEnabled(!readOnly);
    _currentText->setReadOnly(readOnly);
    _dropDownIcon->setReadOnly(readOnly);
}

void ComboBox::setEnabled_natron(bool enabled)
{
    _currentText->setEnabled(enabled);
}

void ComboBox::setAnimation(int i){
    _currentText->setAnimation(i);
    _dropDownIcon->setAnimation(i);
}

void ComboBox::setDirty(bool b)
{
    _currentText->setDirty(b);
    _dropDownIcon->setDirty(b);
}

