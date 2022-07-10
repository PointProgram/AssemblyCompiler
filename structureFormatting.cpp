#include "compilerSettings.h"

using namespace std;

void Compiler::read_f(string name) {
	my_f.open(name);
}
int Compiler::not_open() {
	if (my_f.is_open()) {
		return 0;
	}
	else return 1;
}
void delUnnecessary(string& str) {
	int size = str.length();
	int i = 0, j = -1;
	bool find_space = false;
	while (++j < size && ((str[j] == ' ') || (str[j] == '	')));
	while (j < size) {
		if (str[j] != '	' || str[j] != ' ') {
			str[i++] = str[j++];
			find_space = false;
		}
		else if (str[j++] == '	' || str[j++] == ' ') {
			if (!find_space)
			{
				str[i++] = ' ';
				find_space = true;
			}
		}
	}
	if (i <= 1)
		str.erase(str.begin() + i, str.end());
	else
		str.erase(str.begin() + i, str.end());
}




void Compiler::line_read(OUTPUT out_parameter) {
	string s;
	bool ch_s = false;
	num_of = 0;
	stringstream ss;	
	
	while (getline(my_f, s)) {
		for (size_t i = 0; i < s.length();i++) {
			if ((s[0] == '\0') || (s[0] == '\n') || (s[i] == '	') || (s[i] == ' ')) {
				ch_s = true;
			}
			else {
				ch_s = false;
				break;
			}
		}
		if ((ch_s == false) && (s.length() > 0)) {
			delUnnecessary(s);
			if (out_parameter) {
				ss << endl <<  "Line: " << s << endl;
				print_header(ss);
				analize(s, static_cast<bool>(out_parameter), true, ss);
				ss << endl << endl;
			}
			else {
				analize(s, static_cast<bool>(out_parameter), true, ss);
			}
		}
	}

	if (out_parameter == FILEOUT) {
		identifiersTable.open(identifiersTableFilePath);
		identifiersTable << ss.str();
		identifiersTable.close();

	}
	else if (out_parameter == STDOUT)
		cout << ss.str();

}

string Compiler::type(string w, bool s_const, size_t len) {

	int l_check = 0;
	if (s_const == true) {
		return "Text constant";
	}
	if (len == 1) {
		return "One character";
	}
	else {
		if (w == "DB") {
			return "Data directive identifier type 1";
		}
		else if (w == "DW") {
			return "Data directive identifier type 2";
		}
		else if (w == "DD") {
			return "Data directive identifier type 3";
		}
		else if ((w == "AX") || (w == "BX") || (w == "CX") || (w == "DX") || (w == "SP") || (w == "BP") || (w == "SI") || (w == "DI")) {
			return "Identifier of the 16-bit address register";
		}
		else if ((w == "CS") || (w == "DS") || (w == "SS") || (w == "ES") || (w == "GS") || (w == "FS")) {
			return "Identifier of the segment register";
		}
		else if ((w == "PUSH") || (w == "RET") || (w == "POP") || (w == "MOV") || (w == "JNE") || (w == "CALL")) {
			return "Machine instruction mnemonic identifier";
		}
		else if ((w == "ENDS") || (w == "PROC") || (w == "ENDP") || (w == "ASSUME")) {
			return "Identifier of a directive with one operand";
		}
		else if ((w == "END") || (w == "SEGMENT")) {
			return "Directive identifier without operands";
		}
		else {
			for (size_t i = 0; i < len - 1; i++) {
				if ((((w[i] >= '0') && (w[i] <= '9')) || ((w[i] >= 'A') && (w[i] <= 'F'))) && (w[len - 1] == 'H')) {
					l_check++;
					if (static_cast<size_t>(l_check) == len - 1) {
						l_check = 0;
						return "Hexadecimal constant";
					}
				}
			}
			for (size_t i = 0; i < len; i++) {
				if (((w[0] >= 'A') && (w[0] <= 'Z')) && (((w[i] >= '0') && (w[i] <= '9')) || ((w[i] >= 'A') && (w[i] <= 'Z'))) && (w.length() <= 7)) {
					l_check++;
					if (static_cast<size_t>(l_check) == len) {
						l_check = 0;
						return "The user ID or undefined";
					}
				}
			}
			for (size_t i = 0; i < len - 1; i++) {
				if (((w[0] >= 'A') && (w[0] <= 'Z')) && (((w[i] >= '0') && (w[i] <= '9')) || ((w[i] >= 'A') && (w[i] <= 'Z'))) && (w.length() <= 7) && (w[len - 1] == ':')) {
					return "Label";
				}
				else {
					return "ERROR";
				}
			}

		}
	}
	return "";
}



