//     ____   ______ __
//    / __ \ / ____// /
//   / /_/ // /    / /
//  / ____// /___ / /___   PixInsight Class Library
// /_/     \____//_____/   PCL 02.01.11.0937
// ----------------------------------------------------------------------------
// Standard TIFF File Format Module Version 01.00.07.0363
// ----------------------------------------------------------------------------
// TIFFFormat.h - Released 2018-12-12T09:25:15Z
// ----------------------------------------------------------------------------
// This file is part of the standard TIFF PixInsight module.
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

#ifndef __TIFFFormat_h
#define __TIFFFormat_h

#include "TIFF.h"

#include <pcl/MetaFileFormat.h>

namespace pcl
{

// ----------------------------------------------------------------------------

class TIFFFormat : public MetaFileFormat
{
public:

   TIFFFormat();

   virtual IsoString Name() const;

   virtual StringList FileExtensions() const;
   virtual IsoStringList MimeTypes() const;

   virtual uint32 Version() const;

   virtual String Description() const;
   virtual String Implementation() const;

   virtual String IconImageFile() const;

   virtual bool CanStore32Bit() const;
   virtual bool CanStoreFloat() const;
   virtual bool CanStoreDouble() const;
   virtual bool CanStoreResolution() const;
   virtual bool CanStoreICCProfiles() const;
   virtual bool SupportsCompression() const;
   virtual bool CanEditPreferences() const;
   virtual bool UsesFormatSpecificData() const;

   virtual bool ValidateFormatSpecificData( const void* data ) const;
   virtual void DisposeFormatSpecificData( void* data ) const;

   virtual FileFormatImplementation* Create() const;

   virtual bool EditPreferences() const;

   // TIFF format-specific data

   struct FormatOptions
   {
      uint32            signature;
      TIFFImageOptions  options;

      FormatOptions();
      FormatOptions( const FormatOptions& );

      static FormatOptions* FromGenericDataBlock( const void* );
   };

   // Default TIFF format options.
   static TIFFImageOptions DefaultOptions();

   // Defines how to proceed when there are out of range values in a just-read
   // floating point TIFF image.
   enum out_of_range_policy
   {
      OutOfRangePolicy_SetRange,    // use LowerRange() & UpperRange() values
      OutOfRangePolicy_AskIfOut,    // if there are out of range values, ask range and action
      OutOfRangePolicy_AskAlways,   // always ask for range and action (truncate, rescale)
      OutOfRangePolicy_Default = OutOfRangePolicy_SetRange
   };

   // Defines how to fix out of range pixel values in a just-read floating
   // point TIFF image.
   enum out_of_range_fix_mode
   {
      OutOfRangeFix_Rescale,        // rescale existing values to [lowerRange,upperRange]
      OutOfRangeFix_Truncate,       // truncate existing values to [lowerRange,upperRange]
      OutOfRangeFix_Ignore,         // allow (do not fix) out-of-range values
      OutOfRangeFix_Default = OutOfRangeFix_Rescale
   };

   struct OutOfRangePolicyOptions
   {
      double                lowerRange        = 0;
      double                upperRange        = 1;
      out_of_range_policy   outOfRangePolicy  = OutOfRangePolicy_Default;
      out_of_range_fix_mode outOfRangeFixMode = OutOfRangeFix_Default;
   };

   static OutOfRangePolicyOptions DefaultOutOfRangePolicyOptions();

   // Overridden embedding options.
   struct EmbeddingOverrides
   {
      bool overrideICCProfileEmbedding = false;
      bool embedICCProfiles            = false;
   };

   static EmbeddingOverrides DefaultEmbeddingOverrides();
};

// ----------------------------------------------------------------------------

} // pcl

#endif   // __TIFFFormat_h

// ----------------------------------------------------------------------------
// EOF TIFFFormat.h - Released 2018-12-12T09:25:15Z
