/***************************************************************************
                          german_stem.h  -  description
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

#ifndef __GERMAN_STEM_H__
#define __GERMAN_STEM_H__

#include "stemming.h"

namespace stemming
	{
	//------------------------------------------------------
	template<typename Tchar_type = char,
			typename Tchar_traits = std::char_traits<Tchar_type> >
	class german_stem : stem<Tchar_type,Tchar_traits>
		{
	public:
		//---------------------------------------------
		void operator()(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			//change 'ß' to "ss"
			string_util::replace_all(text, "ß", "ss");
			if (text.length() < 3)
				{
				remove_german_umlauts(text);
				return;
				}

			//reset internal data
			this->m_r1 = this->m_r2 = this->m_rv =0;

			trim_western_punctuation(text);

			hash_german_yu(text, "aeiouüyäöAÄEIOÖUÜY");

			find_r1(text, "aeiouüyäöAÄEIOÖUÜY");
			if (this->m_r1 == text.length() )
				{
				remove_german_umlauts(text);
				unhash_german_yu(text);
				return;
				}
			find_r2(text, "aeiouüyäöAÄEIOÖUÜY");
			//R1 must have at least 3 characters in front of it
			if (this->m_r1 < 3)
				{
				this->m_r1 = 3;
				}

			//step 1:
			step_1(text);
			//step 2:
			step_2(text);
			//step 3:
			step_3(text);

			remove_german_umlauts(text);
			unhash_german_yu(text);
			}
	private:
		//---------------------------------------------
		void step_1(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (delete_if_is_in_r1(text, "ern", 3) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "er", 2) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "es", 2) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "en", 2) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "em", 2) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "e", 1) )
				{
				return;
				}
			else if (is_suffix_in_r1(text,"s", 1) )
				{
				if (string_util::is_one_of(text[text.length()-2], "bdfghklmnrtBDFGHKLMNRT") )
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
			if (delete_if_is_in_r1(text, "est", 3) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "er", 2) )
				{
				return;
				}
			else if (delete_if_is_in_r1(text, "en", 2) )
				{
				return;
				}
			else if (text.length() >= 6 &&
					is_suffix_in_r1(text, "st", 2) )
				{
				if (string_util::is_one_of(text[text.length()-3], "bdfghklmntBDFGHKLMNT") )
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					}
				return;
				}
			}

		//---------------------------------------------
		void step_3(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (delete_if_is_in_r2(text, "heit", 4) ||
				delete_if_is_in_r2(text, "lich", 4) )
				{
				if (delete_if_is_in_r1(text, "er", 2) ||
					delete_if_is_in_r1(text, "en", 2) )
					{
					return;
					}
				return;
				}
			else if (delete_if_is_in_r2(text, "keit", 4) )
				{
				if (delete_if_is_in_r2(text, "lich", 4) ||
					delete_if_is_in_r2(text, "ig", 2) )
					{
					return;
					}
				return;
				}
			else if (is_suffix(text,"isch", 4) )
				{
				if (this->m_r2 <= static_cast<int>(text.length()-4) &&
					!(text[text.length()-5] == 'e' || text[text.length()-5] == 'E'))
					{
					text.erase(text.end()-4, text.end() );
					update_r_sections(text);
					}
				}
			else if (delete_if_is_in_r2(text, "end", 3) )
				{
				if (text.length() >= 3 &&
					is_suffix_in_r2(text, "ig", 2)  &&
					!(text[text.length()-3] == 'e' || text[text.length()-3] == 'E'))
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (delete_if_is_in_r2(text, "ung", 3) )
				{
				if (text.length() >= 3 &&
					is_suffix_in_r2(text, "ig", 2)  &&
					!(text[text.length()-3] == 'e' || text[text.length()-3] == 'E'))
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (is_suffix(text,"ig", 2) )
				{
				if (this->m_r2 <= static_cast<int>(text.length()-2) &&
					!(text[static_cast<int>(text.length()-3)] == 'e' || text[static_cast<int>(text.length()-3)] == 'E'))
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					}
				}
			else if (is_suffix(text,"ik", 2) )
				{
				if (this->m_r2 <= static_cast<int>(text.length()-2) &&
					!(text[static_cast<int>(text.length()-3)] == 'e' || text[static_cast<int>(text.length()-3)] == 'E'))
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					}
				}
			}
		};
	}

#endif //__GERMAN_STEM_H__