void Compiler::lex_par(vector<string> lex, stringstream &ss) {
	int name_field = -1, first_mnem = -1, len_mnem = 0, first_op = -1, first_op_len = 0, second_op = -1, second_op_len = 0, third_op = -1, third_op_len = 0;
	bool com = false, ero = false, com1 = false;
	int tmp, k = 0, _i = 0;
	error rr;
	for (size_t i = 0; i < lex.size();i++) {
		if (lex[i] == ":") {
			if (lex.size() > 1 && _i > 0) k = _i - 1;
			if ((lex[k] != "DS") && (lex[k] != "SS") && (lex[k] != "ES") && (lex[k] != "GS") && (lex[k] != "FS") && (lex[k] != "CS"))
				name_field = k + 1;
			ero = true;
		}
		else if ((lex[i] == "SEGMENT")) {
			if (lex.size() > 1 && _i > 0) k = _i - 1;
			name_field = k + 1;
			first_mnem = i + 1;
			len_mnem += 1;
			ero = true;
		}
		else if ((lex[i] == "ENDS")) {
			if (lex.size() > 1 && _i > 0) k = _i - 1;
			name_field = k + 1;
			first_mnem = i + 1;
			len_mnem += 1;
			ero = true;
		}
		else if ((lex[i] == "ENDP")) {
			if (lex.size() > 1 && _i > 0) k = _i - 1;
			name_field = k + 1;
			first_mnem = _i + 1;
			len_mnem += 1;
			ero = true;
		}
		else if ((lex[i] == "PROC")) {
			if (lex.size() > 1 && _i > 0) k = _i - 1;
			name_field = k + 1;
			first_mnem = _i + 1;
			len_mnem += 1;
			ero = true;
		}
		else if ((lex[i] == "DB" || lex[i] == "DW" || lex[i] == "DD")) {
			if (lex.size() > 1 && _i > 0) k = _i - 1;
			name_field = k + 1;
			first_mnem = _i + 1;
			len_mnem += 1;
			for (size_t j = _i + 1; j < lex.size(); j++) {
				first_op = _i + 2;
				first_op_len += 1;
				if (j == lex.size() - 1) break;
			}
			ero = true;
		}
		else if ((lex[i] == "END")) {
			name_field = _i + 1;
			if (lex.size() > 1) {
				first_mnem = _i + 2;
				len_mnem += 1;
			}
			ero = true;
		}
		else if (lex[i] == "ASSUME") {
			first_mnem = _i + 1;
			len_mnem += 1;
			for (size_t j = _i + 1; j < lex.size(); j++) {
				if (lex[j] == ",") {
					if (com == true)
						com1 = true;
					com = true;
					tmp = j + 1;
					continue;
				}
				if (com == false) {
					first_op = _i + 2;
					first_op_len += 1;
				}
				else if (com1 == true) {
					third_op = tmp + 1;
					third_op_len += 1;
				}
				else if ((com == true) && (com1 == false)) {
					second_op = tmp + 1;
					second_op_len += 1;
				}
				if (j == lex.size() - 1) break;
			}
			ero = true;
		}
		else if ((lex[i] == "RET")) {
			first_mnem = _i + 1;
			len_mnem += 1;
			ero = true;
		}
		else if ((lex[i] == "PUSH")) {
			first_mnem = _i + 1;
			len_mnem += 1;
			for (size_t j = _i + 1; j < lex.size(); j++) {
				first_op = _i + 2;
				first_op_len += 1;
				if (j == lex.size() - 1) break;
			}
			ero = true;
		}
		else if ((lex[i] == "POP")) {
			first_mnem = _i + 1;
			len_mnem += 1;
			for (size_t j = _i + 1; j < lex.size(); j++) {
				first_op = _i + 2;
				first_op_len += 1;
				if (j == lex.size() - 1) break;
			}
			ero = true;
		}
		else if ((lex[i] == "MOV")) {
			first_mnem = _i + 1;
			len_mnem += 1;
			for (size_t j = _i + 1; j < lex.size(); j++) {
				if (lex[j] == ",") {
					com = true;
					tmp = j + 1;
					continue;
				}
				if (com == false) {
					first_op = i + 2;
					first_op_len += 1;
				}
				else {
					second_op = tmp + 1;
					second_op_len += 1;
				}
				if (j == lex.size() - 1) break;
			}
			ero = true;
		}
		else if ((lex[i] == "JNE")) {
			first_mnem = _i + 1;
			len_mnem += 1;
			for (size_t j = _i + 1; j < lex.size(); j++) {
				first_op = _i + 2;
				first_op_len += 1;
				if (static_cast<size_t>(j) == lex.size() - 1) break;
			}
			ero = true;
		}
		else if ((lex[i] == "CALL")) {
			first_mnem = _i + 1;
			len_mnem += 1;
			for (size_t j = _i + 1; j < lex.size(); j++) {
				first_op = _i + 2;
				first_op_len += 1;
				if (static_cast<size_t>(j) == lex.size() - 1) break;
			}
			ero = true;
		}
		if (ero == false && i == lex.size() - 1) {
			rr.line = num_of;
			rr.n_er = "SYNTAX ERROR";
			er.push_back(rr);
			//cout << lex[i] << " this word"<< endl;
		}
		if (i == lex.size() - 1) {
			print_lex_part(name_field, first_mnem, len_mnem, first_op, first_op_len, second_op, second_op_len, third_op, third_op_len, ss);
		}
		_i++;
	}
}
void Compiler::analize(string word, bool lex_part, bool stg,  stringstream &ss) {
	int length = word.length();
	string ptr_word, str_lex;
	vector<string> _lex;
	error rr;
	bool comment = false, string_const1 = false, string_const2 = false, construct = false, first_set = false, err = false, cap = false;
	size_t num = 0;
	for (int i = 0; i < length; i++) {
		if ((((word[i] >= 'A') && (word[i] <= 'Z')) || ((word[i] >= '0') && (word[i] <= '9')) || ((word[i] == '_') || (word[i] == '@') || (word[i] == '?') || (word[i] == '$'))) && (comment == false) && (string_const1 == false)) {
			str_lex.push_back(word[i]);
			construct = true;
			err = true;
		}
		else if (((word[i] == ':') || (word[i] == ',') || (word[i] == '[') || (word[i] == ']') || (word[i] == '+')) && (comment == false) && (string_const1 == false)) {
			ptr_word = word[i];
			if (word[i] == '[') {
				cap = true;
			}
			else if (word[i] == ']') {
				cap = false;
			}
			if (construct == true) {
				//call function to analize
				if (lex_part == true)
					print_title(++num, str_lex, str_lex.length(), type(str_lex, string_const1, str_lex.length()), ss);
				if (type(str_lex, string_const1, str_lex.length()) == "ERROR") {
					rr.line = num_of;
					rr.n_er = "SYNTAX ERROR";
					rr.row = word;
					er.push_back(rr);
					break;
				}
					_lex.push_back(str_lex);
					if (stg == true)
						lex_em.push_back(str_lex);
				construct = false;
				str_lex.clear();
			}
			//call function to analize
			if (lex_part == true)
				print_title(++num, ptr_word, ptr_word.length(), type(ptr_word, string_const1, ptr_word.length()), ss);
			if (type(ptr_word, string_const1, ptr_word.length()) == "ERROR") {
				rr.line = num_of;
				rr.n_er = "SYNTAX ERROR";
				rr.row = word;
				er.push_back(rr);
				break;
			}
			_lex.push_back(ptr_word);
			if (stg == true)
				lex_em.push_back(ptr_word);
			err = true;
		}
		else if (((word[i] == '-') || (word[i] == '*') || (word[i] == '/')) && (comment == false) && (string_const1 == false) && (cap == true)) {
			ptr_word = word[i];
			if (lex_part == true) 
				print_title(++num, ptr_word, ptr_word.length(), type(ptr_word, string_const1, ptr_word.length()), ss);
			if (type(ptr_word, string_const1, ptr_word.length()) == "ERROR") {
				rr.line = num_of;
				rr.n_er = "SYNTAX ERROR";
				rr.row = word;
				er.push_back(rr);
				break;
			}
			_lex.push_back(ptr_word);
			if (stg == true)
				lex_em.push_back(ptr_word);
			
			err = true;
		}
		else if (((word[i] == '	') || (word[i] == ' ')) && (string_const1 == false)) {
			if (construct == true) {
				//call function to analize
				if (lex_part == true)
					print_title(++num, str_lex, str_lex.length(), type(str_lex, string_const1, str_lex.length()), ss);
				if (type(str_lex, string_const1, str_lex.length()) == "ERROR") {
					rr.line = num_of;
					rr.n_er = "SYNTAX ERROR";
					rr.row = word;
					er.push_back(rr);
					break;
				}
				_lex.push_back(str_lex);
				if (stg == true)
					lex_em.push_back(str_lex);
			
				construct = false;
				str_lex.clear();
			}
			err = true;
		}
		else if ((word[i] == ';') && (comment == false) && (string_const1 == false)) {
			comment = true;
			str_lex.clear();
			err = true;
		}
		else if (((word[i] == '\'') || (word[i] == '"')) && (comment == false)) {
			first_set = true;
			if (string_const1 == true) {
				if ((word[i + 1] == ' ') || (word[i + 1] == ' ') || (word[i + 1] == '\0')) {
					string_const2 = true;
					if (lex_part == true)
						print_title(++num, str_lex, str_lex.length(), type(str_lex, string_const1, str_lex.length()), ss);
					if (type(str_lex, string_const1, str_lex.length()) == "ERROR") {
						rr.line = num_of;
						rr.n_er = "SYNTAX ERROR";
						rr.row = word;
						er.push_back(rr);
						break;
					}
					_lex.push_back(str_lex);
					if (stg == true)
						lex_em.push_back(str_lex);
					constant.push_back(str_lex);
					
					str_lex.clear();
				}
				else {
					first_set = false;
				}
			}
			string_const1 = true;
			if (string_const2 == true) {
				string_const1 = false;
				string_const2 = false;
			}
			err = true;
		}
		else if (!string_const1) {
			rr.line = num_of;
			rr.n_er = "SYNTAX ERROR";
			rr.row = word;
			er.push_back(rr);
			break;
		}
		if (string_const1 == true) {
			if (first_set != true)
				str_lex += word[i];
			first_set = false;
		}
		if ((i == (length - 1)) && (construct == true)) {
			//call function to analize
			if (lex_part == true)
				print_title(++num, str_lex, str_lex.length(), type(str_lex, string_const1, str_lex.length()), ss);
			if (type(str_lex, string_const1, str_lex.length()) == "ERROR") {
				rr.line = num_of;
				rr.n_er = "SYNTAX ERROR";
				rr.row = word;
				er.push_back(rr);
				break;
			}
			_lex.push_back(str_lex);
			if (stg == true)
				lex_em.push_back(str_lex);
			
			construct = false;
			str_lex.clear();
		}
		if (err == false && i == length - 1) {
			rr.line = num_of;
			rr.n_er = "SYNTAX ERROR";
			rr.row = word;
			er.push_back(rr);
			break;
		}
		if (i == length - 1) {
			if (stg == true) {
				lex_em.push_back("nopi");
				num_of++;
			}
		}
		if (i == length - 1 && lex_part == true) {
			lex_par(_lex, ss);
			for (size_t i = 0;i < er.size();i++) {
				if (er[i].line == num_of) {
					er[i].row = word;
				}
			}
		}
	}
}

