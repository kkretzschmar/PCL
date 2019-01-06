//     ____   ______ __
//    / __ \ / ____// /
//   / /_/ // /    / /
//  / ____// /___ / /___   PixInsight Class Library
// /_/     \____//_____/   PCL 02.01.11.0937
// ----------------------------------------------------------------------------
// pcl/DrizzleData.h - Released 2018-12-12T09:24:21Z
// ----------------------------------------------------------------------------
// This file is part of the PixInsight Class Library (PCL).
// PCL is a multiplatform C++ framework for development of PixInsight modules.
//
// Copyright (c) 2003-2018 Pleiades Astrophoto S.L. All Rights Reserved.
//
// Redistribution and use in both source and binary forms, with or without
// modification, is permitted provided that the following conditions are met:
//
// 1. All redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. All redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// 3. Neither the names "PixInsight" and "Pleiades Astrophoto", nor the names
//    of their contributors, may be used to endorse or promote products derived
//    from this software without specific prior written permission. For written
//    permission, please contact info@pixinsight.com.
//
// 4. All products derived from this software, in any form whatsoever, must
//    reproduce the following acknowledgment in the end-user documentation
//    and/or other materials provided with the product:
//
//    "This product is based on software from the PixInsight project, developed
//    by Pleiades Astrophoto and its contributors (http://pixinsight.com/)."
//
//    Alternatively, if that is where third-party acknowledgments normally
//    appear, this acknowledgment must be reproduced in the product itself.
//
// THIS SOFTWARE IS PROVIDED BY PLEIADES ASTROPHOTO AND ITS CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL PLEIADES ASTROPHOTO OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, BUSINESS
// INTERRUPTION; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; AND LOSS OF USE,
// DATA OR PROFITS) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------

#ifndef __PCL_DrizzleData_h
#define __PCL_DrizzleData_h

/// \file pcl/DrizzleData.h

#include <pcl/Defs.h>

#include <pcl/File.h>
#include <pcl/SurfaceSplines.h>
#include <pcl/TimePoint.h>

namespace pcl
{

// ----------------------------------------------------------------------------

class PCL_CLASS XMLDocument;
class PCL_CLASS XMLElement;

/*!
 * \class DrizzleData
 * \brief Drizzle integration data parser and generator
 *
 * %DrizzleData implements support for the %XML drizzle data format (XDRZ
 * format, .xdrz file suffix).
 *
 * A drizzle image integration process has been formalized as the following
 * chain of subtasks on the PixInsight/PCL platform:
 *
 * \li 1. Demosaicing. Only required if the input data set has been mosaiced
 * with a color filter array, such as a Bayer filter.
 *
 * \li 2. Image registration. Generates image alignment information in the form
 * of alignment matrices (projective transformations) and/or two-dimensional
 * surface splines (for arbitrary distortion correction).
 *
 * \li 3. Image integration. Generates statistical data for each channel of the
 * integrated image. This includes estimates of location and scale, statistical
 * weights and pixel rejection data.
 *
 * \li 4. Drizzle integration. The input data for the drizzle algorithm is the
 * input image for subtasks 1 (Bayer drizzle) or 2 (normal drizzle). The
 * information generated by subtasks 2 and 3 is used to perform a drizzle image
 * integration process by inverse coordinate projection with image weighting,
 * image normalization, and pixel rejection.
 *
 * An XDRZ file stores all of the data generated by the above drizzle
 * integration subtasks as an %XML document. The %DrizzleData class is a
 * high-level interface for parsing and generation of these special files.
 *
 * This class also implements an internal read-only compatibility layer with
 * the old plain text drizzle data format (.drz file suffix) used by versions
 * of PixInsight before the 1.8.5 standard distribution. By using the class
 * constructor or the Parse() member function to load and parse an existing
 * file, the correct file format is detected automatically.
 *
 * \sa NormalizationData
 */
class PCL_CLASS DrizzleData
{
public:

   /*!
    * Represents a vector surface spline interpolation/approximation in two
    * dimensions. This class is used to implement image alignment with
    * arbitrary distortion correction.
    */
   typedef PointSurfaceSpline<DPoint>     vector_spline;

   /*!
    * Represents a coordinate interpolating/approximating surface spline used
    * for image registration with arbitrary distortion correction.
    */
   typedef vector_spline::spline          spline;

