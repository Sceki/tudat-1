/*    Copyright (c) 2010-2018, Delft University of Technology
 *    All rigths reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#ifndef TUDAT_CREATERADIATIONPRESSUREINTERFACE_H
#define TUDAT_CREATERADIATIONPRESSUREINTERFACE_H

#include <memory>
#include <functional>
#include <boost/make_shared.hpp>

#include "Tudat/SimulationSetup/EnvironmentSetup/body.h"
#include "Tudat/Astrodynamics/ElectroMagnetism/radiationPressureInterface.h"


namespace tudat
{

namespace simulation_setup
{

//! Default values for radiation pressure.
static const std::map< std::string, double > defaultRadiatedPowerValues =
{ { "Sun",  3.839E26 } };

//! List of radiation pressure model types.
enum RadiationPressureType
{
    cannon_ball,
    solar_sail
};

//! Base class for radiation pressure interface settings.
/*!
 *  Base class for providing settings for automatic radiation pressure properties creation.  This is
 *  a non-functional base class, specific implementations must be defined in derived classes.
 */
class RadiationPressureInterfaceSettings
{
public:

    //! Constructor
    /*!
     * Constructor.
     * \param radiationPressureType Type of radiation pressure interface that is to be made.
     * \param sourceBody Name of body emitting the radiation.
     * \param occultingBodies List of bodies causing (partial) occultation.
     * \param centralBodies List of central bodies.
     */
    RadiationPressureInterfaceSettings(
        const RadiationPressureType radiationPressureType,
        const std::string& sourceBody,
        const std::vector< std::string > occultingBodies = std::vector< std::string >( ),
        const std::vector< std::string > centralBodies = std::vector< std::string >( ) ):
          radiationPressureType_( radiationPressureType ), sourceBody_( sourceBody ),
          occultingBodies_( occultingBodies ), centralBodies_( centralBodies ){  }

    //! Destructor
    virtual ~RadiationPressureInterfaceSettings( ){ }

    //! Function returning type of radiation pressure interface that is to be made.
    /*!
     *  Function returning type of radiation pressure interface that is to be made.
     *  \return Type of radiation pressure interface that is to be made.
     */
    RadiationPressureType getRadiationPressureType( ){ return radiationPressureType_; }

    //! Function returning name of body emitting the radiation.
    /*!
     *  Function returning name of body emitting the radiation.
     *  \return Name of body emitting the radiation.
     */
    std::string getSourceBody( ){ return sourceBody_; }

    //! Function returning list of bodies causing (partial) occultation.
    /*!
     *  Function returning list of bodies causing (partial) occultation.
     *  \return List of bodies causing (partial) occultation.
     */
    std::vector< std::string > getOccultingBodies( ){ return occultingBodies_; }

    //! Function returning list of central bodies.
    /*!
     *  Function returning list of central bodies.
     *  \return List of central bodies.
     */
    std::vector< std::string > getCentralBodies( ){ return centralBodies_; }

protected:

    //! Type of radiation pressure interface that is to be made.
    RadiationPressureType radiationPressureType_;

    //! Name of body emitting the radiation.
    std::string sourceBody_;

    //! List of bodies causing (partial) occultation
    std::vector< std::string > occultingBodies_;

    //! List of bodies causing (partial) occultation
    std::vector< std::string > centralBodies_;
};

//! Class providing settings for the creation of a cannonball radiation pressure interface
class CannonBallRadiationPressureInterfaceSettings: public RadiationPressureInterfaceSettings
{
public:

    /*! Constructor
     * Constructor
     * \param sourceBody Name of body emitting the radiation.
     * \param area Surface area that undergoes radiation pressure.
     * \param radiationPressureCoefficient Radiation pressure coefficient.
     * \param occultingBodies List of bodies causing (partial) occultation.
     */
    CannonBallRadiationPressureInterfaceSettings(
            const std::string& sourceBody, const double area, const double radiationPressureCoefficient,
            const std::vector< std::string >& occultingBodies = std::vector< std::string >( ) ):
        RadiationPressureInterfaceSettings( cannon_ball, sourceBody, occultingBodies ),
        area_( area ), radiationPressureCoefficient_( radiationPressureCoefficient ){ }

    //! Function to return surface area that undergoes radiation pressure.
    /*!
     *  Function to return surface area that undergoes radiation pressure.
     *  \return Surface area that undergoes radiation pressure.
     */
    double getArea( ){ return area_; }

    //! Function to set surface area that undergoes radiation pressure.
    /*!
     *  Function to set surface area that undergoes radiation pressure.
     *  \param area Surface area that undergoes radiation pressure.
     */
    void setArea( double area ){ area_ = area; }

    //! Function to return radiation pressure coefficient.
    /*!
     *  Function to return radiation pressure coefficient.
     *  \return Radiation pressure coefficient.
     */
    double getRadiationPressureCoefficient( ){ return radiationPressureCoefficient_; }


private:

