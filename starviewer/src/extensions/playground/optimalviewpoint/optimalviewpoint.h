/***************************************************************************
 *   Copyright (C) 2006 by Grup de Gràfics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#ifndef UDGOPTIMALVIEWPOINT_H
#define UDGOPTIMALVIEWPOINT_H

#include <QObject>

// #include <QGradient>    // per QGradientStops
#include <vector>       // per std::vector<*>
#include "transferfunction.h"

class vtkCamera;
class vtkRenderer;






class vtkImageActor;

class vtkRenderWindow;


class vtkInteractorStyleSwitch;

class vtkVolume;

//class VolumVTK;


class QColor;
//---------------------
class vtkImageData;

class vtkRenderWindowInteractor;

class vtkPiecewiseFunction;
class vtkColorTransferFunction;




namespace udg {

// class OptimalViewpointHelper;
class OptimalViewpointVolume;
class OptimalViewpointPlane;

/**
 * Aquesta classe encapsula el mètode de visualització del Punt de Vista Òptim
 * (Optimal Viewpoint).
 * No és ben bé un mètode de visualització, sinó un algorisme per calcular el
 * punt de vista que aporta més informació a l'espectador donat un volum
 * determinat.
 */
class OptimalViewpoint : public QObject {

    Q_OBJECT

public:
    
    //typedef QPair<qreal, QColor> QGradientStop;       // a <QGradient>
    //typedef QVector<QGradientStop> QGradientStops;    // a <QGradient>
//     typedef QGradientStops TransferFunction;                                // funció de transferència per un volum d'un mirall

    OptimalViewpoint();
    virtual ~OptimalViewpoint();

    /**
     * Retorna el renderer principal (el que ha de fer servir
     * MagicMirrorsViewer).
     */
    vtkRenderer * getMainRenderer() const;
    void setMainRenderer( vtkRenderer * mainRenderer );

    /// Assigna l'interactor de la finestra de visualització.
    void setInteractor( vtkRenderWindowInteractor * interactor );

    /// Afegeix un volum.
    void setImage( vtkImageData * image );
    void setSegmentationFileName( QString name );

    /// Estableix el nombre de miralls.
    void setNumberOfPlanes( unsigned char numberOfPlanes );

    void setShade( bool shade );

    void setImageSampleDistance( double imageSampleDistance );

    void setSampleDistance( double sampleDistance );

    void setBlockLength( unsigned char blockLength );

    /// Estableix totes les funcions de transferència.
    void setTransferFunction( const TransferFunction & transferFunction );

    /// Actualitza els miralls.
    void updatePlanes();



    bool resultsChanged() const;
    std::vector<double> * getResults();
    std::vector<double> * getEntropyRateResults();
    std::vector<double> * getExcessEntropyResults();
    const TransferFunction & getAdjustedTransferFunction() const;



    // nous paràmetres
    void setOpacityForComputing( bool on );
    void setInterpolation( int interpolation );
    void setSpecular( bool specular );
    void setSpecularPower( double specularPower );
    void setUpdatePlane( short updatePlane );
    void setCompute( bool compute );
    void setSimilarityThreshold( double similarityThreshold );



    /// Carrega la segmentació del volum des dun fitxer de text.
    signed char loadSegmentationFromFile( const QString & segmentationFileName );
    /// Fa una segmentació automàtica del volum.
    unsigned char doAutomaticSegmentation( unsigned short iterations,
                                           unsigned char blockLength,
                                           unsigned char numberOfClusters,
                                           double noise,
                                           double imageSampleDistance,
                                           double sampleDistance );




private slots:

    void newResults();
    void setAdjustedTransferFunction( const TransferFunction & adjustedTransferFunction );

private:

    typedef std::vector<OptimalViewpointPlane *> PlaneVector;

    vtkRenderer * m_renderer;
    vtkCamera * m_camera;
    OptimalViewpointVolume * m_volume;
    PlaneVector * m_planes;     // vector de plans

    vtkRenderWindowInteractor * m_interactor;   // interactor
//     OptimalViewpointHelper * m_helper;            // actualitzador
    unsigned short m_planeSize;                // mida dels miralls
    unsigned char m_numberOfPlanes;            // nombre de miralls
    unsigned char m_numberOfClusters;
    bool m_resultsChanged;

    TransferFunction m_adjustedTransferFunction;


    // nous paràmetres
    short m_updatePlane;
    bool m_compute;
    double m_similarityThreshold;


}; // end class OptimalViewpoint

}; // end namespace udg

#endif // UDGOPTIMALVIEWPOINT_H
