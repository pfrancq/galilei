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

			find_r1(text, "aeiouyäöAEIOUYÄÖ");
			find_r2(text, "aeiouyäöAEIOUYÄÖ");

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
				is_suffix_in_r1(text, "kään", 4) )
				{
				if (text.length() >= 5 &&
					string_util::is_one_of(text[text.length()-5], "ntaeiouyäöAEIOUYÄÖ") )
					{
					text.erase(text.end()-4, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (is_suffix_in_r1(text, "kin", 3) ||
				is_suffix_in_r1(text, "han", 3) ||
				is_suffix_in_r1(text, "hän", 3) )
				{
				if (text.length() >= 4 &&
					string_util::is_one_of(text[text.length()-4], "ntaeiouyäöAEIOUYÄÖ") )
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
				is_suffix_in_r1(text, "kö", 2) ||
				is_suffix_in_r1(text, "pa", 2) ||
				is_suffix_in_r1(text, "pä", 2) )
				{
				if (text.length() >= 3 &&
					string_util::is_one_of(text[text.length()-3], "ntaeiouyäöAEIOUYÄÖ") )
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
				delete_if_is_in_r1(text, "nsä", 3, false) ||
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
			else if (is_suffix_in_r1(text, "än", 2) )
				{
				if ((text.length() >= 4 &&
					(text.compare(text.length()-4, 2, "tä", 2) == 0 ||
					text.compare(text.length()-4, 2, "nä", 2) == 0) ) ||
					(text.length() >= 5 &&
					(text.compare(text.length()-5, 3, "ssä", 3) == 0 ||
					text.compare(text.length()-5, 3, "stä", 3) == 0 ||
					text.compare(text.length()-5, 3, "llä", 3) == 0 ||
					text.compare(text.length()-5, 3, "ltä", 3) == 0) ) )
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
				string_util::is_one_of(text[text.length()-5], "aeiouäöAEIOUÄÖ") &&
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
					string_util::is_one_of(text[text.length()-6], "aeiouäöAEIOUÄÖ") )
				{
				text.erase(text.end()-4, text.end() );
				update_r_sections(text);
				m_step_3_successful = true;
				return;
				}
			//suffix followed by Vi
			else if (text[text.length()-4] == 'i' &&
					is_suffix_in_r1(text, "den", 3) &&
					string_util::is_one_of(text[text.length()-5], "aeiouäöAEIOUÄÖ") )
				{
				text.erase(text.end()-3, text.end() );
				update_r_sections(text);
				m_step_3_successful = true;
				return;
				}
			else if ((is_suffix_in_r1(text, "tta", 3) ||
				is_suffix_in_r1(text, "ttä", 3)) &&
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
				is_suffix_in_r1(text, "hän", 3) ||
				is_suffix_in_r1(text, "hön", 3)) )
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
				delete_if_is_in_r1(text, "ssä", 3, false) ||
				delete_if_is_in_r1(text, "sta", 3, false) ||
				delete_if_is_in_r1(text, "stä", 3, false) ||
				delete_if_is_in_r1(text, "lla", 3, false) ||
				delete_if_is_in_r1(text, "llä", 3, false) ||
				delete_if_is_in_r1(text, "lta", 3, false) ||
				delete_if_is_in_r1(text, "ltä", 3, false) ||
				delete_if_is_in_r1(text, "lle", 3, false) ||
				delete_if_is_in_r1(text, "ksi", 3, false) ||
				delete_if_is_in_r1(text, "ine", 3, false) ||
				delete_if_is_in_r1(text, "na", 2, false) ||
				delete_if_is_in_r1(text, "nä", 2, false) )
				{
				m_step_3_successful = true;
				return;
				}
			else if (delete_if_is_in_r1(text, "ta", 2, false) ||
				delete_if_is_in_r1(text, "tä", 2, false) )
				{
				m_step_3_successful = true;
				return;
				}
			//suffix followed by cv
			else if ((is_suffix_in_r1(text, "a", 1) || is_suffix_in_r1(text, "ä", 1) )&&
					!string_util::is_one_of(text[text.length()-3], "aeiouyäöAEIOUYÄÖ") &&
					string_util::is_one_of(text[text.length()-2], "aeiouyäöAEIOUYÄÖ") )
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
				if ((string_util::is_one_of(text[text.length()-1], "aeiouäöAEIOUÄÖ") &&
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
				delete_if_is_in_r2(text, "impä", 4, false) ||
				delete_if_is_in_r2(text, "immi", 4, false) ||
				delete_if_is_in_r2(text, "imma", 4, false) ||
				delete_if_is_in_r2(text, "immä", 4, false) ||
				delete_if_is_in_r2(text, "eja", 3, false) ||
				delete_if_is_in_r2(text, "ejä", 3, false) )
				{
				return;
				}
			else if (text.length() >= 5 &&
					(is_suffix_in_r2(text, "mpi", 3) ||
					is_suffix_in_r2(text, "mpa", 3) ||
					is_suffix_in_r2(text, "mpä", 3) ||
					is_suffix_in_r2(text, "mmi", 3) ||
					is_suffix_in_r2(text, "mma", 3) ||
					is_suffix_in_r2(text, "mmä", 3) ) )
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
				if (string_util::is_one_of(text[text.length()-2], "aeiouyäöAEIOUYÄÖ") )
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
				string_util::is_one_of(text[text.length()-1], "aeiouäöAEIOUÄÖ") &&
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
				!string_util::is_one_of(text[text.length()-2], "aeiouyäöAEIOUYÄÖ") &&
				string_util::is_one_of(text[text.length()-1], "aeiäAEIÄ") )
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
			size_t index = text.find_last_not_of("aeiouyäöAEIOUYÄÖ");
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