    //! Surface area that undergoes radiation pressure.
    double area_;

    //! Radiation pressure coefficient.
    double radiationPressureCoefficient_;
};

//! Class providing settings for the creation of a cannonball radiation pressure interface
class SolarSailRadiationInterfaceSettings: public RadiationPressureInterfaceSettings
{
public:

    /*! Constructor
     * Constructor
     * \param sourceBody Name of body emitting the radiation.
     * \param area Surface area that undergoes radiation pressure.
     * \param radiationPressureCoefficient Radiation pressure coefficient.
     * \param occultingBodies List of bodies causing (partial) occultation.
     */

    SolarSailRadiationInterfaceSettings(
        const std::string& sourceBody,
        const std::function< void(const double) > updateFunction,
        const double area,
        const std::function< double(  ) > coneAngle,
        const std::function< double(  ) > clockAngle,
        const double frontEmissivityCoefficient, const double backEmissivityCoefficient, const double frontLambertianCoefficient,
        const double backLambertianCoefficient, const double reflectivityCoefficient,
        const double specularReflectionCoefficient,
        const std::vector< std::string >& occultingBodies = std::vector< std::string >( ),
        const std::vector< std::string >& centralBodies = std::vector< std::string >( )):
          RadiationPressureInterfaceSettings( solar_sail, sourceBody, occultingBodies, centralBodies ),
          updateFunction_ (updateFunction), area_( area ), coneAngleFunction_( coneAngle ),
          clockAngleFunction_( clockAngle ), frontEmissivityCoefficient_( frontEmissivityCoefficient ),
          backEmissivityCoefficient_( backEmissivityCoefficient ),
          frontLambertianCoefficient_( frontLambertianCoefficient ),
          backLambertianCoefficient_( backLambertianCoefficient ),
          reflectivityCoefficient_( reflectivityCoefficient ),
          specularReflectionCoefficient_( specularReflectionCoefficient ){ }

    //! Function to return surface area that undergoes radiation pressure.
    /*!
     *  Function to return surface area that undergoes radiation pressure.
     *  \return Surface area that undergoes radiation pressure.
     */
    double getArea( ){ return area_; }

    //! Function to set surface area that undergoes radiation pressure.
    /*!
     *  Function to set surface area that undergoes radiation pressure.
     *  \param area Surface area that undergoes radiation pressure.
     */
    void setArea( double area ){ area_ = area; }

    //! Function to return cone angle
    /*!
     *  Function to return cone angle
     *  \return Cone angle
     */
    std::function< double(  ) > getConeAngle(  ) {return coneAngleFunction_;}



    //! Function to set cone angle.
    /*!
     *  Function to set cone angle.
     *  \param cone angle.
     */
    void setConeAngle( std::function< double( ) > coneAngle ){coneAngleFunction_=coneAngle;}


    //! Function to return clock angle
    /*!
     *  Function to return clock angle
     *  \return Cone angle
     */
    std::function< double(  ) > getClockAngle(  ) { return clockAngleFunction_; }

    //! Function to set clock angle.
    /*!
     *  Function to set clock angle.
     *  \param clock angle.
     */
    void setClockAngle( std::function< double(  ) > clockAngle ){ clockAngleFunction_=clockAngle; }


    //! Function to return front emissivity coefficient
    /*!
     *  Function to return front emissivity coefficient
     *  \return Front emissivity coefficient
     */
    double getFrontEmissivityCoefficient() { return frontEmissivityCoefficient_; }

    //! Function to set front emissivity coefficient.
    /*!
     *  Function to set front emissivity coefficient.
     *  \param Front emissivity coefficient.
     */
    void setFrontEmissivityCoefficient( double frontEmissivityCoefficient ){ frontEmissivityCoefficient_ = frontEmissivityCoefficient; }


    //! Function to return back emissivity coefficient
    /*!
     *  Function to return back emissivity coefficient
     *  \return Back emissivity coefficient
     */
    double getBackEmissivityCoefficient() { return backEmissivityCoefficient_; }

    //! Function to set back emissivity coefficient.
    /*!
     *  Function to set back emissivity coefficient.
     *  \param Back emissivity coefficient.
     */
    void setBackEmissivityCoefficient( double backEmissivityCoefficient ){ backEmissivityCoefficient_ = backEmissivityCoefficient; }


    //! Function to return front Lambertian coefficient
    /*!
     *  Function to return front Lambertian coefficient
     *  \return Front Lambertian coefficient
     */
    double getFrontLambertianCoefficient() { return frontLambertianCoefficient_; }

    //! Function to set front Lambertian coefficient.
    /*!
     *  Function to set front Lambertian coefficient.
     *  \param Front Lambertian coefficient.
     */
    void setFrontLambertianCoefficient( double frontLambertianCoefficient ){ frontLambertianCoefficient_ = frontLambertianCoefficient; }


