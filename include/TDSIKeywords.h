/* =============== GPL HEADER =====================
 * TDSKeywords.h is part of TuxDroidScript Interpreter
 * Copyleft (C) 2014 - Joel Matteotti <joel _DOT_ matteotti _AT_ free _DOT_ fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 *
 * ====================================================
*/

//
// Manage language files
//

#ifndef __TDSKEYWORDS_H__
#define __TDSKEYWORDS_H__

//Specific language Keywords
#ifdef TDS_LANG_FR
	#include <lang/TDSIKeywords_FR.h>
#endif

#ifdef TDS_LANG_EN
	#include <lang/TDSIKeywords_EN.h>
#endif


//Standard Keywords

//Mathematic:

#define KW_MATH_EQUAL "="
#define KW_MATH_ADD "+"
#define KW_MATH_SUB "-"
#define KW_MATH_DIV "/"


#endif
