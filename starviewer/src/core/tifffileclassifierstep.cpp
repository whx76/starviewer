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

#include "tifffileclassifierstep.h"

#include "image.h"
#include "logging.h"
#include "patient.h"
#include "patientfillerinput.h"

#include <QFileInfo>

#include <vtkNew.h>
#include <vtkTIFFReader.h>

namespace udg {

TiffFileClassifierStep::TiffFileClassifierStep()
{
}

bool TiffFileClassifierStep::fillIndividually()
{
    Q_ASSERT(m_input);

    QString file = m_input->getFile();

    vtkNew<vtkTIFFReader> tiffReader;

    if (tiffReader->CanReadFile(qPrintable(file)) == 0)
    {
        ERROR_LOG(file + " is not a TIFF image");
        return false;
    }

    // Abans de res comprovar que l'arxiu no estigui ja classificat
    // comprovem primer que l'arxiu no estigui ja dins de l'estructura, el qual vol dir que ja l'han classificat
    bool found = false;
    int i = 0;
    while (i < m_input->getNumberOfPatients() && !found)
    {
        found = m_input->getPatient(i)->hasFile(file);
        i++;
    }
    if (found)
    {
        return true;
    }

    // TODO faltaria comprovar si tenim algun pacient igual o no? i assignar al mateix estudi o algo semblant? En certa manera si sempre assignem la mateixa
    // informació, posteriorment els pacients que anem creant, ja es fusionaran ells mateixos. Els id's que han de ser diferents haurien de ser els de les series

    Patient *patient;
    Study *study;
    if (m_input->getNumberOfPatients() == 0)
    {
        // Creem el pacient
        patient = new Patient;

        patient->setFullName("TIFF File ");
        patient->setID("TIFF Patient");

        m_input->addPatient(patient);

        // Creem l'estudi
        study = new Study;
        study->setInstanceUID("TIFFStudy-#123456#");
        study->setDate(QDate::currentDate());
        study->setTime(QTime::currentTime());
        study->setID("TIFFStudy-#123456#");
        study->setDescription("TIFF Study");
        patient->addStudy(study);
    }
    else
    {
        patient = m_input->getPatient();
        study = patient->getStudies().first();
    }

    // Creem la serie
    Series *series = new Series;
    static int seriesUID = 123456;
    static int seriesNumber = 0;
    series->setInstanceUID(QString("TIFFSeries-#%1#").arg(seriesUID++));

    series->setModality("CT");  // TODO assuming CT
    series->setSeriesNumber(QString::number(seriesNumber += 101));
    series->setDate(QDate::currentDate());
    series->setTime(QTime::currentTime());

    QFileInfo fileInfo(file);
    series->setDescription(fileInfo.fileName());
    study->addSeries(series);

    // Aquí fem el pas de l'image filler step: creem la imatge
    Image *image = new Image;
    image->setPath(file);
    static int mhdSOPIntanceUID = 0;
    image->setSOPInstanceUID(QString("TIFFImage-#%1#").arg(mhdSOPIntanceUID++));
    // TODO obtenir l'spacing i thickness de l'mhd, aquests valors són temporals i per defecte únicament
    image->setPixelSpacing(1.0, 1.0);
    image->setSliceThickness(1.0);
    image->setSamplesPerPixel(1);

    // TODO obtenir valor restants d'imatge de l'mhd si és possible

    series->addImage(image);

    return true;
}

} // namespace udg
