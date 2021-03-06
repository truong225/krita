/*
 *  Copyright (c) 2010 Dmitry Kazakov <dimula73@gmail.com>
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

#include "kis_abstract_compression.h"

KisAbstractCompression::KisAbstractCompression()
{
}

KisAbstractCompression::~KisAbstractCompression()
{
}

void KisAbstractCompression::adjustForDataSize(qint32 dataSize)
{
    Q_UNUSED(dataSize);
}

void KisAbstractCompression::linearizeColors(quint8 *input, quint8 *output,
                                             qint32 dataSize, qint32 pixelSize)
{
    quint8 *outputByte = output;
    quint8 *lastByte = input + dataSize -1;

    for(qint32 i = 0; i < pixelSize; i++) {
        quint8 *inputByte = input + i;
        while (inputByte <= lastByte) {
            *outputByte = *inputByte;
            outputByte++;
            inputByte+=pixelSize;
        }
    }
}

void KisAbstractCompression::delinearizeColors(quint8 *input, quint8 *output,
                                               qint32 dataSize, qint32 pixelSize)
{
    /**
     * In the beginning, i wrote "delinearization" in a way,
     * that looks like a "linearization", but it turned to be quite
     * inefficient. It seems like reading from random positions is
     * much faster than writing to random areas. So this version is
     * 13% faster.
     */

    quint8 *outputByte = output;
    quint8 *lastByte = output + dataSize -1;

    qint32 strideSize = dataSize / pixelSize;
    quint8 *startByte = input;

    while (outputByte <= lastByte) {
        quint8 *inputByte = startByte;

        for(qint32 i = 0; i < pixelSize; i++) {
            *outputByte = *inputByte;
            outputByte++;
            inputByte += strideSize;
        }

        startByte++;
    }
}
