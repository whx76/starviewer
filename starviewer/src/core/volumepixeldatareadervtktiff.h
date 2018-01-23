/*************************************************************************************
  Copyright (C) 2014 Laboratori de Gràfics i Imatge, Universitat de Girona &
  Institut de Diagnòstic per la Imatge.
  Girona 2014. All rights reserved.
  http://starviewer.udg.edu

  This file is part of the Starviewer (Medical Imaging Software) open source project.
  It is subject to the license terms in the LICENSE file found in the top-level
  directory of this distribution and at http://starviewer.udg.edu/license. No part of
  the Starviewer (Medical Imaging Software) open source project, including this file,
  may be copied, modified, propagated, or distributed except according to the
  terms contained in the LICENSE file.
 *************************************************************************************/

#ifndef UDG_VOLUMEPIXELDATAREADERVTKTIFF_H
#define UDG_VOLUMEPIXELDATAREADERVTKTIFF_H

#include "volumepixeldatareader.h"

namespace udg {

class VolumePixelDataReaderVtkTiff : public VolumePixelDataReader {

public:

    explicit VolumePixelDataReaderVtkTiff(QObject *parent = nullptr);

    int read(const QStringList &filenames) override;
    void requestAbort() override;

};

} // namespace udg

#endif // UDG_VOLUMEPIXELDATAREADERVTKTIFF_H
