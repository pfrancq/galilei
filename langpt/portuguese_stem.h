/***************************************************************************
                          portuguese_stem.h  -  description
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

#ifndef __PORTUGUESE_STEM_H__
#define __PORTUGUESE_STEM_H__

#include "stemming.h"

namespace stemming
	{
	//------------------------------------------------------
	template<typename Tchar_type = char,
			typename Tchar_traits = std::char_traits<Tchar_type> >
	class portuguese_stem : stem<Tchar_type,Tchar_traits>
		{
	public:
		portuguese_stem() : m_step1_step2_altered(false), m_altered_suffix_index(0)
			{}
		//---------------------------------------------
		void operator()(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (text.length() < 3)
				{
				return;
				}
			trim_western_punctuation(text);

			//reset internal data
			m_altered_suffix_index = 0;
			m_step1_step2_altered = false;
			m_r1 = m_r2 = m_rv =0;

			string_util::replace_all(text, "ã", "a~");
			string_util::replace_all(text, "Ã", "A~");
			string_util::replace_all(text, "õ", "o~");
			string_util::replace_all(text, "Õ", "O~");

			find_r1(text, "aeiouáéíóúâêôEIOUÁÉÍÓÚÂÊÔ");
			find_r2(text, "aeiouáéíóúâêôEIOUÁÉÍÓÚÂÊÔ");
			find_spanish_rv(text, "aeiouáéíóúâêôEIOUÁÉÍÓÚÂÊÔ");

			step_1(text);
			//intermediate steps handled by step 1
			if (!m_step1_step2_altered)
				{
				step_4(text);
				}
			step_5(text);

			string_util::replace_all(text, "a~", "ã");
			string_util::replace_all(text, "A~", "Ã");
			string_util::replace_all(text, "o~", "õ");
			string_util::replace_all(text, "O~", "Õ");
			}
	private:
		//---------------------------------------------
		void step_1(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			size_t original_length = text.length();
			if (is_suffix_in_r2(text, "uciones", 7) )
				{
				text.erase(text.end()-6, text.end() );
				m_altered_suffix_index = text.length()-1;
				update_r_sections(text);
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "amentos", 7) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "imentos", 7) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "amento", 6) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "imento", 6) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "adoras", 6) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "adores", 6) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "aço~es", 6) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (is_suffix_in_r2(text, "ências", 6) )
				{
				text.replace(text.end()-6, text.end(), "ente");
				m_altered_suffix_index = text.length()-4;
				update_r_sections(text);
				//NOOP (fall through to branching statement)
				}
			else if (is_suffix_in_r2(text, "logías", 6) )
				{
				text.erase(text.end()-3, text.end() );
				m_altered_suffix_index = text.length()-3;
				update_r_sections(text);
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r1(text, "amente", 6) )
				{
				if (delete_if_is_in_r2(text, "iv", 2, false) )
					{
					delete_if_is_in_r2(text, "at", 2, false);
					}
				else
					{
					if (delete_if_is_in_r2(text, "os", 2) ||
						delete_if_is_in_r2(text, "ic", 2) ||
						delete_if_is_in_r2(text, "ad", 2) )
						{
						//NOOP (fall through to branching statement)
						}
					}
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "idades", 6) )
				{
				if (delete_if_is_in_r2(text, "abil", 4) ||
					delete_if_is_in_r2(text, "ic", 2) ||
					delete_if_is_in_r2(text, "iv", 2) )
					{
					//NOOP (fall through to branching statement)
					}
				//NOOP (fall through to branching statement)
				}			
			else if (is_suffix_in_r2(text, "logía", 5) )
				{
				text.erase(text.end()-2, text.end() );
				m_altered_suffix_index = text.length()-3;
				update_r_sections(text);
				//NOOP (fall through to branching statement)
				}			
			else if (is_suffix_in_r2(text, "ución", 5) )
				{
				text.erase(text.end()-4, text.end() );
				m_altered_suffix_index = text.length()-1;
				update_r_sections(text);
				//NOOP (fall through to branching statement)
				}
			else if (is_suffix_in_r2(text, "ência", 5) )
				{
				text.replace(text.end()-5, text.end(), "ente");
				m_altered_suffix_index = text.length()-4;
				update_r_sections(text);
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "mente", 5) )
				{
				if (delete_if_is_in_r2(text, "avel", 4) ||
					delete_if_is_in_r2(text, "ivel", 4) )
					{
					//NOOP (fall through to branching statement)
					}
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "idade", 5) )
				{
				if (delete_if_is_in_r2(text, "abil", 4) ||
					delete_if_is_in_r2(text, "ic", 2) ||
					delete_if_is_in_r2(text, "iv", 2) )
					{
					//NOOP (fall through to branching statement)
					}
				//NOOP (fall through to branching statement)
				}
			else if (is_suffix(text, "eiras", 5) )
				{
				if (m_rv <= static_cast<int>(text.length()-4) )
					{
					text.erase(text.end()-2, text.end() );
					m_altered_suffix_index = text.length()-3;
					update_r_sections(text);
					}
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ismos", 5) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "istas", 5) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "adora", 5) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "aça~o", 5) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ezas", 4) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "icos", 4) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "icas", 4) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ismo", 4) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ável", 4) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ível", 4) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ista", 4) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "osos", 4) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "osas", 4) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ador", 4) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ivas", 4) )
				{
				if (delete_if_is_in_r2(text, "at", 2) )
					{
					//NOOP (fall through to branching statement)
					}
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ivos", 4) )
				{
				if (delete_if_is_in_r2(text, "at", 2) )
					{
					//NOOP (fall through to branching statement)
					}
				//NOOP (fall through to branching statement)
				}			
			else if (is_suffix(text, "eira", 4) )
				{
				if (m_rv <= static_cast<int>(text.length()-3) )
					{
					text.erase(text.end()-1, text.end() );
					m_altered_suffix_index = text.length()-3;
					update_r_sections(text);
					}
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "iva", 3) )
				{
				if (delete_if_is_in_r2(text, "at", 2) )
					{
					//NOOP (fall through to branching statement)
					}
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ivo", 3) )
				{
				if (delete_if_is_in_r2(text, "at", 2) )
					{
					//NOOP (fall through to branching statement)
					}
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "eza", 3) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ico", 3) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "ica", 3) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "oso", 3) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_r2(text, "osa", 3) )
				{
				//NOOP (fall through to branching statement)
				}
			//branch to the next appropriate step
			if (original_length == text.length() )
				{
				step_2(text);
				}
			else
				{
				m_step1_step2_altered = true;
				step_3(text);
				}
 			}

		//---------------------------------------------
		void step_2(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			size_t original_length = text.length();
			
			if (delete_if_is_in_rv(text, "aríamos", 7, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "eríamos", 7, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "iríamos", 7, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ássemos", 7, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "êssemos", 7, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "íssemos", 7, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "aríeis", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "eríeis", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "iríeis", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ásseis", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ésseis", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ísseis", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "áramos", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "éramos", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "íramos", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ávamos", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "aremos", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "eremos", 6, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "iremos", 6 ,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ariam", 5, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "eriam", 5, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "iriam", 5, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "assem", 5, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "essem", 5, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "issem", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ara~o", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "era~o", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ira~o", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "arias", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "erias", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "irias", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ardes", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "erdes", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "irdes", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "asses", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "esses", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "isses", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "astes", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "estes", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "istes", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "áreis", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "areis", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "éreis", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ereis", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "íreis", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ireis", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "áveis", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "íamos", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "armos", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ermos", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "irmos", 5,false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "aria", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "eria", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "iria", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "asse", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "esse", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "isse", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "aste", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "este", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "iste", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "arei", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "erei", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "irei", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "aram", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "eram", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "iram", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "avam", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "arem", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "erem", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "irem", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ando", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "endo", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "indo", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "adas", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "idas", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "arás", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "aras", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "erás", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "eras", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "irás", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "avas", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ares", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "eres", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ires", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "íeis", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ados", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "idos", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ámos", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "amos", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "emos", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "imos", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "iras", 4, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ada", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ida", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ará", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ara", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "erá", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "era", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "irá", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ava", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "iam", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ado", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ido", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ias", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ais", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "eis", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ira", 3, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ia", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ei", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "am", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "em", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ar", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "er", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ir", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "as", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "es", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "is", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "eu", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "iu", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}
			else if (delete_if_is_in_rv(text, "ou", 2, false) )
				{
				//NOOP (fall through to branching statement)
				}

			if (original_length != text.length() )
				{
				m_step1_step2_altered = true;
				step_3(text);
				}
			}
		//---------------------------------------------
		void step_3(std::basic_string<Tchar_type, Tchar_traits>& text) 
			{
			if (is_suffix(text, "ci", 2) &&
				m_rv <= text.length()-1)
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			//in case step 1 did not fully remove the standard suffix
			else if (m_altered_suffix_index >= 2 &&
				text.compare(m_altered_suffix_index-2, 2, "ci", 2)==0)
				{
				//remove the 'i' that's inside of the string
				text.erase(m_altered_suffix_index-1, 1);
				update_r_sections(text);
				}
			}
		//---------------------------------------------
		void step_4(std::basic_string<Tchar_type, Tchar_traits>& text) 
			{
			if (delete_if_is_in_rv(text, "os", 2) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "a", 1) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "i", 1) )
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
			}
		//---------------------------------------------
		void step_5(std::basic_string<Tchar_type, Tchar_traits>& text) 
			{
			if (delete_if_is_in_rv(text, "e", 1, false) )
				{
				if (m_rv <= text.length()-1 &&
					(is_suffix(text, "gu", 2) ||
					is_suffix(text, "ci", 2)) )
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (delete_if_is_in_rv(text, "é", 1, false) )
				{
				if (m_rv <= text.length()-1 &&
					(is_suffix(text, "gu", 2) ||
					is_suffix(text, "ci", 2)) )
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (delete_if_is_in_rv(text, "ê", 1, false) )
				{
				if (m_rv <= text.length()-1 &&
					(is_suffix(text, "gu", 2) ||
					is_suffix(text, "ci", 2)) )
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				return;
				}
			else if (text[text.length()-1] == 'Ç')
				{
				text[text.length()-1] = 'C';
				}
			else if (text[text.length()-1] == 'ç')
				{
				text[text.length()-1] = 'c';
				}
			}		
		//internal data specific to Portuguese stemmer
		bool m_step1_step2_altered;
		size_t m_altered_suffix_index;
		};
	}

#endif //__PORTUGUESE_STEM_H__