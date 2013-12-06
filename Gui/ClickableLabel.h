//  Natron
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef NATRON_GUI_CLICKABLELABEL_H_
#define NATRON_GUI_CLICKABLELABEL_H_

#include <QtCore/QObject>
#include <QtGui/QLabel>

#include "Global/Macros.h"

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:

    ClickableLabel(const QString &text, QWidget *parent): QLabel(text, parent), _toggled(false) {}

    virtual ~ClickableLabel() OVERRIDE FINAL {}

    void setClicked(bool b) {
        _toggled = b;
    }

signals:
    void clicked(bool);

private:
    void mousePressEvent(QMouseEvent *) {
        _toggled = !_toggled;
        emit clicked(_toggled);
    }

private:
    bool _toggled;
};

#endif // NATRON_GUI_CLICKABLELABEL_H_