void Compiler::seg_analize(vector<string> vec) {

	string seg_name;
	int beg = 0, end = -2, num = -1;
	bool exi;
	for (size_t i = 0;i < vec.size(); i++) {
		//check on error
		while (vec[i] != "nopi") {
			i++;
		}
		num++;
		beg = end + 2;
		end = i - 1;
		exi = false;
		uName en;
		for (int j = beg; j <= end; j++) {
			if (end - beg == 1 && type(vec[beg], false, vec[beg].length()) == "The user ID or undefined" && vec[end] == "SEGMENT") {
				seg_name = vec[beg];
				for (size_t k = 0; k < eb.size();k++) {
					if (eb[k]._name == seg_name && eb[k].r_type == "segment") {
						cout << "Segment already exists with that name" << endl;
						exi = true;
					}
				}
				if (exi == true)
					break;
				en._name = seg_name;
				en._type = "Segment";
				en.r_type = "segment";
				//roz = 16;
				_offset = 0;
				en._beg = num;
				eb.push_back(en);
				//cout << vec[beg] << " " << vec[end] << endl;
				break;
			}
			else if (end - beg == 1 && type(vec[beg], false, vec[beg].length()) == "The user ID or undefined" && vec[end] == "ENDS") {
				Active_seg = 0;
				seg_name = vec[beg];
				for (size_t k = 0; k < eb.size();k++) {
					if (eb[k]._name == seg_name && eb[k].r_type == "segment") {
						exi = true;
						eb[k]._end = num;
						//cout << vec[beg] << " " << vec[end] << endl;
					}
					if (k == (eb.size() - 1) && exi == false) {
						error rr;
						rr.line = num;
						rr.row = "ERROR";
						er.push_back(rr);
					}
				}
				break;
			}
		}
	}
}



