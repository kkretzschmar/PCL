//     ____   ______ __
//    / __ \ / ____// /
//   / /_/ // /    / /
//  / ____// /___ / /___   PixInsight Class Library
// /_/     \____//_____/   PCL 02.01.11.0927
// ----------------------------------------------------------------------------
// Standard Convolution Process Module Version 01.01.03.0272
// ----------------------------------------------------------------------------
// ConvolutionProcess.cpp - Released 2018-11-23T18:45:58Z
// ----------------------------------------------------------------------------
// This file is part of the standard Convolution PixInsight module.
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

#include "ConvolutionProcess.h"
#include "ConvolutionParameters.h"
#include "ConvolutionInstance.h"
#include "ConvolutionInterface.h"

#include <pcl/Console.h>
#include <pcl/Arguments.h>
#include <pcl/View.h>
#include <pcl/Exception.h>

namespace pcl
{

// ----------------------------------------------------------------------------

#include "ConvolutionIcon.xpm"

// ----------------------------------------------------------------------------

ConvolutionProcess* TheConvolutionProcess = 0;

// ----------------------------------------------------------------------------

ConvolutionProcess::ConvolutionProcess() : MetaProcess()
{
   TheConvolutionProcess = this;

   // Instantiate process parameters
   new ConMode( this );
   new ConSigma( this );
   new ConShape( this );
   new ConAspectRatio( this );
   new ConRotationAngle( this );
   new ConSourceCode( this );
   new ConRescaleHighPass( this );
   new ConViewId( this );
}

// ----------------------------------------------------------------------------

IsoString ConvolutionProcess::Id() const
{
   return "Convolution";
}

// ----------------------------------------------------------------------------

IsoString ConvolutionProcess::Category() const
{
   return "Convolution";
}

// ----------------------------------------------------------------------------

uint32 ConvolutionProcess::Version() const
{
   return 0x100; // required
}

// ----------------------------------------------------------------------------

String ConvolutionProcess::Description() const
{
   // ### TODO
   return
   "";
}

// ----------------------------------------------------------------------------

const char** ConvolutionProcess::IconImageXPM() const
{
   return ConvolutionIcon_XPM;
}
// ----------------------------------------------------------------------------

ProcessInterface* ConvolutionProcess::DefaultInterface() const
{
   return TheConvolutionInterface;
}
// ----------------------------------------------------------------------------

ProcessImplementation* ConvolutionProcess::Create() const
{
   return new ConvolutionInstance( this );
}

// ----------------------------------------------------------------------------

ProcessImplementation* ConvolutionProcess::Clone( const ProcessImplementation& p ) const
{
   const ConvolutionInstance* instPtr = dynamic_cast<const ConvolutionInstance*>( &p );
   return (instPtr != 0) ? new ConvolutionInstance( *instPtr ) : 0;
}

// ----------------------------------------------------------------------------

bool ConvolutionProcess::CanProcessCommandLines() const
{
   return true;
}

// ----------------------------------------------------------------------------

// ### TODO

static void ShowHelp()
{
   Console().Write(
"<raw>"
"Usage: Convolution [<arg_list>] [<view_list>]"
"\n"
"\n--interface"
"\n"
"\n      Launches the interface of this process."
"\n"
"\n--help"
"\n"
"\n      Displays this help and exits."
"</raw>" );
}

int ConvolutionProcess::ProcessCommandLine( const StringList& argv ) const
{
   ArgumentList arguments =
      ExtractArguments( argv, ArgumentItemMode::AsViews, ArgumentOption::AllowWildcards );

   ConvolutionInstance instance( this );

   bool launchInterface = false;
   int count = 0;

   for ( ArgumentList::const_iterator i = arguments.Begin(); i != arguments.End(); ++i )
   {
      const Argument& arg = *i;

      if ( arg.IsNumeric() )
      {
         throw Error( "Unknown numeric argument: " + arg.Token() );
      }
      else if ( arg.IsString() )
      {
         throw Error( "Unknown string argument: " + arg.Token() );
      }
      else if ( arg.IsSwitch() )
      {
         throw Error( "Unknown switch argument: " + arg.Token() );
      }
      else if ( arg.IsLiteral() )
      {
         // These are standard parameters that all processes should provide.
         if ( arg.Id() == "-interface" )
            launchInterface = true;
         else if ( arg.Id() == "-help" )
         {
            ShowHelp();
            return 0;
         }
         else
            throw Error( "Unknown argument: " + arg.Token() );
      }
      else if ( arg.IsItemList() )
      {
         ++count;

         if ( arg.Items().IsEmpty() )
         {
            Console().WriteLn( "No view(s) found: " + arg.Token() );
            throw;
         }

         for ( StringList::const_iterator j = arg.Items().Begin(); j != arg.Items().End(); ++j )
         {
            View v = View::ViewById( *j );
            if ( v.IsNull() )
               throw Error( "No such view: " + *j );
            instance.LaunchOn( v );
         }
      }
   }

   if ( launchInterface )
      instance.LaunchInterface();
   else if ( count == 0 )
   {
      if ( ImageWindow::ActiveWindow().IsNull() )
         throw Error( "There is no active image window." );
      instance.LaunchOnCurrentView();
   }

   return 0;
}

// ----------------------------------------------------------------------------

} // pcl

// ----------------------------------------------------------------------------
// EOF ConvolutionProcess.cpp - Released 2018-11-23T18:45:58Z