   /*!
    * A list of coordinates corresponding to pixels rejected by an image
    * integration task.
    */
   typedef Array<Point>                   rejection_coordinates;

   /*!
    * A set of per-channel pixel rejection coordinates.
    */
   typedef Array<rejection_coordinates>   rejection_data;

   /*!
    * Constructs an empty %DrizzleData object.
    */
   DrizzleData() = default;

   /*!
    * Copy constructor.
    */
   DrizzleData( const DrizzleData& ) = default;

   /*!
    * Move constructor.
    */
   DrizzleData( DrizzleData&& ) = default;

   /*!
    * Constructs a new %DrizzleData instance by loading and parsing a file.
    *
    * \param filePath   Path to an existing file that will be parsed. The file
    *                   contents can be in %XML drizzle data format (normally,
    *                   a file with the .xdrz suffix), or in old plain text
    *                   format (typically with the .drz suffix). This
    *                   constructor will detect the format in use from the
    *                   first bytes of the file, and will decode it
    *                   correspondingly.
    *
    * \param ignoreIntegrationData  If true, all drizzle data relative to the
    *                   image integration task will be ignored. This includes
    *                   statistical location and scale estimates for image
    *                   normalization, image weights, and pixel rejection data.
    *
    * This constructor validates the data retrieved from the specified file. It
    * throws the appropriate Error exceptions in the event of parsing errors or
    * invalid data.
    */
   DrizzleData( const String& filePath, bool ignoreIntegrationData = false )
   {
      Parse( filePath, ignoreIntegrationData );
   }

   /*!
    * Constructs a new %DrizzleData instance by parsing a well-formed %XML
    * document.
    *
    * \param xml        Reference to the source %XML document. This constructor
    *                   expects an %XML document in valid %XML drizzle data
    *                   format (.xdrz).
    *
    * \param ignoreIntegrationData  If true, all drizzle data relative to the
    *                   image integration task will be ignored. This includes
    *                   statistical location and scale estimates for image
    *                   normalization, image weights, and pixel rejection data.
    *
    * This constructor validates the data retrieved from the specified %XML
    * document. It throws an Error exception in the event of an invalid
    * document or invalid data.
    */
   DrizzleData( const XMLDocument& xml, bool ignoreIntegrationData = false )
   {
      Parse( xml, ignoreIntegrationData );
   }

   /*!
    * Constructs a new %DrizzleData instance by parsing an %XML element.
    *
    * \param element    Reference to the source %XML element. This constructor
    *                   expects an %XML document tree in valid %XML drizzle
    *                   data format (.xdrz) rooted at this element.
    *
    * \param ignoreIntegrationData  If true, all drizzle data relative to the
    *                   image integration task will be ignored. This includes
    *                   statistical location and scale estimates for image
    *                   normalization, image weights, and pixel rejection data.
    *
    * This constructor validates the data retrieved from the specified %XML
    * element. It throws an Error exception in the event of invalid data.
    */
   DrizzleData( const XMLElement& element, bool ignoreIntegrationData = false )
   {
      Parse( element, ignoreIntegrationData );
   }

   /*!
    * Virtual destructor.
    */
   virtual ~DrizzleData()
   {
   }

   /*!
    * Copy assignment operator. Returns a reference to this object.
    */
   DrizzleData& operator =( const DrizzleData& ) = default;

   /*!
    * Move assignment operator. Returns a reference to this object.
    */
   DrizzleData& operator =( DrizzleData&& ) = default;

   /*!
    * Returns the full path to the unregistered image file corresponding to the
    * drizzle image represented by this instance.
    *
    * The file identified by this function stores the input image of the image
    * registration task. This file can be used as input for a drizzle
    * integration task.
    *
    * \sa SetSourceFilePath()
    */
   const String& SourceFilePath() const
   {
      return m_sourceFilePath;
   }

   /*!
    * Sets the path to the unregistered image file corresponding to the drizzle
    * image represented by this instance.
    *
    * \sa SourceFilePath()
    */
   void SetSourceFilePath( const String& filePath )
   {
      m_sourceFilePath = File::FullPath( filePath );
   }

