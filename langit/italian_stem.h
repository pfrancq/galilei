/***************************************************************************
                          italian_stem.h  -  description
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

#ifndef __ITALIAN_STEM_H__
#define __ITALIAN_STEM_H__

#include "stemming.h"

namespace stemming
	{
	//------------------------------------------------------
	template<typename Tchar_type = char,
			typename Tchar_traits = std::char_traits<Tchar_type> >
	class italian_stem : stem<Tchar_type,Tchar_traits>
		{
	public:
		//---------------------------------------------
		void operator()(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (text.length() < 3)
				{
				italian_acutes_to_graves(text);
				return;
				}

			//reset internal data
			m_r1 = m_r2 = m_rv =0;

			trim_western_punctuation(text);
			italian_acutes_to_graves(text);
			hash_italian_ui(text, "aeiouàèìòùAEIOUÀÈÌÒÙ");

			find_r1(text, "aeiouàèìòùAEIOUÀÈÌÒÙ");
			find_r2(text, "aeiouàèìòùAEIOUÀÈÌÒÙ");
			find_spanish_rv(text, "aeiouàèìòùAEIOUÀÈÌÒÙ");

			//step 0:
			step_0(text);
			//step 1:
			size_t text_length = text.length();
			step_1(text);

			//step 2 is called only if step 1 did not remove a suffix
			if (text_length == text.length() )
				{
				step_2(text);
				}

			//step 3:
			step_3a(text);
			step_3b(text);

			unhash_italian_ui(text);
			}
	private:
		//---------------------------------------------
		void step_0(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (is_suffix(text, "gliela", 6) )
				{
				if (step_0a(text, 6) || step_0b(text, 6) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "gliele", 6) )
				{
				if (step_0a(text, 6) || step_0b(text, 6) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "glieli", 6) )
				{
				if (step_0a(text, 6) || step_0b(text, 6) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "glielo", 6) )
				{
				if (step_0a(text, 6) || step_0b(text, 6) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "gliene", 6) )
				{
				if (step_0a(text, 6) || step_0b(text, 6) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "sene", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "mela", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "mele", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "meli", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "melo", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "mene", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "tela", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "tele", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "teli", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "telo", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "tene", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "cela", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "cele", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "celi", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "celo", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "cene", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "vela", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "vele", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "veli", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "velo", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "vene", 4) )
				{
				if (step_0a(text, 4) || step_0b(text, 4) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "gli", 3) )
				{
				if (step_0a(text, 3) || step_0b(text, 3) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "ci", 2) )
				{
				if (step_0a(text, 2) || step_0b(text, 2) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "la", 2) )
				{
				if (step_0a(text, 2) || step_0b(text, 2) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "le", 2) )
				{
				if (step_0a(text, 2) || step_0b(text, 2) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "li", 2) )
				{
				if (step_0a(text, 2) || step_0b(text, 2) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "lo", 2) )
				{
				if (step_0a(text, 2) || step_0b(text, 2) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "mi", 2) )
				{
				if (step_0a(text, 2) || step_0b(text, 2) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "ne", 2) )
				{
				if (step_0a(text, 2) || step_0b(text, 2) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "si", 2) )
				{
				if (step_0a(text, 2) || step_0b(text, 2) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "ti", 2) )
				{
				if (step_0a(text, 2) || step_0b(text, 2) ) { /*NOOP*/ }
				return;
				}
			else if (is_suffix(text, "vi", 2) )
				{
				if (step_0a(text, 2) || step_0b(text, 2) ) { /*NOOP*/ }
				return;
				}
			}
		//---------------------------------------------
		bool step_0a(std::basic_string<Tchar_type, Tchar_traits>& text, size_t suffix_length)
			{
			if ((text.length() >= suffix_length + 4) &&
				m_rv <= text.length()-4-suffix_length &&
				(text.compare(text.length()-4-suffix_length, 4, "ando", 4) == 0 ||
				text.compare(text.length()-4-suffix_length, 4, "endo", 4) == 0) )
				{
				text.erase(text.end()-suffix_length, text.end() );
				update_r_sections(text);
				return true;
				}
			return false;
			}
		//---------------------------------------------
		bool step_0b(std::basic_string<Tchar_type, Tchar_traits>& text, size_t suffix_length)
			{
			if ((text.length() >= suffix_length + 2) &&
				m_rv <= text.length()-2-suffix_length &&
				(text.compare(text.length()-2-suffix_length, 2, "ar", 2) == 0 ||
				text.compare(text.length()-2-suffix_length, 2, "er", 2) == 0 ||
				text.compare(text.length()-2-suffix_length, 2, "ir", 2) == 0) )
				{
				text.replace(text.end()-suffix_length, text.end(), "e");
				update_r_sections(text);
				return true;
				}
			return false;
			}

		//---------------------------------------------
		void step_1(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (delete_if_is_in_rv(text, "amento", 6) ||
				delete_if_is_in_rv(text, "amenti", 6) ||
				delete_if_is_in_rv(text, "imento", 6) ||
				delete_if_is_in_rv(text, "imenti", 6) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "azione", 6) )
				{
				delete_if_is_in_r2(text, "ic", 2);
				return;
				}
			else if (delete_if_is_in_r2(text, "azioni", 6) )
				{
				delete_if_is_in_r2(text, "ic", 2);
				return;
				}
			else if (is_suffix_in_r2(text, "uzione", 6) ||
				is_suffix_in_r2(text, "uzioni", 6) ||
				is_suffix_in_r2(text, "usione", 6) ||
				is_suffix_in_r2(text, "usioni", 6) )
				{
				text.erase(text.end()-5, text.end() );
				update_r_sections(text);
				return;
				}
			else if (delete_if_is_in_r1(text, "amente", 6) )
				{
				if (delete_if_is_in_r2(text, "iv", 2) )
					{
					delete_if_is_in_r2(text, "at", 2);
					}
				else if (delete_if_is_in_r2(text, "abil", 4) ||
						delete_if_is_in_r2(text, "ic", 2) ||
						delete_if_is_in_r2(text, "os", 2) )
					{ /*NOOP*/ }
				return;
				}
			else if (delete_if_is_in_r2(text, "atrice", 6) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "atrici", 6) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "abile", 5) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "abili", 5) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ibile", 5) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ibili", 5) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "mente", 5) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "atore", 5) )
				{
				delete_if_is_in_r2(text, "ic", 2);
				return;
				}
			else if (delete_if_is_in_r2(text, "atori", 5) )
				{
				delete_if_is_in_r2(text, "ic", 2);
				return;
				}
			else if (is_suffix_in_r2(text, "logia", 5) ||
				is_suffix_in_r2(text, "logie", 5) )
				{
				text.erase(text.end()-2, text.end() );
				update_r_sections(text);
				return;
				}
			else if (is_suffix_in_r2(text, "enza", 4) ||
				is_suffix_in_r2(text, "enze", 4) )
				{
				text.replace(text.end()-2, text.end(), "te" );
				update_r_sections(text);
				return;
				}
			else if (delete_if_is_in_r2(text, "anza", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "anze", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "iche", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ichi", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ismo", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ismi", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ista", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "iste", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "isti", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "istà", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "istè", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "istì", 4) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ico", 3) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ici", 3) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ica", 3) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ice", 3) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "oso", 3) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "osi", 3) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "osa", 3) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ose", 3) )
				{
				return;
				}
			else if (delete_if_is_in_r2(text, "ità", 3) )
				{
				if (delete_if_is_in_r2(text, "abil", 4) ||
					delete_if_is_in_r2(text, "ic", 2) ||
					delete_if_is_in_r2(text, "iv", 2) )
					{ /*NOOP*/ }
				return;
				}
			else if (delete_if_is_in_r2(text, "ivo", 3) ||
				delete_if_is_in_r2(text, "ivi", 3) ||
				delete_if_is_in_r2(text, "iva", 3) ||
				delete_if_is_in_r2(text, "ive", 3) )
				{
				if (delete_if_is_in_r2(text, "at", 2) )
					{
					delete_if_is_in_r2(text, "ic", 2);
					}
				return;
				}
			}
		//---------------------------------------------
		void step_2(std::basic_string<Tchar_type, Tchar_traits>& text)
			{			
			if (delete_if_is_in_rv(text, "erebbero", 8, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irebbero", 8, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "assero", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "assimo", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "eranno", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erebbe", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "eremmo", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ereste", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "eresti", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "essero", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iranno", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irebbe", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iremmo", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ireste", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iresti", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iscano", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iscono", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "issero", 6, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "arono", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "avamo", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "avano", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "avate", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "eremo", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erete", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erono", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "evamo", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "evano", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "evate", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iremo", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irete", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irono", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ivamo", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ivano", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ivate", 5, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ammo", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ando", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "asse", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "assi", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "emmo", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "enda", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ende", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "endi", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "endo", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erai", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erei", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "yamo", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iamo", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "immo", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irai", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irei", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "isca", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "isce", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "isci", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "isco", 4, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ano", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "are", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ata", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ate", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ati", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ato", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ava", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "avi", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "avo", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erà", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ere", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "erò", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ete", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "eva", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "evi", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "evo", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irà", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ire", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "irò", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ita", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ite", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iti", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ito", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "iva", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ivi", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ivo", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ono", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "uta", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ute", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "uti", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "uto", 3, false) )
				{
				return;
				}
			else if (delete_if_is_in_rv(text, "ar", 2, false) )
				{
				return;
				}
			//not in original specification, but used in general implementation
			else if (delete_if_is_in_rv(text, "ir", 2, false) )
				{
				return;
				}
			/*deletion or 'er' from rv is considered problematic,
			but part of the standard*/
			}

		//---------------------------------------------
		void step_3a(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (m_rv <= text.length()-1 &&
				string_util::is_one_of(text[text.length()-1], "aeioàèìòAEIOÀÈÌÒ") )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				if (m_rv <= text.length()-1 &&					
					string_util::is_one_of(text[text.length()-1], "iI") )
					{
					text.erase(text.end()-1, text.end() );
					update_r_sections(text);
					}
				}
			}
		//---------------------------------------------
		void step_3b(std::basic_string<Tchar_type, Tchar_traits>& text)
			{
			if (is_suffix_in_rv(text, "ch", 2) ||
				is_suffix_in_rv(text, "gh", 2) )
				{
				text.erase(text.end()-1, text.end() );
				update_r_sections(text);
				}
			}		
		};
	}

#endif //__ITALIAN_STEM_H__
