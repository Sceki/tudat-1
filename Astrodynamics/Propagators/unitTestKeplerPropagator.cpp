/*! \file unitTestKeplerPropagator.cpp
 *    Source file that defines a unit test that tests the Kepler propagator
 *    included in Tudat.
 *
 *    Path              : /Astrodynamics/Propagators/
 *    Version           : 3
 *    Check status      : Checked
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Checker           : E. Iorfida
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : elisabetta_iorfida@yahoo.it
 *
 *    Date created      : 16 February, 2011
 *    Last modified     : 21 Feburary, 2011
 *
 *    References
 *      Melman, J. Propagate software, J.C.P.Melman@tudelft.nl, 2010.
 *
 *    Notes
 *      Test runs code and verifies result against expected value.
 *      If the tested code is erroneous, the test function returns a boolean
 *      true; if the code is correct, the function returns a boolean false.
 *
 *      Currently, this file makes use of benchmark data provided by J. Melman.
 *      In future, it is desirable that the benchmark data is the direct output
 *      of a commercial package such as STK, where are initial conditions of
 *      the simulation are known.
 *
 *      The basic_functions::ROOT_PATH variable needs to be set for this unit
 *      test to work. It must be set to the absolute path to the root directory
 *      for Tudat.
 *
 *    Copyright (c) 2010 Delft University of Technology.
 *
 *    This software is protected by national and international copyright.
 *    Any unauthorized use, reproduction or modification is unlawful and
 *    will be prosecuted. Commercial and non-private application of the
 *    software in any form is strictly prohibited unless otherwise granted
 *    by the authors.
 *
 *    The code is provided without any warranty; without even the implied
 *    warranty of merchantibility or fitness for a particular purpose.
 *
 *    Changelog
 *      YYMMDD    Author            Comment
 *      110216    K. Kumar          First creation of code.
 *      110217    E. Iorfida        Minor changes made.
 *      110221    K. Kumar          Updated variable-naming to comply with
 *                                  protocol.
 */

// Include statements.
#include "unitTestKeplerPropagator.h"

// Using declarations.
using std::cerr;
using std::endl;
using std::string;

