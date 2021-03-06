/*
 *  Copyright (c) 2017 Dmitry Kazakov <dimula73@gmail.com>
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

#ifndef KISSTROKEEFFICIENCYMEASURER_H
#define KISSTROKEEFFICIENCYMEASURER_H

#include "kritaui_export.h"
#include <QScopedPointer>

#include <QtGlobal>

class QPointF;

class KRITAUI_EXPORT KisStrokeEfficiencyMeasurer
{
public:
    KisStrokeEfficiencyMeasurer();
    ~KisStrokeEfficiencyMeasurer();

    void setEnabled(bool value);
    bool isEnabled() const;

    void addSample(const QPointF &pt);
    void addSamples(const QVector<QPointF> &points);

    qreal averageCursorSpeed() const;
    qreal averageRenderingSpeed() const;
    qreal averageFps() const;

    void notifyRenderingStarted();
    void notifyRenderingFinished();

    void notifyCursorMoveStarted();
    void notifyCursorMoveFinished();

    void notifyFrameRenderingStarted();

    void reset();

private:
    struct Private;
    const QScopedPointer<Private> m_d;
};

#endif // KISSTROKEEFFICIENCYMEASURER_H
