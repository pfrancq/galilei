/***************************************************************************
                          spanish_stem.h  -  description
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

#ifndef __SPANISH_STEM_H__
#define __SPANISH_STEM_H__

#include "stemming.h"

namespace stemming
	{
	//------------------------------------------------------
	template<typename Tchar_type = char,
			typename Tchar_traits = std::char_traits<Tchar_type> >
	class spanish_stem : stem<Tchar_type,Tchar_traits>
		{
	public:
		//---------------------------------------------
		void operator()(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (text.length() < 3)
				{
				remove_spanish_acutes(text);
				return;
				}

			//reset internal data
			this->m_r1 = this->m_r2 = this->m_rv =0;

			trim_western_punctuation(text);

			find_r1(text, "aeiouáéíóúüAEIOUÁÉÍÓÚÜ");
			find_r2(text, "aeiouáéíóúüAEIOUÁÉÍÓÚÜ");
			find_spanish_rv(text, "aeiouáéíóúüAEIOUÁÉÍÓÚÜ");

			step_0(text);
			step_1(text);
			//steps 2a and 2b and only called from step1
			step_3(text);

			remove_spanish_acutes(text);
			}
	private:
		//---------------------------------------------
		void step_0(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (is_suffix_in_rv(text, "selos", 5))
				{
				step_0a(text, 5);
				step_0b(text, 5);
				step_0c(text, 5);
				return;
				}
			else if (is_suffix_in_rv(text, "selas", 5))
				{
				step_0a(text, 5);
				step_0b(text, 5);
				step_0c(text, 5);
				return;
				}
			else if (is_suffix_in_rv(text, "sela", 4))
				{
				step_0a(text, 4);
				step_0b(text, 4);
				step_0c(text, 4);
				return;
				}
			else if (is_suffix_in_rv(text, "selo", 4))
				{
				step_0a(text, 4);
				step_0b(text, 4);
				step_0c(text, 4);
				return;
				}
			else if (is_suffix_in_rv(text, "las", 3))
				{
				step_0a(text, 3);
				step_0b(text, 3);
				step_0c(text, 3);
				return;
				}
			else if (is_suffix_in_rv(text, "les", 3))
				{
				step_0a(text, 3);
				step_0b(text, 3);
				step_0c(text, 3);
				return;
				}
			else if (is_suffix_in_rv(text, "los", 3))
				{
				step_0a(text, 3);
				step_0b(text, 3);
				step_0c(text, 3);
				return;
				}
			else if (is_suffix_in_rv(text, "nos", 3))
				{
				step_0a(text, 3);
				step_0b(text, 3);
				step_0c(text, 3);
				return;
				}
			else if (is_suffix_in_rv(text, "la", 2))
				{
				step_0a(text, 2);
				step_0b(text, 2);
				step_0c(text, 2);
				return;
				}
			else if (is_suffix_in_rv(text, "le", 2))
				{
				step_0a(text, 2);
				step_0b(text, 2);
				step_0c(text, 2);
				return;
				}
			else if (is_suffix_in_rv(text, "lo", 2))
				{
				step_0a(text, 2);
				step_0b(text, 2);
				step_0c(text, 2);
				return;
				}
			else if (is_suffix_in_rv(text, "me", 2))
				{
				step_0a(text, 2);
				step_0b(text, 2);
				step_0c(text, 2);
				return;
				}
			else if (is_suffix_in_rv(text, "se", 2))
				{
				step_0a(text, 2);
				step_0b(text, 2);
				step_0c(text, 2);
				return;
				}
			}
///@todo should these be returning a boolean? see italian stemmer...
		//---------------------------------------------
		void step_0a(std::basic_string<Tchar_type, Tchar_traits>& text, size_t suffix_length)
			{
			if ((text.length() >= suffix_length + 5) &&
				this->m_rv <= static_cast<int>(text.length()-5-suffix_length) &&
				text.compare(static_cast<int>(text.length()-5-suffix_length), 5, "iéndo", 5) == 0)
				{
				text.erase(text.end()-suffix_length, text.end() );
				text[text.length()-4] = 'e';
				update_r_sections(text);
				}
			else if ((text.length() >= suffix_length + 4) &&
				this->m_rv <= static_cast<int>(text.length()-4-suffix_length) &&
				text.compare(static_cast<int>(text.length()-4-suffix_length), 4, "ándo", 4) == 0)
				{
				text.erase(text.end()-suffix_length, text.end() );
				text[text.length()-4] = 'a';
				update_r_sections(text);
				}
			else if ((text.length() >= suffix_length + 2) &&
				this->m_rv <= static_cast<int>(text.length()-2-suffix_length) &&
				text.compare(static_cast<int>(text.length()-2-suffix_length), 2, "ár", 2) == 0)
				{
				text.erase(text.end()-suffix_length, text.end() );
				text[text.length()-2] = 'a';
				update_r_sections(text);
				}
			else if ((text.length() >= suffix_length + 2) &&
				this->m_rv <= static_cast<int>(text.length()-2-suffix_length) &&
				text.compare(static_cast<int>(text.length()-2-suffix_length), 2, "ér", 2) == 0)
				{
				text.erase(text.end()-suffix_length, text.end() );
				text[text.length()-2] = 'e';
				update_r_sections(text);
				}
			else if ((text.length() >= suffix_length + 2) &&
				this->m_rv <= static_cast<int>(text.length()-2-suffix_length) &&
				text.compare(static_cast<int>(text.length()-2-suffix_length), 2, "ír", 2) == 0)
				{
				text.erase(text.end()-suffix_length, text.end() );
				text[text.length()-2] = 'i';
				update_r_sections(text);
				}
			}

		//---------------------------------------------
		void step_0b(std::basic_string<Tchar_type, Tchar_traits>& text, size_t suffix_length)
			{
			if ((text.length() >= suffix_length + 5) &&
				this->m_rv <= static_cast<int>(text.length()-5-suffix_length) &&
				text.compare(static_cast<int>(text.length()-5-suffix_length), 5, "iendo", 5) == 0)
				{
				text.erase(text.end()-suffix_length, text.end() );
				update_r_sections(text);
				}
			else if ((text.length() >= suffix_length + 4) &&
				this->m_rv <= static_cast<int>(text.length()-4-suffix_length) &&
				text.compare(static_cast<int>(text.length()-4-suffix_length), 4, "ando", 4) == 0)
				{
				text.erase(text.end()-suffix_length, text.end() );
				update_r_sections(text);
				}
			else if ((text.length() >= suffix_length + 2) &&
				this->m_rv <= static_cast<int>(text.length()-2-suffix_length) &&
				text.compare(static_cast<int>(text.length()-2-suffix_length), 2, "ar", 2) == 0)
				{
				text.erase(text.end()-suffix_length, text.end() );
				update_r_sections(text);
				}
			else if ((text.length() >= suffix_length + 2) &&
				this->m_rv <= static_cast<int>(text.length()-2-suffix_length) &&
				text.compare(static_cast<int>(text.length()-2-suffix_length), 2, "er", 2) == 0)
				{
				text.erase(text.end()-suffix_length, text.end() );
				update_r_sections(text);
				}
			else if ((text.length() >= suffix_length + 2) &&
				this->m_rv <= static_cast<int>(text.length()-2-suffix_length) &&
				text.compare(static_cast<int>(text.length()-2-suffix_length), 2, "ir", 2) == 0)
				{
				text.erase(text.end()-suffix_length, text.end() );
				update_r_sections(text);
				}
			}

		//---------------------------------------------
		void step_0c(std::basic_string<Tchar_type, Tchar_traits>& text, size_t suffix_length)
			{
			if ((text.length() >= suffix_length + 6) &&
				text.compare(text.length()-(suffix_length)-6, 6, "uyendo", 6) == 0 &&
				(this->m_rv <= static_cast<int>(text.length())-(suffix_length+5) ) )
				{
				text.erase(text.end()-suffix_length, text.end() );
				update_r_sections(text);
				}
			}

		//---------------------------------------------
		void step_1(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			size_t original_length = text.length();
			if (delete_if_is_in_r2(text, "imientos", 8) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "amientos", 8) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (is_suffix_in_r2(text, "uciones", 7) )
				{
				text.erase(text.end()-6, text.end() );
				update_r_sections(text);
				return;
				}
			else if (delete_if_is_in_r2(text, "amiento", 7) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "imiento", 7) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "aciones", 7) )
				{
				if (original_length > text.length() )
					{
					delete_if_is_in_r2(text, "ic", 2);
					return;
					}
				step_2a(text);
				}
			else if (is_suffix_in_r2(text, "logías", 6) )
				{
				text.erase(text.end()-3, text.end() );
				update_r_sections(text);
				return;
				}
			else if (is_suffix_in_r2(text, "encias", 6) )
				{
				text.erase(text.end()-2, text.end() );
				text[text.length()-2] = 't';
				text[text.length()-1] = 'e';
				update_r_sections(text);
				return;
				}
			else if (delete_if_is_in_r2(text, "idades", 6) )
				{
				if (original_length > text.length() )
					{
					if (delete_if_is_in_r2(text, "abil", 4) ||
						delete_if_is_in_r2(text, "ic", 2) ||
						delete_if_is_in_r2(text, "iv", 2) )
						{
						return;
						}
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r1(text, "amente", 6) )
				{
				if (original_length > text.length() )
					{
					if (delete_if_is_in_r2(text, "iv", 2) )
						{
						delete_if_is_in_r2(text, "at", 2);
						return;
						}
					else
						{
						if (delete_if_is_in_r2(text, "os", 2) ||
							delete_if_is_in_r2(text, "ic", 2) ||
							delete_if_is_in_r2(text, "ad", 2) )
							{
							return;
							}
						}
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "adores", 6) )
				{
				if (original_length > text.length() )
					{
					delete_if_is_in_r2(text, "ic", 2);
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "adoras", 6) )
				{
				if (original_length > text.length() )
					{
					delete_if_is_in_r2(text, "ic", 2);
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "adora", 5) )
				{
				if (original_length > text.length() )
					{
					delete_if_is_in_r2(text, "ic", 2);
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ación", 5) )
				{
				if (original_length > text.length() )
					{
					delete_if_is_in_r2(text, "ic", 2);
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ibles", 5) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "istas", 5) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ables", 5) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ismos", 5) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "anzas", 5) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (is_suffix_in_r2(text, "logía", 5) )
				{
				text.erase(text.end()-2, text.end() );
				update_r_sections(text);
				return;
				}
			else if (is_suffix_in_r2(text, "ución", 5) )
				{
				text.erase(text.end()-4, text.end() );
				update_r_sections(text);
				return;
				}
			else if (is_suffix_in_r2(text, "encia", 5) )
				{
				text.erase(text.end()-1, text.end() );
				text[text.length()-2] = 't';
				text[text.length()-1] = 'e';
				update_r_sections(text);
				return;
				}
			else if (delete_if_is_in_r2(text, "mente", 5) )
				{
				if (original_length > text.length() )
					{
					if (delete_if_is_in_r2(text, "able", 4) ||
						delete_if_is_in_r2(text, "ible", 4) )
						{
						return;
						}
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "anza", 4) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "icos", 4) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "icas", 4) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ismo", 4) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "able", 4) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ible", 4) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ista", 4) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "osos", 4) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "osas", 4) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ivas", 4) )
				{
				if (original_length > text.length() )
					{
					delete_if_is_in_r2(text, "at", 2);
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ivos", 4) )
				{
				if (original_length > text.length() )
					{
					delete_if_is_in_r2(text, "at", 2);
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ador", 4) )
				{
				if (original_length > text.length() )
					{
					delete_if_is_in_r2(text, "ic", 2);
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "idad", 4) )
				{
				if (original_length > text.length() )
					{
					if (delete_if_is_in_r2(text, "abil", 4) ||
						delete_if_is_in_r2(text, "ic", 2) ||
						delete_if_is_in_r2(text, "iv", 2) )
						{
						return;
						}
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ico", 3) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ica", 3) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "oso", 3) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "osa", 3) )
				{
				if (original_length > text.length() )
					{
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "iva", 3) )
				{
				if (original_length > text.length() )
					{
					delete_if_is_in_r2(text, "at", 2);
					return;
					}
				step_2a(text);
				}
			else if (delete_if_is_in_r2(text, "ivo", 3) )
				{
				if (original_length > text.length() )
					{
					delete_if_is_in_r2(text, "at", 2);
					return;
					}
				step_2a(text);
				}
			//this should only be called from here if nothing was removed in step 1
			if (text.length() == original_length)
				{
				step_2a(text);
				}
			}

		//---------------------------------------------
		void step_2a(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			size_t original_length = text.length();
			if (is_suffix_in_rv(text, "yeron", 5))
				{
				if (text[text.length()-6] == 'u' || text[text.length()-6] == 'U')
					{
					text.erase(text.end()-5, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "yendo", 5))
				{
				if (text[text.length()-6] == 'u' || text[text.length()-6] == 'U')
						{
					text.erase(text.end()-5, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "yamos", 5))
				{
				if (text[text.length()-6] == 'u' || text[text.length()-6] == 'U')
					{
					text.erase(text.end()-5, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "yais", 4))
				{
				if (text[text.length()-5] == 'u' || text[text.length()-5] == 'U')
					{
					text.erase(text.end()-4, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "yan", 3))
				{
				if (text[text.length()-4] == 'u' || text[text.length()-4] == 'U')
					{
					text.erase(text.end()-3, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "yen", 3))
				{
				if (text[text.length()-4] == 'u' || text[text.length()-4] == 'U')
					{
					text.erase(text.end()-3, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "yas", 3))
				{
				if (text[text.length()-4] == 'u' || text[text.length()-4] == 'U')
					{
					text.erase(text.end()-3, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "yes", 3))
				{
				if (text[text.length()-4] == 'u' || text[text.length()-4] == 'U')
					{
					text.erase(text.end()-3, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "ya", 2))
				{
				if (text[text.length()-3] == 'u' || text[text.length()-3] == 'U')
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "ye", 2))
				{
				if (text[text.length()-3] == 'u' || text[text.length()-3] == 'U')
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "yo", 2))
				{
				if (text[text.length()-3] == 'u' || text[text.length()-3] == 'U')
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			else if (is_suffix_in_rv(text, "yó", 2))
				{
				if (text[text.length()-3] == 'u' || text[text.length()-3] == 'U')
					{
					text.erase(text.end()-2, text.end() );
					update_r_sections(text);
					return;
					}
				step_2b(text);
				}
			//only called if 2a fails to remove a suffix
			if (text.length() == original_length)
				{
				step_2b(text);
				}
			}
		//---------------------------------------------
		void step_2b(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (delete_if_is_in_rv(text, "aríamos", 7, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "eríamos", 7, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iríamos", 7, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iéramos", 7, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iésemos", 7, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aríais", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aremos", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "eríais", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "eremos", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iríais", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iremos", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ierais", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ieseis", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "asteis", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "isteis", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ábamos", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "áramos", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ásemos", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "arían", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "arías", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aréis", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erían", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erías", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "eréis", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irían", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irías", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iréis", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ieran", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iesen", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ieron", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iendo", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ieras", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ieses", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "abais", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "arais", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aseis", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "íamos", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "emos", 4, false) )
				{
				if (is_suffix(text, "gu", 2) )
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (delete_if_is_in_rv(text, "arán", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "arás", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aría", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erán", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erás", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ería", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irán", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irás", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iría", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iera", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iese", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aste", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iste", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aban", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aran", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "asen", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aron", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ando", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "abas", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "adas", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "idas", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aras", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ases", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "íais", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ados", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "idos", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "amos", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "imos", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ará", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aré", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erá", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "eré", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irá", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iré", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "aba", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ada", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ida", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ara", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ase", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ían", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ado", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ido", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ías", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "áis", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "éis", 3, false) )
				{
				if (is_suffix(text, "gu", 2) )
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (delete_if_is_in_rv(text, "ía", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ad", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ed", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "id", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "an", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ió", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ar", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "er", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ir", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "as", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ís", 2, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "en", 2, false) )
				{
				if (is_suffix(text, "gu", 2) )
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (delete_if_is_in_rv(text, "es", 2, false) )
				{
				if (is_suffix(text, "gu", 2) )
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				return;
				}
			}
		//---------------------------------------------
		void step_3(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (delete_if_is_in_rv(text, "os", 2) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "a", 1) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "o", 1) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "á", 1) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "í", 1) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ó", 1) )
				{
				return;
				}

			else if (delete_if_is_in_rv(text, "é", 1) ||
					delete_if_is_in_rv(text, "e", 1) )
				{
				if (is_suffix_in_rv(text, "u", 1))
					{
					if (text[text.length()-2] == 'g' || text[text.length()-2] == 'G')
						{
						text.erase(text.end()-1, text.end() );
						}
					}
				return;
				}
			}
		};
	}

#endif //__SPANISH_STEM_H__