   /*!
    * Returns the full path to the mosaiced/unregistered image file
    * corresponding to the drizzle image represented by this instance.
    *
    * The file identified by this function stores the input image of the image
    * demosaicing (e.g., de-Bayering) task. This file can be used as input for
    * a Bayer drizzle integration task.
    *
    * \sa SetCFASourceFilePath(), CFASourcePattern()
    */
   const String& CFASourceFilePath() const
   {
      return m_cfaSourceFilePath;
   }

   /*!
    * Sets the path to the mosaiced/unregistered image file corresponding t
    * the drizzle image represented by this instance.
    *
    * \sa CFASourceFilePath(), SetCFASourcePattern()
    */
   void SetCFASourceFilePath( const String& filePath )
   {
      m_cfaSourceFilePath = File::FullPath( filePath );
   }

   /*!
    * Returns a string representation of the color filter array (CFA) used by
    * the mosaiced/unregistered image file corresponding to the drizzle image
    * represented by this instance.
    *
    * The CFA corresponds to the file represented by CFASourceFilePath(). For
    * the standard Bayer filters, this function returns "RGGB", "BGGR", "GBRG",
    * and "GRBG".
    *
    * \sa SetCFASourcePattern(), CFASourceFilePath()
    */
   const String& CFASourcePattern() const
   {
      return m_cfaSourcePattern;
   }

   /*!
    * Defines the color filter array (CFA) used by the mosaiced/unregistered
    * image file corresponding to the drizzle image represented by this
    * instance.
    *
    * See CFASourcePattern() for more information on CFA representations.
    *
    * \sa CFASourcePattern(), SetCFASourceFilePath()
    */
   void SetCFASourcePattern( const String& cfaPattern )
   {
      m_cfaSourcePattern = cfaPattern;
   }

   /*!
    * Returns the full path to the registered image file corresponding to the
    * drizzle image represented by this instance, or an empty string if that
    * file path is not available.
    *
    * The file identified by this function stores the output image of the image
    * registration task. This file can be used as input for a regular
    * (non-drizzle) integration task.
    *
    * \note This file <em>should not</em> be used as input for drizzle
    * integration, since it has already been registered and interpolated. Use
    * SourceFilePath() or CFASourceFilePath() as input for drizzle or Bayer
    * drizzle, respectively.
    *
    * \sa SetAlignmentTargetFilePath()
    */
   const String& AlignmentTargetFilePath() const
   {
      return m_alignTargetFilePath;
   }

   /*!
    * Sets the path to the registered image file corresponding to the drizzle
    * image represented by this instance.
    *
    * \sa AlignmentTargetFilePath()
    */
   void SetAlignmentTargetFilePath( const String& filePath )
   {
      m_alignTargetFilePath = File::FullPath( filePath );
   }

   /*!
    * Returns the width in pixels of the registration reference image.
    *
    * \sa ReferenceHeight(), SetReferenceDimensions()
    */
   int ReferenceWidth() const
   {
      return m_referenceWidth;
   }

   /*!
    * Returns the height in pixels of the registration reference image.
    *
    * \sa ReferenceWidth(), SetReferenceDimensions()
    */
   int ReferenceHeight() const
   {
      return m_referenceHeight;
   }

   /*!
    * Sets new registration reference image dimensions in pixels, \a width and
    * \a height, respectively.
    *
    * \sa ReferenceWidth(), ReferenceHeight()
    */
   void SetReferenceDimensions( int width, int height )
   {
      m_referenceWidth = width;
      m_referenceHeight = height;
   }

   /*!
    * Returns the number of channels or pixel sample planes in the image
    * represented by this instance.
    */
   int NumberOfChannels() const
   {
      return m_location.Length();
   }

   /*!
    * Returns the 3x3 alignment matrix for the drizzle image represented by
    * this instance. The returned matrix defines a projective geometric
    * transformation (homography).
    *
    * \sa SetAlignmentMatrix()
    */
   const Matrix& AlignmentMatrix() const
   {
      return m_H;
   }

   /*!
    * Defines a new 3x3 alignment matrix. This function is reserved for image
    * registration tasks involved in drizzle integration processes.
    *
    * \sa AlignmentMatrix()
    */
   void SetAlignmentMatrix( const Matrix& H )
   {
      m_H = H;
   }