void Compiler::assume_analize(vector<string> vec, bool pr, stringstream &ss) {

	string assume_name;
	int k = 0;
	string _cs = "Nothing", _ds = "Nothing", _ss = "Nothing", _es = "Nothing", _gs = "Nothing", _fs = "Nothing";
	int num = -1, beg = -2, end = 0;
	segDest des;
	for (size_t i = 0;i < vec.size(); i++) {
		//check on err
		while (vec[i] != "nopi") {
			i++;
		}
		num++;
		error rr;
		beg = end + 2;
		end = i - 1;
		int b = beg;
		if (vec[beg] == "ASSUME") {

			while (b <= end) {
				if (vec[b] == ":") {
					if (vec.size() > 1 && b > 0) k = b - 1;
					if ((vec[k] == "DS") || (vec[k] == "SS") || (vec[k] == "ES") || (vec[k] == "GS") || (vec[k] == "FS") || (vec[k] == "CS")) {
						if (static_cast<size_t>(b + 1) < vec.size()) b++;
						if (type(vec[b], false, vec[i].length()) == "The user ID or undefined") {
							if (vec[k] == "DS") {
								_ds = vec[b];
								des.name_ds = vec[b];
								des.seg_ds = vec[k];
							}
							else if (vec[k] == "CS") {
								_cs = vec[b];
								des.name_cs = vec[b];
								des.seg_cs = vec[k];
							}
							else if (vec[k] == "SS") {
								_ss = vec[b];
								des.name_ss = vec[b];
								des.seg_ss = vec[k];
							}
							else if (vec[k] == "ES") {
								_es = vec[b];
								des.name_es = vec[b];
								des.seg_es = vec[k];
							}
							else if (vec[k] == "GS") {
								_gs = vec[b];
								des.name_gs = vec[b];
								des.seg_gs = vec[k];
							}
							else if (vec[k] == "FS") {
								_fs = vec[b];
								des.name_fs = vec[b];
								des.seg_fs = vec[k];
							}
							else {
								rr.row = "ERROR";
								rr.line = num;
								string ss;
								for (int i = beg; i <= end; i++) {
									ss += vec[i];
									ss += " ";
								}
								rr.row = ss;
							}
						}
					}
				}
				if (b == end) {
					sb.push_back(des);
					er.push_back(rr);
				}
				b++;
			}
			if (pr == true)
				print_assume(_cs, _ds, _ss, _es, _gs, _fs, ss);
		}
	}

}



