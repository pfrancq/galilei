/***************************************************************************
                          norwegian_stem.h  -  description
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

#ifndef __NORWEGIAN_STEM_H__
#define __NORWEGIAN_STEM_H__

#include "stemming.h"

namespace stemming
	{
	//------------------------------------------------------
	template<typename Tchar_type = char,
			typename Tchar_traits = std::char_traits<Tchar_type> >
	class norwegian_stem : stem<Tchar_type,Tchar_traits>
		{
	public:
				//---------------------------------------------
		void operator()(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (text.length() < 3)
				{
				return;
				}
			trim_western_punctuation(text);

			//reset internal data
			this->m_r1 = this->m_r2 = this->m_rv =0;

			find_r1(text, "aeioøuyåæÅAÆEIOØUY");
			if (this->m_r1 == static_cast<int>(text.length() ) )
				{
				return;
				}
			//R1 must have at least 3 characters in front of it
			if (this->m_r1 < 3)
				{
				this->m_r1 = 3;
				}
			//norwegian does not use R2

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
			if (delete_if_is_in_r1(text, "hetenes", 7, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "hetene", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "hetens", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "heter", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "heten", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "endes", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ande", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ende", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "edes", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "enes", 4, false) )
				{
				return;
				}
			if (is_suffix_in_r1(text, "erte", 4) )
				{
				text.erase(text.end()-2, text.end() );
				update_r_sections(text);
				return;
				}
			else if (delete_if_is_in_r1(text, "ers", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ets", 3, false) )
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
			else if (delete_if_is_in_r1(text, "ene", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ane", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "ede", 3, false) )
				{
				return;
				}
			else if (is_suffix_in_r1(text, "ert", 3) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
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
			else if (delete_if_is_in_r1(text, "e", 1,false) )
				{
				return;
				}
			else if (is_suffix_in_r1(text, "s", 1) )
				{
				if (this->m_r1 <= text.length()-1)
					{
					if (string_util::is_one_of(text[text.length()-2],
						"bcdfghjklmnoprtvyzBCDFGHJKLMNOPRTVYZ") )
						{
						text.erase(text.end()-1, text.end() );
						update_r_sections(text);
						return;
						}
					}
				}
			}

		//---------------------------------------------
		void step_2(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (is_suffix_in_r1(text, "dt", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				return;
				}
			else if (is_suffix_in_r1(text, "vt", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				return;
				}
			}
		//---------------------------------------------
		void step_3(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (delete_if_is_in_r1(text, "hetslov", 7, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "slov", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "elov", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "elig", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "eleg", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "lov", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "els", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "lig", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "eig", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "leg", 3, false) )
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

#endif //__NORWEGIAN_STEM_H__
