/***************************************************************************
                          swedish_stem.h  -  description
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

#ifndef __SWEDISH_STEM_H__
#define __SWEDISH_STEM_H__

#include "stemming.h"

namespace stemming
	{
	//------------------------------------------------------
	template<typename Tchar_type = char,
			typename Tchar_traits = std::char_traits<Tchar_type> >
	class swedish_stem : stem<Tchar_type,Tchar_traits>
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
			m_r1 = m_r2 = m_rv =0;

			trim_western_punctuation(text);

			//see where the R1 and R2 sections begin
			//R1 is the first consanant after the first vowel
			find_r1(text, "aeiouyåäöAEIOUYÅÄÖ");
			if (m_r1 == static_cast<int>(text.length() ) )
				{
				return;
				}

			//R1 must have at least 3 characters in front of it
			if (m_r1 < 3)
				{
				m_r1 = 3;	
				}
			//swedish does not use R2

			////step 1:
			step_1(text);
			////step 2:
			step_2(text);
			////step 3:
			step_3(text);
			}
	private:
		//---------------------------------------------
		void step_1(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (delete_if_is_in_r1(text, "heterna", 7, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "hetens", 6, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "arna", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "erna", 4, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "orna", 4, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "ande", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "arne", 4, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "aste", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "anden", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "heten", 5, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "heter", 5, false) )
				{
				return;
				}			
			else if (delete_if_is_in_r1(text, "arnas", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ernas", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ornas", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "arens", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "andet", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "andes", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "aren", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "erns", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ades", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "are", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ade", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "het", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ast", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ens", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ern", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "at", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "es", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "or", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ad", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "as", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ar", 2, false) )
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
			else if (delete_if_is_in_r1(text, "a", 1, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "e", 1, false) )
				{
				return;
				}
			else if (is_suffix_in_r1(text, "s", 1) )
				{
				if (string_util::is_one_of(text[text.length()-2],
					"bcdfghjklmnoprtvyBCDFGHJKLMNOPRTVY") )
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
			if (is_suffix_in_r1(text, "dd", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			else if (is_suffix_in_r1(text, "gd", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			else if (is_suffix_in_r1(text, "nn", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			else if (is_suffix_in_r1(text, "dt", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			else if (is_suffix_in_r1(text, "gt", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			else if (is_suffix_in_r1(text, "kt", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			else if (is_suffix_in_r1(text, "tt", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			}
		//---------------------------------------------
		void step_3(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (is_suffix_in_r1(text, "fullt", 5) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			else if (is_suffix_in_r1(text, "löst", 4) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			else if (delete_if_is_in_r1(text, "lig", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "els", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ig", 2, false) )
				{
				return;
				}
			}		
		};
	}

#endif //__SWEDISH_STEM_H__
