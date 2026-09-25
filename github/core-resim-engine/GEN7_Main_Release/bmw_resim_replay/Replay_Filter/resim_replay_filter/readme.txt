/**
 *
 * Demo Empty Filter
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved.
 *
 * @author               $Author: voigtlpi $
 * @date                 $Date: 2009-07-16 15:36:37 +0200 (Do, 16 Jul 2009) $
 * @version              $Revision: 10093 $
 *
 * @remarks
 *
 */
 
/**
 * \page page_demo_empty Demo Empty Filter
 *
 * Implements an empty filter rump
 * 
 * \par Location
 * \code
 *    ./src/examples/src/filters/demo_empty/
 * \endcode
 *
 * \par Build Environment
 * To see how to set up the build environment have a look at this page @ref page_cmake_overview
 *
 * \par This example shows:
 * \li how to implement a common adtf filter for processing data
 * \li how to create properties for the filter
 * \li how to work with Media Descriptions
 * \li how to deal with a MediaSample that is received, to get the specific data
 * \li how to create a MediaSample
 * \li how to send data over a pin
 *
 * \par Call Sequence
 * For better understanding the sequence of calls while using base device classes of ADTF SDK the 
 * device implementations are added to the installation as cpp-file.\n
 * So you can debug through sequence.
 * 
 * \par The Header for the Demo Empty Filter
 * \include demoemptyfilter.h
 *
 * \par The Implementation for the Demo Empty Filter
 * \include demoemptyfilter.cpp
 *
 */