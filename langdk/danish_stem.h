/***************************************************************************
                          danish_stem.h  -  description
                             -------------------
    begin                : Sat Apr 26 2003
    copyright            : (C) 2003 by Blake Madden
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#ifndef __DANISH_STEM_H__
#define __DANISH_STEM_H__


#include "stemming.h"

namespace stemming
	{
	//------------------------------------------------------
	template<typename Tchar_type,
			typename Tchar_traits>
	class danish_stem : stem<Tchar_type,Tchar_traits>
		{
	public:
				//---------------------------------------------
		void operator()(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (text.length() < 3)
				{
				return;
				}

			//reset internal data
			this->m_r1 = this->m_r2 = this->m_rv =0;

			trim_western_punctuation(text);

			//see where the R1 and R2 sections begin
			//R1 is the first consanant after the first vowel
			find_r1(text, "aeiouyæåøAEIOUYÆÅØ");
			if (this->m_r1 == text.length() )
				{
				return;
				}
			//R1 must have at least 3 characters in front of it
			if (this->m_r1 < 3)
				{
				this->m_r1 = 3;
				}
			//danish does not use R2

			////step 1:
			step_1(text);
			////step 2:
			step_2(text);
			////step 3:
			step_3(text);
			////step 4:
			step_4(text);
			}
	private:
		//---------------------------------------------
		void step_1(std::basic_string<Tchar_type, Tchar_traits>& text)
			{			
			if (delete_if_is_in_r1(text, "erendes", 7, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "erende", 6, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "hedens", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ethed", 5, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "erede", 5, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "heden", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "heder", 5, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "endes", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ernes", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "erens", 5, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "erets", 5, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "eres", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "enes", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "heds", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "erer", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "eren", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "erne", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ende", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ered", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "eret", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "hed", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ets", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ere", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ene", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ens", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ers", 3, false) )
				{
				return;
				}

			else if (delete_if_is_in_r1(text, "et", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "es", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "er", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "en", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "e", 1, false) )
				{
				return;
				}
			else if (is_suffix_in_r1(text, "s", 1) )
				{
				if (string_util::is_one_of(text[text.length()-2], "abcdfghjklmnoprtvyzåABCDGHJKLMNOPRTVYZÅ") )
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				return;
				}
			}

		//---------------------------------------------
		void step_2(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (is_suffix_in_r1(text, "gd", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				return;
				}
			else if (is_suffix_in_r1(text, "dt", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				return;
				}
			else if (is_suffix_in_r1(text, "gt", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				return;
				}
			else if (is_suffix_in_r1(text, "kt", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				return;
				}
			}
		//---------------------------------------------
		void step_3(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			//do this check separately
			if (is_suffix(text, "igst", 4) )
				{
				text.erase(text.end()-2, text.end() );
				update_r_sections(text);
				}
			//now start looking for the longest suffix
			if (delete_if_is_in_r1(text, "elig", 4, false) )
				{
				step_2(text);
				return;
				}
			else if (is_suffix_in_r1(text, "løst", 4) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			else if (delete_if_is_in_r1(text, "lig", 3, false) )
				{
				step_2(text);
				return;
				}
			else if (delete_if_is_in_r1(text, "els", 3, false) )
				{
				step_2(text);
				return;
				}
			else if (delete_if_is_in_r1(text, "ig", 2, false) )
				{
				step_2(text);
				return;
				}
			}

		//---------------------------------------------
		void step_4(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			//undouble consecutive (same) consanants if either are in R1 section
			if (this->m_r1 <= text.length()-1 &&
				Tchar_traits::eq(text[text.length()-2], text[text.length()-1]) )
				{
				if (!string_util::is_one_of(text[text.length()-2], "aeiouyæåøAEIOUYÆÅØ") )
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				}
			}
		};
	}

#endif //__DANISH_STEM_H__
