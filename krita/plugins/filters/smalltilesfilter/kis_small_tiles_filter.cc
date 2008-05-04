/*
 * This file is part of Krita
 *
 * Copyright (c) 2005 Michael Thaler <michael.thaler@physik.tu-muenchen.de>
 *
 * ported from Gimp, Copyright (C) 1997 Eiichi Takamori <taka@ma1.seikyou.ne.jp>
 * original pixelize.c for GIMP 0.54 by Tracy Scott
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

#include "kis_small_tiles_filter.h"

#include <stdlib.h>
#include <vector>

#include <QPoint>
#include <QSpinBox>
#include <q3valuevector.h>

#include <klocale.h>
#include <kiconloader.h>
#include <kgenericfactory.h>

#include <KoProgressUpdater.h>

#include <kis_painter.h>
#include <kis_doc2.h>
#include <kis_debug.h>
#include <kis_image.h>
#include <kis_iterators_pixel.h>
#include <kis_layer.h>
#include <filter/kis_filter_registry.h>
#include <kis_global.h>
#include <kis_types.h>
#include <kis_paint_device.h>
#include <kis_filter_strategy.h>
#include <kis_painter.h>
#include <kis_selection.h>
#include <filter/kis_filter_configuration.h>
#include <kis_processing_information.h>
#include <KoCompositeOp.h>

#include "widgets/kis_multi_integer_filter_widget.h"

KisSmallTilesFilter::KisSmallTilesFilter() : KisFilter(id(), KisFilter::CategoryMap, i18n("&Small Tiles..."))
{
    setSupportsPainting( true );
    setSupportsPreview( true );
    setSupportsIncrementalPainting( false );
}

void KisSmallTilesFilter::process(KisConstProcessingInformation srcInfo,
                 KisProcessingInformation dstInfo,
                 const QSize& size,
                 const KisFilterConfiguration* config,
                 KoUpdater* progressUpdater
        ) const
{
    const KisPaintDeviceSP src = srcInfo.paintDevice();
    KisPaintDeviceSP dst = dstInfo.paintDevice();
    QPoint dstTopLeft = dstInfo.topLeft();
    QPoint srcTopLeft = srcInfo.topLeft();
    Q_ASSERT(!src.isNull());
    Q_ASSERT(!dst.isNull());
    
    //read the filter configuration values from the KisFilterConfiguration object
    quint32 numberOfTiles = config->getInt( "numberOfTiles", 2);

    QRect srcRect = src->exactBounds();

    int w = static_cast<int>(srcRect.width() / numberOfTiles);
    int h = static_cast<int>(srcRect.height() / numberOfTiles);

    KisPaintDeviceSP tile = KisPaintDeviceSP(0);
    if (srcInfo.selection()) {
        KisPaintDeviceSP tmp = new KisPaintDevice(src->colorSpace(), "selected bit");
        KisPainter gc(tmp);
        gc.bltSelection(0, 0, COMPOSITE_COPY, src, OPACITY_OPAQUE, srcTopLeft.x(), srcTopLeft.y(), size.width(), size.height());
        tile = tmp->createThumbnailDevice(srcRect.width() / numberOfTiles, srcRect.height() / numberOfTiles);
    }
    else {
        tile = src->createThumbnailDevice(srcRect.width() / numberOfTiles, srcRect.height() / numberOfTiles);
    }
    if (tile.isNull()) return;

    // XXX: does anyone knows why we need a temporary device ?
    KisPaintDeviceSP scratch = new KisPaintDevice(src->colorSpace());

    KisPainter gc(scratch);

    if( progressUpdater )
    {
        progressUpdater->setRange(0, numberOfTiles );
    }

    for (uint y = 0; y < numberOfTiles; ++y) {
        for (uint x = 0; x < numberOfTiles; ++x) {
            // XXX make composite op and opacity configurable
            gc.bitBlt( w * x, h * y, COMPOSITE_COPY, tile, 0, 0, w, h);
            if(progressUpdater) progressUpdater->setValue( y );
        }
    }
    gc.end();

    gc.begin(dst);
    if (srcInfo.selection()) {
        gc.bltSelection(dstTopLeft.x(), dstTopLeft.y(), COMPOSITE_OVER, scratch, srcInfo.selection(), OPACITY_OPAQUE, 0, 0, size.width(), size.height() );
    }
    else {
        gc.bitBlt(dstTopLeft.x(), dstTopLeft.y(), COMPOSITE_OVER, scratch, OPACITY_OPAQUE, 0, 0, size.width(), size.height() );
    }
    gc.end();

}

KisFilterConfigWidget * KisSmallTilesFilter::createConfigurationWidget(QWidget* parent, const KisPaintDeviceSP, const KisImageSP) const
{
    vKisIntegerWidgetParam param;
    param.push_back( KisIntegerWidgetParam( 2, 5, 1, i18n("Number of tiles"), "smalltiles" ) );
    return new KisMultiIntegerFilterWidget( id().id(),  parent,  id().id(),  param );

}

KisFilterConfiguration* KisSmallTilesFilter::factoryConfiguration(const KisPaintDeviceSP) const
{
    KisFilterConfiguration* config = new KisFilterConfiguration("noise", 1);
    config->setProperty("smalltiles", 2 );
    return config;
}
