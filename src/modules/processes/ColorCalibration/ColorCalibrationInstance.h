// ****************************************************************************
// PixInsight Class Library - PCL 02.00.13.0689
// Standard ColorCalibration Process Module Version 01.02.00.0168
// ****************************************************************************
// ColorCalibrationInstance.h - Released 2014/10/29 07:34:54 UTC
// ****************************************************************************
// This file is part of the standard ColorCalibration PixInsight module.
//
// Copyright (c) 2003-2014, Pleiades Astrophoto S.L. All Rights Reserved.
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
// ****************************************************************************

#ifndef __ColorCalibrationInstance_h
#define __ColorCalibrationInstance_h

#include <pcl/ProcessImplementation.h>
#include <pcl/MetaParameter.h> // for pcl_bool

namespace pcl
{

// ----------------------------------------------------------------------------
// ColorCalibrationInstance
// ----------------------------------------------------------------------------

class ColorCalibrationInstance : public ProcessImplementation
{
public:

   ColorCalibrationInstance( const MetaProcess* );
   ColorCalibrationInstance( const ColorCalibrationInstance& );

   virtual void Assign( const ProcessImplementation& );

   virtual bool CanExecuteOn( const View&, pcl::String& whyNot ) const;
   virtual bool ExecuteOn( View& );

   virtual void* LockParameter( const MetaParameter*, size_type tableRow );
   virtual bool AllocateParameter( size_type sizeOrLength, const MetaParameter* p, size_type tableRow );
   virtual size_type ParameterLength( const MetaParameter* p, size_type tableRow ) const;

   // -------------------------------------------------------------------------

private:

   String   whiteReferenceViewId;      // if empty, use $T
   float    whiteLow;
   float    whiteHigh;
   pcl_bool whiteUseROI;
   Rect     whiteROI;
   pcl_bool structureDetection;
   int32    structureLayers;
   int32    noiseLayers;

   pcl_bool manualWhiteBalance;
   float    manualRedFactor;
   float    manualGreenFactor;
   float    manualBlueFactor;

   String   backgroundReferenceViewId; // if empty, use $T
   float    backgroundLow;
   float    backgroundHigh;
   pcl_bool backgroundUseROI;
   Rect     backgroundROI;

   pcl_bool outputWhiteReferenceMask;
   pcl_bool outputBackgroundReferenceMask;

   friend class ColorCalibrationProcess;
   friend class ColorCalibrationInterface;
   friend class ColorCalibrationEngine;
};

// ----------------------------------------------------------------------------

} // pcl

#endif   // __ColorCalibrationInstance_h

// ****************************************************************************
// EOF ColorCalibrationInstance.h - Released 2014/10/29 07:34:54 UTC