    //! Function to return back Lambertian coefficient
    /*!
     *  Function to return back Lambertian coefficient
     *  \return Back Lambertian coefficient
     */
    double getBackLambertianCoefficient() { return backLambertianCoefficient_; }

    //! Function to set back Lambertian coefficient.
    /*!
     *  Function to set back Lambertian coefficient.
     *  \param Back Lambertian coefficient.
     */
    void setBackLambertianCoefficient( double backLambertianCoefficient ){ backLambertianCoefficient_ = backLambertianCoefficient; }


    //! Function to return reflectivity coefficient
    /*!
     *  Function to return reflectivity coefficient
     *  \return Reflectivity coefficient
     */
    double getReflectivityCoefficient() { return reflectivityCoefficient_; }

    //! Function to set reflectivity coefficient.
    /*!
     *  Function to set reflectivity coefficient.
     *  \param Reflectivity coefficient.
     */
    void setReflectivityCoefficient( double reflectivityCoefficient ){ reflectivityCoefficient_ = reflectivityCoefficient; }


    //! Function to return specular reflection coefficient
    /*!
     *  Function to return specular reflection coefficient
     *  \return Specular reflection coefficient
     */
    double getSpecularReflectionCoefficient() { return specularReflectionCoefficient_; }

    //! Function to set specular reflection coefficient.
    /*!
     *  Function to set specular reflection coefficient.
     *  \param Specular reflection coefficient.
     */
    void setSpecularReflectionCoefficient( double specularReflectionCoefficient ){ specularReflectionCoefficient_ = specularReflectionCoefficient; }


    //! Update function
    std::function< void( const double ) > updateFunction_;

private:

    //! Surface area that undergoes radiation pressure.
    double area_;

    //! Cone angle of the sail
    std::function< double(  ) > coneAngleFunction_;

    //! Clock angle of the sail
    std::function< double(  ) > clockAngleFunction_;

    //! Front emissivity coefficient of the sail
    double frontEmissivityCoefficient_;

    //! Back emissivity coefficient of the sail
    double backEmissivityCoefficient_;

    //! Front Lambertian coefficient of the sail
    double frontLambertianCoefficient_;

    //! Back lambertian coefficient of the sail
    double backLambertianCoefficient_;

    //! Reflectivity coefficient of the sail
    double reflectivityCoefficient_;

    //! Specular reflection coefficient of the sail
    double specularReflectionCoefficient_;


};

//! Function to obtain (by reference) the position functions and radii of occulting bodies
/*!
 * Function to obtain (by reference) the position functions and radii of occulting bodies.
 * \param bodyMap List of body objects.
 * \param occultingBodies List of bodies causing occultation.
 * \param occultingBodyPositions List of position functions of occulting bodies (return by reference
 * output variable).
 * \param occultingBodyRadii List of radii of occulting bodies (return by reference
 * output variable).
 */
void getOccultingBodiesInformation(
    const NamedBodyMap& bodyMap, const std::vector< std::string >& occultingBodies,
    std::vector< std::function< Eigen::Vector3d( ) > >& occultingBodyPositions,
    std::vector< double >& occultingBodyRadii );


//! Function to obtain (by reference) the position functions and velocity of central bodies
/*!
 * Function to obtain (by reference) the position functions and velocity of central bodies.
 * \param bodyMap List of body objects.
 * \param centralBodies List of central bodies.
 * \param centralBodiesPosition List of central bodies' position functions (return by reference
 * output variable).
 * \param centralBodiesVelocity List of velocity of central bodies (return by reference
 * output variable).
 */
void getCentralBodiesInformation(
    const NamedBodyMap& bodyMap, const std::vector< std::string >& centralBodies,
    std::vector< std::function< Eigen::Vector3d( ) > >& centralBodiesPosition,
    std::vector< std::function< Eigen::Vector3d( ) > >& centralBodiesVelocity);

//! Function to create a radiation pressure interface.
/*!
 *  Function to create a radiation pressure interface.
 *  \param radiationPressureInterfaceSettings Settings for the radiation pressure interface.
 *  \param bodyName Name of body for which radiation pressure interface.
 *  \param bodyMap List of body objects to use for creation of radiation pressure interface.
 *  \return Radiation pressure interface pointer of requested type and settings.
 */
std::shared_ptr< electro_magnetism::RadiationPressureInterface > createRadiationPressureInterface(
        const std::shared_ptr< RadiationPressureInterfaceSettings > radiationPressureInterfaceSettings,
        const std::string& bodyName, const NamedBodyMap& bodyMap );



} // namespace simulation_setup

} // namespace tudat

#endif // TUDAT_CREATERADIATIONPRESSUREINTERFACE_H