   /*!
    * Returns the vector surface spline interpolation/approximation device in
    * two dimensions for the drizzle image represented by this instance. These
    * splines define an image registration transformation with corrections for
    * arbitrary distortions.
    *
    * \sa SetAlignmentSplines()
    */
   const vector_spline& AlignmentSplines() const
   {
      return m_S;
   }

   /*!
    * Sets a new vector surface spline interpolation/approximation device. This
    * function is reserved for image registration tasks involved in drizzle
    * integration processes.
    *
    * \sa AlignmentSplines()
    */
   void SetAlignmentSplines( const vector_spline& S )
   {
      m_S = S;
   }

   /*!
    * Returns true iff this instance defines a 3x3 alignment matrix for the
    * projective image registration transformation.
    *
    * \sa AlignmentMatrix(), HasAlignmentSplines()
    */
   bool HasAlignmentMatrix() const
   {
      return !m_H.IsEmpty();
   }

   /*!
    * Returns true iff this instance defines a vector surface spline
    * interpolation/approximation device.
    *
    * \sa AlignmentSplines(), HasAlignmentMatrix()
    */
   bool HasAlignmentSplines() const
   {
      return m_S.IsValid();
   }

   /*!
    * Returns the vector of per-channel robust estimates of location for the
    * drizzle image represented by this instance.
    *
    * See Scale() for a description of the output normalization procedure.
    *
    * Typically, the components of the vector returned by this function are
    * median values, or similar robust estimates of location, computed for each
    * channel of the image identified by AlignmentTargetFilePath(). These
    * estimates are normally generated by a regular integration task.
    *
    * \sa SetLocation(), ReferenceLocation(), Scale()
    */
   const DVector& Location() const
   {
      return m_location;
   }

   /*!
    * Sets a new vector of per-channel robust estimates of location for the
    * drizzle image represented by this instance.
    *
    * \sa Location(), ReferenceLocation(), SetScale()
    */
   void SetLocation( const DVector& v )
   {
      m_location = v;
   }

   /*!
    * Returns the vector of per-channel robust estimates of location for the
    * reference integration image associated with the drizzle image represented
    * by this instance.
    *
    * See Scale() for a description of the output normalization procedure.
    *
    * Typically, the components of the vector returned by this function are
    * median values, or similar robust estimates of location, computed for each
    * channel of an image used as reference by a regular integration task.
    *
    * \sa SetLocation(), ReferenceLocation(), Scale()
    */
   const DVector& ReferenceLocation() const
   {
      return m_referenceLocation;
   }

   /*!
    * Sets a new vector of per-channel robust estimates of location for the
    * reference integration image.
    *
    * \sa ReferenceLocation(), Location(), SetScale()
    */
   void SetReferenceLocation( const DVector& v )
   {
      m_referenceLocation = v;
   }

   /*!
    * Returns the vector of per-channel scaling factors for the drizzle image
    * represented by this instance.
    *
    * For normalization of output pixel samples in a channel c of the drizzle
    * image, the following equation must be applied:
    *
    * <tt>vc' = (vc - Lc)*Sc + L0c</tt>
    *
    * where vc is the source pixel sample, Lc is the location estimate given by
    * Location()[c], Sc is the scaling factor given by Scale()[c], L0c is the
    * reference location estimate given by ReferenceLocation()[c], and vc' is
    * the normalized pixel sample value.
    *
    * Typically, the components of the vector returned by this function are
    * computed from robust estimates of dispersion, such as MAD or the square
    * root of the biweight midvariance among many others, computed for each
    * channel of the image identified by AlignmentTargetFilePath() and
    * multiplied by the inverse of the corresponding estimates computed for an
    * integration reference image. Scaling factors are normally generated by a
    * regular integration task.
    *
    * \sa SetScale(), Location(), ReferenceLocation()
    */
   const DVector& Scale() const
   {
      if ( !m_scale.IsEmpty() )
         return m_scale;
      if ( m_unitScale.IsEmpty() )
         m_unitScale = DVector( 1.0, NumberOfChannels() );
      return m_unitScale;
   }

   /*!
    * Sets a new vector of per-channel scaling factors for the drizzle image
    * represented by this instance.
    *
    * \sa Scale(), SetLocation()
    */
   void SetScale( const DVector& v )
   {
      m_scale = v;
   }