//! Namespace for all unit tests.
namespace unit_tests
{

//! Test of implementation of Kepler propagator class.
bool testKeplerPropagator( )
{
    // Test to see if the orbit of a satellite around the Earth is correctly
    // reproduced with respect to benchmark reference data.

    // Test result initialised to false.
    bool isKeplerPropagatorErroneous = false;

    // Load benchmark data.
    // This data originates from J. Melman and is generated by the software
    // package Propagate. The benchmark data was verified against output from
    // Satellite Toolkit (STK).

    // Load file with benchmark data.
    string relativePathToBenchmarkData
            = "Astrodynamics/Propagators/twoBodyKeplerData.dat";

    string absolutePathToBenchmarkData = basic_functions::ROOT_PATH
                                         + relativePathToBenchmarkData;

    std::ifstream twoBodyKeplerBenchmarkFile(
            absolutePathToBenchmarkData.c_str( ) );

    // Check if file could be opened.
    if ( !twoBodyKeplerBenchmarkFile )
    {
        cerr << "Error: Two-body Kepler benchmark data file could not be "
             << "opened." << endl;
        cerr << absolutePathToBenchmarkData << endl;
    }

    // Create propagation history map for benchmark data to be stored in.
    std::map < double, CartesianElements > benchmarkKeplerPropagationHistory;

    // Declare elapsed time.
    double elapsedTime = 0.0;

    // Initialize counter.
    unsigned int twoBodyKeplerDataCounter = 0;

    // Populate propagation history map with benchmark data from file.
    while ( !twoBodyKeplerBenchmarkFile.eof( ) )
    {
        // Store elapsed time which is given in first coloumn.
        twoBodyKeplerBenchmarkFile >> elapsedTime;

        // Store state date from file.
        for ( unsigned int i = 0; i < 6; i++ )
        {
            twoBodyKeplerBenchmarkFile >>
                    benchmarkKeplerPropagationHistory[
                            twoBodyKeplerDataCounter * 3600.0 ].state( i );
        }

        // Increment counter.
        twoBodyKeplerDataCounter++;
    }

    // Close benchmark data file.
    twoBodyKeplerBenchmarkFile.close( );

    // Run Kepler propagator simulation.

    // Create pointer to the state of satellite Asterix given in Cartesian
    // elements.
    CartesianElements stateOfAsterix;

    // Fill initial state vector with position and
    // velocity given for Asterix.
    // Position is given in kilometers and
    // velocity is given in kilometers per second.
    stateOfAsterix.setCartesianElementX( 6.75e3 );
    stateOfAsterix.setCartesianElementY( 0.0 );
    stateOfAsterix.setCartesianElementZ( 0.0 );
    stateOfAsterix.setCartesianElementXDot( 0.0 );
    stateOfAsterix.setCartesianElementYDot( 8.0595973215 );
    stateOfAsterix.setCartesianElementZDot( 0.0 );

    // Convert initial state vector to meters from
    // kilometers.
    stateOfAsterix.state =
            unit_conversions::convertKilometersToMeters(
                    stateOfAsterix.state );

    // Create map of propagation history of Asterix using a Kepler propagator
    // and a map iterator.
    std::map < double, State > asterixKeplerPropagationHistory;
    std::map < double, State >::iterator
            iteratorAsterixKeplerPropagationHistory;

    // Create a pointer to new vehicle for Asterix.
    Vehicle asterix;

    // Create pre-defined Earth object.
    Planet predefinedEarth;
    predefinedEarth.setPredefinedPlanetSettings( Planet::earth );

    // Create Newton-Raphson object.
    NewtonRaphson newtonRaphson;

    // Create Kepler propagator object.
    KeplerPropagator keplerPropagator;

    // Set Newton-Raphson method.
    keplerPropagator.setNewtonRaphson( &newtonRaphson );

    // Add Asterix as the body that has to be propagated.
    keplerPropagator.addBody( &asterix );

    // Set the central body for Asterix
    keplerPropagator.setCentralBody( &asterix, &predefinedEarth );

    // Create series propagator.
    SeriesPropagator seriesPropagator;

    // Set the series propagation start time.
    seriesPropagator.setSeriesPropagationStart( 0.0 );

    // Set the propagation end time.
    seriesPropagator.setSeriesPropagationEnd( 86400.0 );

    // Set fixed output interval for series propagation.
    seriesPropagator.setFixedOutputInterval( 3600.0 );

    // Set Kepler propagator for series propagation.
    seriesPropagator.setPropagator( &keplerPropagator );

    // Set initial state of Asterix.
    seriesPropagator.setInitialState( &asterix, &stateOfAsterix );

    // Run simulation.
    seriesPropagator.execute( );

    // Get series propagation history of Asterix.
    asterixKeplerPropagationHistory
            = seriesPropagator
              .getPropagationHistoryAtFixedOutputIntervals( &asterix );

    // Compute propagation history state data from meters to kilometers.
    for ( iteratorAsterixKeplerPropagationHistory
          = asterixKeplerPropagationHistory.begin( );
          iteratorAsterixKeplerPropagationHistory
          != asterixKeplerPropagationHistory.end( );
          iteratorAsterixKeplerPropagationHistory++ )
    {
        iteratorAsterixKeplerPropagationHistory->second.state
                = unit_conversions::convertMetersToKilometers(
                        iteratorAsterixKeplerPropagationHistory
                        ->second.state );
    }

    // Declare tolerance between benchmark data and simulation data.
    double toleranceBetweenBenchmarkAndSimulationData = 1e-6;

    // Declare difference between benchmark data and simulation data.
    double differenceKeplerData;

    // Check if results match benchmark data.
    for ( unsigned int i = 0;
          i < seriesPropagator.getSeriesPropagationStart( )
          / seriesPropagator.getFixedOutputInterval( ); i++ )
    {
        // Initialize difference in data.
        differenceKeplerData = 0.0;

        for ( unsigned int i = 0; i < 6; i++ )
        {
            differenceKeplerData
                    += mathematics::computeAbsoluteValue(
                            asterixKeplerPropagationHistory[
                                    i * seriesPropagator
                                    .getFixedOutputInterval( ) ].state( i )
                            - benchmarkKeplerPropagationHistory[
                                    i * seriesPropagator
                                    .getFixedOutputInterval( ) ].state( i ) );
        }

        if ( differenceKeplerData
             > toleranceBetweenBenchmarkAndSimulationData )
        {
            isKeplerPropagatorErroneous = true;

            cerr << "The Kepler propagator does not produce "
                 << "consistent results, as running a simulation with "
                 << "does not yield the same results as the benchmark data "
                 << "given the same initial condition." << endl;
        }
    }

    // Return test result.
    // If test is successful return false; if test fails, return true.
    return isKeplerPropagatorErroneous;
}

}

// End of file.