void Compiler::var_analize(vector<string> vec) {

	string lab_name, value, var_name;
	int num = -1, beg = -2, end = 0;
	bool exi;
	error rr;
	for (size_t i = 0;i < vec.size(); i++) {
		//check if lexem has error
		while (vec[i] != "nopi") {
			i++;
		}
		num++;
		beg = end + 2;
		end = i - 1;
		exi = false;
		for (int j = beg; j <= end; j++) {
			if (end - beg == 1 && type(vec[beg], false, vec[beg].length()) == "The user ID or undefined" && vec[end] == ":") {
				lab_name = vec[beg];
				uName en;
				for (size_t k = 0; k < eb.size();k++) {
					if (eb[k]._name == lab_name && eb[k].r_type == "label") {
						rr.line = num;
						rr.n_er= "Label already exists with that name";
						er.push_back(rr);
						exi = true;
					}
				}
				if (exi == true)
					break;
				en._name = lab_name;
				en._type = "Near";
				en.r_type = "label";
				en._beg = num;
				en._end = num;
				eb.push_back(en);
				//cout << vec[beg] << " " << vec[end] << endl;
				break;
			}
			else if (end - beg == 2 && type(vec[beg], false, vec[beg].length()) == "The user ID or undefined" && (vec[beg + 1] == "DB" || vec[beg + 1] == "DW" || vec[beg + 1] == "DD")) {
				var_name = vec[beg];
				uName en;
				for (size_t k = 0; k < eb.size();k++) {
					if (eb[k]._name == var_name && eb[k].r_type == "var") {
						rr.line = num;
						rr.n_er = "Label already exists with that name";
						er.push_back(rr);
						exi = true;
					}
				}
				if (exi == true)
					break;
				en._name = var_name;
				if (vec[beg + 1] == "DB")
					en._type = "Byte";
				else if (vec[beg + 1] == "DW")
					en._type = "Word";
				else if (vec[beg + 1] == "DD")
					en._type = "Dword";
				en.r_type = "var";
				en._beg = num;
				en._end = num;
				en.value = vec[end];
				eb.push_back(en);
				//cout << vec[beg] << " " << vec[end] << endl;
				break;
			}
		}
	}
}

