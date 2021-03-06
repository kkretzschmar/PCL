//     ____   ______ __
//    / __ \ / ____// /
//   / /_/ // /    / /
//  / ____// /___ / /___   PixInsight Class Library
// /_/     \____//_____/   PCL 02.01.11.0938
// ----------------------------------------------------------------------------
// Standard ColorSpaces Process Module Version 01.01.00.0374
// ----------------------------------------------------------------------------
// RGBWorkingSpaceParameters.h - Released 2019-01-21T12:06:41Z
// ----------------------------------------------------------------------------
// This file is part of the standard ColorSpaces PixInsight module.
//
// Copyright (c) 2003-2019 Pleiades Astrophoto S.L. All Rights Reserved.
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

#ifndef __RGBWorkingSpaceParameters_h
#define __RGBWorkingSpaceParameters_h

#include <pcl/MetaParameter.h>

namespace pcl
{

PCL_BEGIN_LOCAL

// ----------------------------------------------------------------------------

class RGBWSChannelTable : public MetaTable
{
public:

   RGBWSChannelTable( MetaProcess* );

   virtual IsoString Id() const;

   virtual size_type MinLength() const;
   virtual size_type MaxLength() const;
};

extern RGBWSChannelTable*  TheRGBWSChannelTableParameter;

// ----------------------------------------------------------------------------

class RGBWSLuminanceCoefficient : public MetaFloat
{
public:

   RGBWSLuminanceCoefficient( MetaTable* );

   virtual IsoString Id() const;
   virtual int Precision() const;

   virtual double MinimumValue() const;
   virtual double MaximumValue() const;
};

extern RGBWSLuminanceCoefficient*  TheRGBWSLuminanceCoefficientParameter;

// ----------------------------------------------------------------------------

class RGBWSChromaticityX : public MetaFloat
{
public:

   RGBWSChromaticityX( MetaTable* );

   virtual IsoString Id() const;
   virtual int Precision() const;

   virtual double MinimumValue() const;
   virtual double MaximumValue() const;
};

extern RGBWSChromaticityX*   TheRGBWSChromaticityXParameter;

// ----------------------------------------------------------------------------

class RGBWSChromaticityY : public MetaFloat
{
public:

   RGBWSChromaticityY( MetaTable* );

   virtual IsoString Id() const;
   virtual int Precision() const;

   virtual double MinimumValue() const;
   virtual double MaximumValue() const;
};

extern RGBWSChromaticityY*   TheRGBWSChromaticityYParameter;

// ----------------------------------------------------------------------------

class RGBWSGamma : public MetaFloat
{
public:

   RGBWSGamma( MetaProcess* );

   virtual IsoString Id() const;
   virtual int Precision() const;

   virtual double MinimumValue() const;
   virtual double MaximumValue() const;
   virtual double DefaultValue() const;
};

extern RGBWSGamma*  TheRGBWSGammaParameter;

// ----------------------------------------------------------------------------

class RGBWSsRGBG : public MetaBoolean
{
public:

   RGBWSsRGBG( MetaProcess* );

   virtual IsoString Id() const;

   virtual bool DefaultValue() const;
};

extern RGBWSsRGBG*  TheRGBWSsRGBGParameter;

// ----------------------------------------------------------------------------

class RGBWSApplyGlobal : public MetaBoolean
{
public:

   RGBWSApplyGlobal( MetaProcess* );

   virtual IsoString Id() const;

   virtual bool DefaultValue() const;
};

extern RGBWSApplyGlobal*   TheRGBWSApplyGlobalParameter;

// ----------------------------------------------------------------------------

PCL_END_LOCAL

} // pcl

#endif   // __RGBWorkingSpaceParameters_h

// ----------------------------------------------------------------------------
// EOF RGBWorkingSpaceParameters.h - Released 2019-01-21T12:06:41Z