   /*!
    * Returns the vector of per-channel statistical weights for the drizzle
    * image represented by this instance.
    *
    * Typically, the components of the returned vector are efficient
    * statistical weights computed for each channel of the image identified by
    * AlignmentTargetFilePath(). These weights are normally generated by a
    * regular integration task.
    *
    * \sa SetWeight(), Location(), ReferenceLocation(), Scale()
    */
   const DVector& Weight() const
   {
      if ( !m_weight.IsEmpty() )
         return m_weight;
      if ( m_unitWeight.IsEmpty() )
         m_unitWeight = DVector( 1.0, NumberOfChannels() );
      return m_unitWeight;
   }

   /*!
    * Sets a new vector of per-channel statistical weights for the drizzle
    * image represented by this instance.
    *
    * \sa Weight(), SetLocation(), SetReferenceLocation(), SetScale()
    */
   void SetWeight( const DVector& v )
   {
      m_weight = v;
   }

   /*!
    * Returns true iff this %DrizzleData object transports image integration
    * data for normalization and image combination. This includes at least
    * location and reference location estimates, and optionally (but usually
    * present) scaling factors and statistical image weights.
    */
   bool HasIntegrationData() const
   {
      return !m_location.IsEmpty();
   }

   /*!
    * Returns true iff this %DrizzleData object transports per-channel
    * statistical image weights. If no weight information is available, the
    * Weight() member function will return a reference to an internal vector
    * with all components set to one.
    */
   bool HasImageWeightsData() const
   {
      return !m_weight.IsEmpty();
   }

   /*!
    * Returns the vector of per-channel low pixel rejection counts for the
    * drizzle image represented by this instance.
    *
    * The components of the returned vector are the total amounts of rejected
    * low pixels for each channel of the image identified by
    * AlignmentTargetFilePath(). Pixel rejection counts are normally generated
    * by a regular image integration task.
    *
    * \sa RejectionHighCount(), RejectionMap()
    */
   const UI64Vector& RejectionLowCount() const
   {
      return m_rejectionLowCount;
   }

   /*!
    * Returns the vector of per-channel high pixel rejection counts for the
    * drizzle image represented by this instance.
    *
    * The components of the returned vector are the total amounts of rejected
    * high pixels for each channel of the image identified by
    * AlignmentTargetFilePath(). Pixel rejection counts are normally generated
    * by a regular image integration task.
    *
    * \sa RejectionLowCount(), RejectionMap()
    */
   const UI64Vector& RejectionHighCount() const
   {
      return m_rejectionHighCount;
   }

   /*!
    * Returns a reference to a pixel rejection map generated for the drizzle
    * image represented by this instance.
    *
    * This member function returns a reference to an 8-bit unsigned integer
    * image known as <em>drizzle rejection map</em>. Each channel of the
    * rejection map corresponds to the same channel of the image represented by
    * this object. For a channel index c, a pixel sample of a rejection map can
    * have the following values:
    *
    * \li 0 (no bit set): The corresponding pixel has not been rejected for
    * image channel c.
    *
    * \li Bit 0 set: High statistical rejection: The pixel has been rejected
    * above the estimated central value of its pixel integration stack using a
    * statistical rejection algorithm (such as sigma clipping).
    *
    * \li Bit 1 set: Low statistical rejection: The pixel has been rejected
    * below the estimated central value of its pixel integration stack using a
    * statistical rejection algorithm (such as sigma clipping).
    *
    * \li Bit 2 set: High range rejection: The pixel has been rejected because
    * its value is greater than or equal to a prescribed upper limit.
    *
    * \li Bit 3 set: Low range rejection: The pixel has been rejected because
    * its value is smaller than or equal to a prescribed lower limit.
    *
    * \li Bit 4 set: High large-scale rejection: The pixel belongs to a bright
    * large-scale image structure that has been rejected using multiscale
    * analysis techniques.
    *
    * \li Bit 5 set: Low large-scale rejection: The pixel belongs to a dark
    * large-scale image structure that has been rejected using multiscale
    * analysis techniques.
    *
    * Bits 6 and 7 are reserved for future extensions and should be zero in the
    * current implementation.
    *
    * Rejection maps are automatically generated from pixel rejection data
    * stored in .xdrz files. Pixel rejection data are normally generated by a
    * regular image integration task.
    *
    * If no pixel rejection information is available in this instance, this
    * function returns a reference to an empty image.
    *
    * \sa SetRejectionMap(), HasRejectionData(), IsRejected()
    */
   const UInt8Image& RejectionMap() const
   {
      return m_rejectionMap;
   }