void Compiler::proc_analize(vector<string> vec) {

	string value, proc_name;
	int num = -1, beg = -2, end = 0;
	bool exi;
	for (size_t i = 0;i < vec.size(); i++) {
		while (vec[i] != "nopi") {
			i++;
		}
		num++;
		beg = end + 2;
		end = i - 1;
		exi = false;
		error rr;
		uName en;
		for (int j = beg; j <= end; j++) {
			if (end - beg == 1 && type(vec[beg], false, vec[beg].length()) == "The user ID or undefined" && vec[end] == "PROC") {
				proc_name = vec[beg];
				for (size_t k = 0; k < eb.size();k++) {
					if (eb[k]._name == proc_name && eb[k].r_type == "proc") {
						rr.line = num;
						rr.n_er = "Label already exists with that name";
						er.push_back(rr);
						exi = true;
					}
				}
				if (exi == true)
					break;
				en._name = proc_name;
				en._type = "Near";
				en.r_type = "proc";
				en._beg = num;
				eb.push_back(en);
				//cout << vec[beg] << " " << vec[end] << endl;
				break;
			}
			else if (end - beg == 1 && type(vec[beg], false, vec[beg].length()) == "The user ID or undefined" && (vec[end] == "ENDP")) {
				proc_name = vec[beg];
				for (size_t k = 0; k < eb.size();k++) {
					if (eb[k]._name == proc_name && eb[k].r_type == "proc") {
						exi = true;
						eb[k]._end = num;
						//cout << vec[beg] << " " << vec[end] << endl;
					}
					if (k == (eb.size() - 1) && exi == false) {
						rr.line = num;
						rr.n_er = "Cannot close nonexisting segment";
						er.push_back(rr);
					}
				}
				break;
			}
		}
	}
}




void Compiler::er_check() {
	for (size_t y = 0; y < er.size(); y++) {
		if (er[y].line != -1) {
			transcript trans;
			trans.number = er[y].line;
			trans.row_ = er[y].row;
			trans.e_type = er[y].n_er;
			trans.er_s = true;
			tr.push_back(trans);
		}
	}
}



void Compiler::compose_full_compiled_file(bool is_, OUTPUT out_parameter) {
	stringstream ss;
	line_read(NOTOUT);
	assume_analize(lex_em, false, ss);

	seg_analize(lex_em);
	var_analize(lex_em);
	proc_analize(lex_em);

	if (!is_)
		ss << "|N |" << "Size  |" << "Offset" << "|---------Row---------" << endl;
	else
		ss << "|N |" << "Size  |" << "Offset|" << " Bytes " <<"|---------Row---------" << endl;
	size_count(lex_em);

	pr_table(is_, ss);
	assume_analize(lex_em, true, ss);
	print_seg(ss);
	pr_use(ss);

	if (out_parameter == FILEOUT) {
		identifiersTable.open(listingFilePath);
		identifiersTable << ss.str();
		identifiersTable.close();

	}
	else if (out_parameter == STDOUT)
		cout << ss.str();
}