/*
 *  Copyright (c) 2020 Eoin O'Neill <eoinoneill1991@gmail.com>
 *  Copyright (c) 2020 Emmet O'Neill <emmetoneill.pdx@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef KISLAYERFILTERWIDGET_H
#define KISLAYERFILTERWIDGET_H

#include <QWidget>
#include "kis_types.h"

#include "kritaui_export.h"


class KRITAUI_EXPORT KisLayerFilterWidget : public QWidget
{
    Q_OBJECT
private:

    class EventFilter : public QObject {
    private:
        QWidget* m_buttonContainer;

        enum State{
            Idle,
            WaitingForDragLeave, //Waiting for mouse to exit first clicked while the mouse button is down.
            WaitingForDragEnter //Waiting for mouse to slide across buttons within the same button group.
        };

        State currentState;
        QPoint lastKnownMousePosition;

    public:
        EventFilter(QWidget *buttonContainer, QObject *parent = nullptr);

    protected:
        bool eventFilter(QObject *obj, QEvent *event);
        void checkSlideOverNeighborButtons(QMouseEvent* mouseEvent, class QAbstractButton* startingButton);
    };

    EventFilter *buttonEventFilter;
    class QLineEdit *textFilter;
    class KisColorLabelButtonGroup *buttonGroup;

public:
    KisLayerFilterWidget(QWidget *parent = nullptr);

    static void scanUsedColorLabels(KisNodeSP node, QSet<int> &colorLabels);
    void updateColorLabels(KisNodeSP root);

    bool isCurrentlyFiltering();
    QList<int> getActiveColors();
    QString getTextFilter();

    void reset();

Q_SIGNALS:
    void filteringOptionsChanged();

};

#endif // KISLAYERFILTERWIDGET_H