   /*!
    * Returns true iff the pixel at the specified pixel coordinates has been
    * rejected for the drizzle image represented by this instance.
    *
    * \sa RejectionMap(), HasRejectionData()
    */
   bool IsRejected( const Point& position, int channel = 0 ) const
   {
      return  m_rejectionMap.Includes( position )
          &&  m_rejectionMap.IsValidChannelIndex( channel )
          && (m_rejectionMap( position, channel ) & 0x3F) != 0;
   }

   /*!
    * Defines per-channel pixel rejection data for the drizzle image
    * represented by this instance. The specified \a map image is a drizzle
    * rejection map as described in RejectionMap().
    *
    * \sa RejectionMap(), HasRejectionData()
    */
   void SetRejectionMap( const UInt8Image& map )
   {
      m_rejectionMap = map;
   }

   /*!
    * Returns true iff pixel rejection information is available for the drizzle
    * image represented by this instance.
    */
   bool HasRejectionData() const
   {
      return !m_rejectionMap.IsEmpty();
   }

   /*!
    * Returns the UTC time this drizzle data was created, or an invalid
    * TimePoint instance if creation time information is not available.
    */
   TimePoint CreationTime() const
   {
      return m_creationTime;
   }

   /*!
    * Clears all of the drizzle integration data transported by this instance,
    * yielding an empty %DrizzleData object.
    */
   void Clear();

   /*!
    * Clears the image integration  data transported by this instance. This
    * includes statistical location and scale estimates for image
    * normalization, image weights, and pixel rejection data.
    */
   void ClearIntegrationData();

   /*!
    * Loads and parses a drizzle data file.
    *
    * \param filePath   Path to an existing file that will be parsed. The file
    *                   contents can be in %XML drizzle data format (normally,
    *                   a file with the .xdrz suffix), or in old plain text
    *                   format (typically with the .drz suffix). This function
    *                   will detect the format in use from the first bytes read
    *                   from the file, and will decode it correspondingly.
    *
    * \param ignoreIntegrationData  If true, all drizzle data relative to the
    *                   image integration task will be ignored. This includes
    *                   statistical location and scale estimates for image
    *                   normalization, image weights, and pixel rejection data.
    *
    * All of the previous data transported by this instance will be replaced
    * with new data acquired from the specified file.
    *
    * This function validates the data retrieved from the specified file. It
    * throws an Error exception in the event of parsing errors or invalid data.
    */
   void Parse( const String& filePath, bool ignoreIntegrationData = false );

   /*!
    * Parses a well-formed %XML document.
    *
    * \param xml        Reference to the source %XML document. This member
    *                   function expects an %XML document in valid %XML
    *                   drizzle data format (.xdrz).
    *
    * \param ignoreIntegrationData  If true, all drizzle data relative to the
    *                   image integration task will be ignored. This includes
    *                   statistical location and scale estimates for image
    *                   normalization, image weights, and pixel rejection data.
    *
    * All of the previous data transported by this instance will be replaced
    * with new data acquired from the specified %XML contents.
    *
    * This function validates the data retrieved from the specified %XML
    * document. It throws an Error exception in the event of an invalid
    * document or invalid data.
    */
   void Parse( const XMLDocument& xml, bool ignoreIntegrationData = false );

   /*!
    * Parses an %XML element.
    *
    * \param element    Reference to the source %XML element. This member
    *                   function expects an %XML document tree in valid %XML
    *                   drizzle data format (.xdrz) rooted at this element.
    *
    * \param ignoreIntegrationData  If true, all drizzle data relative to the
    *                   image integration task will be ignored. This includes
    *                   statistical location and scale estimates for image
    *                   normalization, image weights, and pixel rejection data.
    *
    * All of the previous data transported by this instance will be replaced
    * with new data acquired from the specified %XML contents.
    *
    * This function validates the data retrieved from the specified %XML
    * element. It throws an Error exception in the event of invalid data.
    */
   void Parse( const XMLElement& element, bool ignoreIntegrationData = false );

