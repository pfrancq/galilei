/***************************************************************************
                          finnish_stem.h  -  description
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

#ifndef __FINNISH_STEM_H__
#define __FINNISH_STEM_H__

#include "stemming.h"

namespace stemming
	{
	//------------------------------------------------------
	template<typename Tchar_type = char,
			typename Tchar_traits = std::char_traits<Tchar_type> >
	class finnish_stem : stem<Tchar_type,Tchar_traits>
		{
	public:
		finnish_stem() : m_step_3_successful(false) {}
				//---------------------------------------------
		void operator()(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (text.length() < 2)
				{
				return;
				}

			//reset internal data
			m_step_3_successful = false;
			this->m_r1 = this->m_r2 = this->m_rv =0;

			trim_western_punctuation(text);

			find_r1(text, "aeiouy��AEIOUY��");
			find_r2(text, "aeiouy��AEIOUY��");

			step_1(text);
			step_2(text);
			step_3(text);
			step_4(text);
			step_5(text);
			step_6(text);
			}
	private:
		//---------------------------------------------
		void step_1(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (is_suffix_in_r1(text, "kaan", 4) ||
				is_suffix_in_r1(text, "k��n", 4) )
				{
				if (text.length() >= 5 &&
					string_util::is_one_of(text[text.length()-5], "ntaeiouy��AEIOUY��") )
					{
					text.erase(text.end()-4, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (is_suffix_in_r1(text, "kin", 3) ||
				is_suffix_in_r1(text, "han", 3) ||
				is_suffix_in_r1(text, "h�n", 3) )
				{
				if (text.length() >= 4 &&
					string_util::is_one_of(text[text.length()-4], "ntaeiouy��AEIOUY��") )
					{
					text.erase(text.end()-3, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (is_suffix_in_r1(text, "sti", 3) )
				{
				delete_if_is_in_r2(text, "sti", 3);
				return;
				}
			else if (is_suffix_in_r1(text, "ko", 2) ||
				is_suffix_in_r1(text, "k�", 2) ||
				is_suffix_in_r1(text, "pa", 2) ||
				is_suffix_in_r1(text, "p�", 2) )
				{
				if (text.length() >= 3 &&
					string_util::is_one_of(text[text.length()-3], "ntaeiouy��AEIOUY��") )
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					}
				return;
				}
			}
		//---------------------------------------------
		void step_2(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (delete_if_is_in_r1(text, "nsa", 3, false) ||
				delete_if_is_in_r1(text, "ns�", 3, false) ||
				delete_if_is_in_r1(text, "mme", 3, false) ||
				delete_if_is_in_r1(text, "nne" ,3, false) )
				{
				return;
				}
			else if (is_suffix_in_r1(text, "si", 2) )
				{
				if (text.length() >= 3 &&
					!(text[text.length()-3] == 'k' || text[text.length()-3] == 'K'))
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (delete_if_is_in_r1(text, "ni", 2, false) )
				{
				if (is_suffix(text, "kse", 3) )
					{
					text[text.length()-1] = 'i';
					}
				return;
				}
			else if (is_suffix_in_r1(text, "an", 2) )
				{
				if ((text.length() >= 4 &&
					(text.compare(text.length()-4, 2, "ta", 2) == 0 ||
					text.compare(text.length()-4, 2, "na", 2) == 0) ) ||
					(text.length() >= 5 &&
					(text.compare(text.length()-5, 3, "ssa", 3) == 0 ||
					text.compare(text.length()-5, 3, "sta", 3) == 0 ||
					text.compare(text.length()-5, 3, "lla", 3) == 0 ||
					text.compare(text.length()-5, 3, "lta", 3) == 0) ) )
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (is_suffix_in_r1(text, "�n", 2) )
				{
				if ((text.length() >= 4 &&
					(text.compare(text.length()-4, 2, "t�", 2) == 0 ||
					text.compare(text.length()-4, 2, "n�", 2) == 0) ) ||
					(text.length() >= 5 &&
					(text.compare(text.length()-5, 3, "ss�", 3) == 0 ||
					text.compare(text.length()-5, 3, "st�", 3) == 0 ||
					text.compare(text.length()-5, 3, "ll�", 3) == 0 ||
					text.compare(text.length()-5, 3, "lt�", 3) == 0) ) )
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (is_suffix_in_r1(text, "en", 2) )
				{
				if (text.length() >= 5 &&
					(text.compare(text.length()-5, 3, "lle", 3) == 0 ||
					text.compare(text.length()-5, 3, "ine", 3) == 0 ) )
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
			//seen followed by LV
			if (is_suffix_in_r1(text, "seen", 4) &&
				string_util::is_one_of(text[text.length()-5], "aeiou��AEIOU��") &&
				text[text.length()-5] == text[text.length()-6])
				{
				text.erase(text.end()-4, text.end() );
				update_r_sections(text);
				m_step_3_successful = true;
				return;
				}
			//suffix followed by Vi
			else if (text[text.length()-5] == 'i' &&
					(is_suffix_in_r1(text, "siin", 4) ||
					is_suffix_in_r1(text, "tten", 4) ) &&
					string_util::is_one_of(text[text.length()-6], "aeiou��AEIOU��") )
				{
				text.erase(text.end()-4, text.end() );
				update_r_sections(text);
				m_step_3_successful = true;
				return;
				}
			//suffix followed by Vi
			else if (text[text.length()-4] == 'i' &&
					is_suffix_in_r1(text, "den", 3) &&
					string_util::is_one_of(text[text.length()-5], "aeiou��AEIOU��") )
				{
				text.erase(text.end()-3, text.end() );
				update_r_sections(text);
				m_step_3_successful = true;
				return;
				}
			else if ((is_suffix_in_r1(text, "tta", 3) ||
				is_suffix_in_r1(text, "tt�", 3)) &&
				text[text.length()-4] == 'e')
				{
				text.erase(text.end()-3, text.end() );
				update_r_sections(text);
				m_step_3_successful = true;
				return;
				}
			//ends if VHVN
			else if (
				(is_suffix_in_r1(text, "han", 3) ||
				is_suffix_in_r1(text, "hen", 3) ||
				is_suffix_in_r1(text, "hin", 3) ||
				is_suffix_in_r1(text, "hon", 3) ||
				is_suffix_in_r1(text, "h�n", 3) ||
				is_suffix_in_r1(text, "h�n", 3)) )
				{
				if (text[text.length()-2] == text[text.length()-4])
					{
					text.erase(text.end()-3, text.end() );
					update_r_sections(text);
					m_step_3_successful = true;
					}
				return;
				}
			else if (delete_if_is_in_r1(text, "ssa", 3, false) ||
				delete_if_is_in_r1(text, "ss�", 3, false) ||
				delete_if_is_in_r1(text, "sta", 3, false) ||
				delete_if_is_in_r1(text, "st�", 3, false) ||
				delete_if_is_in_r1(text, "lla", 3, false) ||
				delete_if_is_in_r1(text, "ll�", 3, false) ||
				delete_if_is_in_r1(text, "lta", 3, false) ||
				delete_if_is_in_r1(text, "lt�", 3, false) ||
				delete_if_is_in_r1(text, "lle", 3, false) ||
				delete_if_is_in_r1(text, "ksi", 3, false) ||
				delete_if_is_in_r1(text, "ine", 3, false) ||
				delete_if_is_in_r1(text, "na", 2, false) ||
				delete_if_is_in_r1(text, "n�", 2, false) )
				{
				m_step_3_successful = true;
				return;
				}
			else if (delete_if_is_in_r1(text, "ta", 2, false) ||
				delete_if_is_in_r1(text, "t�", 2, false) )
				{
				m_step_3_successful = true;
				return;
				}
			//suffix followed by cv
			else if ((is_suffix_in_r1(text, "a", 1) || is_suffix_in_r1(text, "�", 1) )&&
					!string_util::is_one_of(text[text.length()-3], "aeiouy��AEIOUY��") &&
					string_util::is_one_of(text[text.length()-2], "aeiouy��AEIOUY��") )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				m_step_3_successful = true;
				return;
				}	
			//suffix followed by LV or ie
			else if (is_suffix_in_r1(text, "n", 1) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				if ((string_util::is_one_of(text[text.length()-1], "aeiou��AEIOU��") &&
					text[text.length()-1] == text[text.length()-2]) ||
					is_suffix_in_r1(text, "ie", 2) ) 
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				m_step_3_successful = true;
				return;
				}
			}
		//---------------------------------------------
		void step_4(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (delete_if_is_in_r2(text, "impi", 4, false) ||
				delete_if_is_in_r2(text, "impa", 4, false) ||
				delete_if_is_in_r2(text, "imp�", 4, false) ||
				delete_if_is_in_r2(text, "immi", 4, false) ||
				delete_if_is_in_r2(text, "imma", 4, false) ||
				delete_if_is_in_r2(text, "imm�", 4, false) ||
				delete_if_is_in_r2(text, "eja", 3, false) ||
				delete_if_is_in_r2(text, "ej�", 3, false) )
				{
				return;
				}
			else if (text.length() >= 5 &&
					(is_suffix_in_r2(text, "mpi", 3) ||
					is_suffix_in_r2(text, "mpa", 3) ||
					is_suffix_in_r2(text, "mp�", 3) ||
					is_suffix_in_r2(text, "mmi", 3) ||
					is_suffix_in_r2(text, "mma", 3) ||
					is_suffix_in_r2(text, "mm�", 3) ) )
				{
				if (text.compare(text.length()-5, 2, "po") != 0)
					{
					text.erase(text.end()-3, text.end() );
					update_r_sections(text);
					}
				}
			return;
			}
		//---------------------------------------------
		void step_5(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			//if step 3 was successful is removing a suffix
			if (m_step_3_successful)
				{
				if (delete_if_is_in_r1(text, "i", 1) ||
					delete_if_is_in_r1(text, "j", 1) )
					{
					//NOOP
					}
				}
			else
				{
				if (string_util::is_one_of(text[text.length()-2], "aeiouy��AEIOUY��") )
					{
					if (delete_if_is_in_r1(text, "t", 1) )
						{
						if (!delete_if_is_in_r2(text, "imma", 4) )
							{
							if (is_suffix_in_r2(text, "mma", 3) &&
								text.compare(text.length()-5, 2, "po") != 0)
								{
								text.erase(text.end()-3, text.end() );
								update_r_sections(text);
								}
							}
						}
					}
				}
			}
		//---------------------------------------------
		void step_6(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			step_6a(text);
			step_6b(text);
			step_6c(text);
			step_6d(text);
			step_6e(text);
			}
		//---------------------------------------------
		void step_6a(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (this->m_r1 <= text.length()-2 &&
				string_util::is_one_of(text[text.length()-1], "aeiou��AEIOU��") &&
				text[text.length()-1] == text[text.length()-2])
				{
				text.erase(text.end()-1);
				update_r_sections(text);
				}
			}
		//---------------------------------------------
		void step_6b(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (this->m_r1 <= text.length()-2 &&
				!string_util::is_one_of(text[text.length()-2], "aeiouy��AEIOUY��") &&
				string_util::is_one_of(text[text.length()-1], "aei�AEI�") )
				{
				text.erase(text.end()-1);
				update_r_sections(text);
				}
			}
		//---------------------------------------------
		void step_6c(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (is_suffix_in_r1(text, "oj", 2) ||
				is_suffix_in_r1(text, "uj", 2) )
				{
				text.erase(text.end()-1);
				update_r_sections(text);
				}
			}
		//---------------------------------------------
		void step_6d(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (is_suffix_in_r1(text, "jo", 2) )
				{
				text.erase(text.end()-1);
				update_r_sections(text);
				}
			}
		//---------------------------------------------
		void step_6e(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			//find the last consanant
			size_t index = text.find_last_not_of("aeiouy��AEIOUY��");
			if (index == std::basic_string<Tchar_type, Tchar_traits>::npos ||
				index < 1)
				{
				return;
				}
			if (text[index] == text[index-1])
				{
				text.erase(text.begin()+(index) );
				update_r_sections(text);
				}
			}
		//internal data specific to Finnish stemmer
		bool m_step_3_successful;
		};
	}

#endif //__FINNISH_STEM_H__
