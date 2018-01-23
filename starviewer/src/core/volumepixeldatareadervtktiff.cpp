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

#include "volumepixeldatareadervtktiff.h"

#include "logging.h"
#include "volumepixeldata.h"

#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>
#include <vtkTIFFReader.h>

namespace udg {

VolumePixelDataReaderVtkTiff::VolumePixelDataReaderVtkTiff(QObject *parent)
    : VolumePixelDataReader(parent)
{
}

int VolumePixelDataReaderVtkTiff::read(const QStringList &filenames)
{
    vtkNew<vtkTIFFReader> tiffReader;
    int errorCode = NoError;

    if (filenames.isEmpty())
    {
        WARN_LOG("Empty file list");
        return InvalidFileName;
    }

    // Convert QStringList to vtkStringArray
    if (filenames.size() > 1)
    {
        auto stringArray = vtkSmartPointer<vtkStringArray>::New();

        for (int i = 0; i < filenames.size(); i++)
        {
            stringArray->InsertNextValue(filenames.at(i).toStdString());
        }

        DEBUG_LOG("Reading multiple files with VTK-TIFF");
        tiffReader->SetFileNames(stringArray);
    }
    else
    {
        DEBUG_LOG("Reading a single file with VTK-TIFF");
        tiffReader->SetFileName(qPrintable(filenames.first()));
    }

    try
    {
        tiffReader->Update();
    }
    catch (const std::bad_alloc &)
    {
        errorCode = OutOfMemory;
    }
    catch (const std::exception &exception)
    {
        WARN_LOG(QString("An exception was thrown while reading with vtkTIFFReader: %1").arg(exception.what()));
        errorCode = UnknownError;
    }
    catch (...)
    {
        WARN_LOG("An exception was thrown while reading with vtkTIFFReader");
        errorCode = UnknownError;
    }

    emit progress(100);

    m_volumePixelData = new VolumePixelData();
    m_volumePixelData->setData(tiffReader->GetOutput());

    return errorCode;
}

void VolumePixelDataReaderVtkTiff::requestAbort()
{
}

} // namespace udg