   /*!
    * Returns true if drizzle data compression is enabled for serializations
    * performed with this object.
    *
    * Drizzle data compression is enabled by default for newly constructed
    * %DrizzleData objects. Currently the LZ4 compression codec is applied for
    * serialization of pixel rejection data.
    *
    * \sa EnableCompression(), DisableCompression()
    */
   bool IsCompressionEnabled() const
   {
      return m_compressionEnabled;
   }

   /*!
    * Enables compression of serialized drizzle data.
    *
    * \sa IsCompressionEnabled(), DisableCompression()
    */
   void EnableCompression( bool enable = true )
   {
      m_compressionEnabled = enable;
   }

   /*!
    * Disables compression of serialized drizzle data.
    *
    * \sa IsCompressionEnabled(), EnableCompression()
    */
   void DisableCompression( bool disable = true )
   {
      EnableCompression( !disable );
   }

   /*!
    * Serializes the drizzle integration data transported by this object as a
    * new %XML document in .xdrz format.
    *
    * The returned XMLDocument object has been allocated dynamically. The
    * caller is responsible for destroying and deallocating it as appropriate.
    */
   XMLDocument* Serialize() const;

   /*!
    * Serializes the drizzle integration data transported by this object as a
    * new %XML document file in .xdrz format. The file will be newly created at
    * the specified file \a path.
    *
    * \warning If a file already exists at the specified \a path, its previous
    * contents will be lost after calling this function.
    */
   void SerializeToFile( const String& path ) const;

private:

           String         m_sourceFilePath;
           String         m_cfaSourceFilePath;
           String         m_cfaSourcePattern;
           String         m_alignTargetFilePath;
           int            m_referenceWidth  = -1;
           int            m_referenceHeight = -1;
           Matrix         m_H;
           vector_spline  m_S;
           Vector         m_location;
           Vector         m_referenceLocation;
           Vector         m_scale;
   mutable Vector         m_unitScale;
           Vector         m_weight;
   mutable Vector         m_unitWeight;
           UI64Vector     m_rejectionLowCount;
           UI64Vector     m_rejectionHighCount;
           UInt8Image     m_rejectionMap;
           TimePoint      m_creationTime;
           bool           m_compressionEnabled = true;

           // Working data for old text format compatibility.
           spline         m_Sx;
           spline         m_Sy;
           rejection_data m_rejectLowData;
           rejection_data m_rejectHighData;

   void ParseRejectionMap( const XMLElement& );
   void SerializeRejectionMap( XMLElement* ) const;

   static void ParseSpline( spline&, const XMLElement& );
   static void SerializeSpline( XMLElement*, const spline& );

   /*!
    * \internal
    * \class DrizzleData::PlainTextDecoder
    * \brief Read-only compatibility with old plain text .drz files
    */
   class PlainTextDecoder
   {
   public:

      PlainTextDecoder( DrizzleData* data, bool ignoreIntegrationData ) :
         m_data( data ),
         m_ignoreIntegrationData( ignoreIntegrationData )
      {
      }

      virtual ~PlainTextDecoder()
      {
      }

      void Decode( IsoString&, size_type start = 0, size_type end = 0 );

   protected:

      PlainTextDecoder() = default;

   private:

      DrizzleData* m_data = nullptr;
      bool         m_ignoreIntegrationData = false;

      virtual void ProcessBlock( IsoString&, const IsoString&, size_type, size_type );

      static rejection_coordinates ParseRejectionCoordinates( IsoString&, size_type, size_type );
      static rejection_data ParseRejectionData( IsoString&, size_type, size_type );
      static spline ParseSurfaceSpline( IsoString&, size_type, size_type );
   };

   class PlainTextSplineDecoder : public PlainTextDecoder
   {
   public:

      PlainTextSplineDecoder( spline& S ) : m_S( S )
      {
      }

      virtual ~PlainTextSplineDecoder()
      {
      }

   private:

      spline& m_S;

      void ProcessBlock( IsoString&, const IsoString&, size_type, size_type ) override;
   };
};

// ----------------------------------------------------------------------------

} // pcl

#endif   // __PCL_DrizzleData_h

// ----------------------------------------------------------------------------
// EOF pcl/DrizzleData.h - Released 2018-12-12T09:24:21Z
