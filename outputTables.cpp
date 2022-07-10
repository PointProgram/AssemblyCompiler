#include "compilerSettings.h"

void Compiler::print_header(std::stringstream &ss) {
	ss << "\t\t\t\t\t\t\t\t\t________________\n";
	ss << "\t\t\t\t\t\t\t\t\t|Table of tokens|\n";
	ss << std::string(104, '*');
	ss << "\n|   N   |\t Lexeme\t\t\t\t|    Lexeme length in   |\t\tLexeme type\t\t\t\t\t\t\t\t|";
	ss << "\n|       |\t\t\t\t\t\t|\t\tcharacters  \t|\t\t\t\t\t\t\t\t\t\t\t\t|\n";
	ss << "|_______|_______________________|_______________________|_______________________________________________|";
}

void Compiler::print_title(size_t num, string lexem, size_t len, string descr, stringstream &ss) {
	ss << "\n|" << setw(6) << num << " |" << setw(22) << lexem.c_str() << " |" << setw(22) << len << " | " << setw(45) <<descr.c_str() <<" |";
	ss << "\n|_______|_______________________|_______________________|_______________________________________________|";

}

void Compiler::print_lex_part(int n_field, int f_mnem, int l_mnem, int f_op, int f_op_len, int s_op, int s_op_len, int t_op, int t_op_len, stringstream &ss) {
	string n_f, f_m, l_m, f_o, f_o_l, s_o, s_o_l, t_o, t_o_l;
	if (n_field == -1)
		n_f = "-";
	else
		n_f = to_string(n_field);
	if (f_mnem == -1)
		f_m = "-";
	else
		f_m = to_string(f_mnem);
	if (l_mnem == 0)
		l_m = "-";
	else
		l_m = to_string(l_mnem);
	if (f_op == -1)
		f_o = "-";
	else
		f_o = to_string(f_op);
	if (f_op_len == 0)
		f_o_l = "-";
	else
		f_o_l = to_string(f_op_len);
	if (s_op == -1)
		s_o = "-";
	else
		s_o = to_string(s_op);
	if (s_op_len == 0)
		s_o_l = "-";
	else
		s_o_l = to_string(s_op_len);
	if (t_op == -1)
		t_o = "-";
	else
		t_o = to_string(t_op);
	if (t_op_len == 0)
		t_o_l = "-";
	else
		t_o_l = to_string(t_op_len);
	ss << endl;
	ss << "\t\t\t\t\t\t\t\t__________________________\n";
	ss << "\t\t\t\t\t\t\t\t|Sentence structure table|\n";
	ss << std::string(89, '-') << endl;
	ss << "|" << "Label field" << "|  " << "Mnemonic field" << "  |    " << "1st operand" << "   |    " << "2nd operand" << "   |    " << "3rd operand" << "   |" << endl;
	ss << "|" << "N lexeme" << "   |" << "N1 lexeme" << "|" << "Length" << "  |" << "N2 lexeme " << "|" << "Length " << "|" << "N3 lexeme " << "|" << "Length " << "|" << "N4 lexeme " << "|" << "Length |" << endl;
	ss << "|     " << n_f << "     |    " << f_m << "    |   " << l_m << "    |     " << f_o << "    |    " << f_o_l << "  |     " << s_o << "    |  " << s_o_l << "    |     " << t_o << "    |  " << t_o_l << "    |" << endl;
	ss << std::string(89, '-');
}

void Compiler::print_seg(stringstream &ss) {
	ss << endl <<std::string(61, '_');

	ss << "\n|Segment name |\t\tDefault bit rate\t   | Current offset |\n";
	ss << "|_____________|____________________________|________________|\n";

	for (size_t i = 0;i < eb.size();i++) {
		if (eb[i].r_type == "segment") {
			stringstream stream;
			stream << hex << eb[i].offset;
			string st = stream.str();
			//printf("|%10s   |            %4d            |      %4s      |\n", eb[i]._name.c_str(), 16, st.c_str());
			ss << "|" << setfill(' ') << setw(12) << eb[i]._name.c_str() <<" |" << setw(16) << 16 << "\t\t\t   |\t\t" << setfill('0') << setw(4) << st.c_str() <<setfill(' ') << " \t|\n";
			//printf("|_____________|____________________________|________________|\n");
			ss << "|_____________|____________________________|________________|\n";
		}
	}
}

void Compiler::print_assume(string cs, string ds, string ss, string es, string gs, string fs, stringstream &strs) {
	strs << endl << std::string(47, '_');

	strs << "\n|  Segment register   |       Assignment      |\n";
	strs << "|_____________________|_______________________|";
	strs << "\n|\t\t CS\t\t\t  |\t\t  " << setfill(' ') << setw(8) <<  cs.c_str() << "\t\t  |";
	strs << "\n|\t\t DS\t\t\t  |\t\t  " << setw(8) <<  ds.c_str() << "\t\t  |";
	strs << "\n|\t\t SS\t\t\t  |\t\t  " << setw(8) <<  ss.c_str() << "\t\t  |";
	strs << "\n|\t\t ES\t\t\t  |\t\t  " << setw(8) <<  es.c_str() << "\t\t  |";
	strs << "\n|\t\t GS\t\t\t  |\t\t  " << setw(8) <<  gs.c_str() << "\t\t  |";
	strs << "\n|\t\t FS\t\t\t  |\t\t  " << setw(8) <<  fs.c_str() << "\t\t  |\n";
	strs << "|_____________________|_______________________|";
}


string convert(string _reg, string _mod, string _rm) {
	string mod_r;
	mod_r += _mod;
	mod_r += _reg;
	mod_r += _rm;
	string binary_str(mod_r);
	bitset<8> set(binary_str);
	stringstream res;
	res << hex << uppercase << set.to_ulong();
	return res.str();

}

void Compiler::pr_table(bool stga, stringstream &ss) {
	numm = 0;
	int j = 0;
	er_check();
	for (int i = 0; i < num_of; i++) {
		for (size_t dep = 0; dep < tr.size(); dep++) {
			if (tr[dep].number == i) {
				j = dep;
			}
		}
		if (tr[j].er_s == true) {
			ss << "|" << tr[j].number + 1 << "	";
			ss << tr[j].e_type << "     " << tr[j].row_ << " " << endl;
			continue;
		}
		stringstream stream, st, s, s_r;
		string s1, s2;
		if (tr[j].empty == true) {
			ss << endl;
			ss << "|" << tr[j].number + 1 << "	";
			ss << "				";
			for (int i = tr[j].begg; i <= tr[j].endd; i++) {
				ss << lex_em[i] << " ";
			}
			ss << endl;
			ss << endl;
			continue;
		}
		
		stream << hex << uppercase << tr[j].of_size;
		ss << "|" << tr[j].number + 1 << "	";
		
		s1 = stream.str();
		ss << std::setfill('0') << std::setw(4) << s1 << "    ";
		
		st << hex << uppercase << tr[j].main_offs;
		s2 = st.str();
		ss << std::setfill('0') << std::setw(4) << s2 << "    ";
		if (stga == true) {
			for (size_t ii = 0; ii < tr.size(); ii++) {
				if (tr[ii].number == tr[j].number) {
					if (!tr[ii].reg.empty() || !tr[ii].mod.empty() || !tr[ii].rm.empty()) {
						tr[ii].mod_r_m = convert(tr[ii].reg, tr[ii].mod, tr[ii].rm);
					}
					if (tr[ii].off_set != -1)
						s << hex << uppercase << tr[ii].off_set;
					if (tr[ii].jm != -1) {
						s_r << hex << uppercase << tr[ii].jm;
					}
					if (!tr[ii].pref.empty()) {
						ss << tr[ii].pref << ": ";
					}
					if (!tr[ii].b_over.empty()) {
						ss << tr[ii].b_over << "| ";
					}
					if (!tr[ii].op.empty()) {
						ss << tr[ii].op << " ";
					}
					if (!tr[ii].mod_r_m.empty()) {
						ss << tr[ii].mod_r_m << " ";
					}
					if (tr[ii].jm != -1) {
						ss << s_r.str() << " ";
					}
					if (!tr[ii].j_m.empty()) {
						ss << tr[ii].j_m << " ";
					}
					if (!s.str().empty())
						ss << std::setfill('0') << std::setw(4) << s.str() << " ";
				}
			}
		}
		ss << "    ";
		for (int i = tr[j].begg; i <= tr[j].endd; i++) {
			ss << lex_em[i] << " ";
		}
		ss << endl;
	}
}



void Compiler::pr_use(stringstream &ss) {
	for (size_t i = 0; i < eb.size(); i++) {
		for (size_t j = 0; j < tb.size();j++) {
			if (tb[j].nam == eb[i]._name) {
				eb[i].offset = tb[j]._offset;
				eb[i]._seg = tb[j].sg;
			}
		}
	}

	ss << endl << std::string(44, '_');

	string st, seg;
	bool b = false;
	ss << "\n| Identifier  |   Type    |      Value     |\n";
	ss << "|_____________|___________|________________|\n";

	for (size_t i = 0;i < eb.size();i++) {
		if (eb[i].r_type == "segment") {
			seg = "---";
			st = "---";
			b = true;
		}
		else {
			stringstream stream;
			stream << hex << uppercase << eb[i].offset;
			st = stream.str();
			seg = eb[i]._seg;
			b = false;
		}
		if (b == false) {
			//printf("|%10s   |%11s|%11s:%4s|\n", eb[i]._name.c_str(), eb[i]._type.c_str(), seg.c_str(), st.c_str());
			ss << "|" << setw(10) << eb[i]._name.c_str()<< "   |" << setw(11) << eb[i]._type.c_str() << "|" \
			<< setw(11) << seg.c_str() << ":" << setfill('0') <<setw(4) << st.c_str() << setfill(' ') << "|\n";
			//printf("|_____________|___________|________________|\n");
			ss << "|_____________|___________|________________|\n";
		}
		else {
			//printf("|%10s   |%11s|%12s%4s|\n", eb[i]._name.c_str(), eb[i]._type.c_str(), seg.c_str(), st.c_str());
			
			ss << "|" << setw(10) << eb[i]._name.c_str()<< "   |" << setw(11) << eb[i]._type.c_str() << "|" \
			<< setw(11) << seg.c_str() << ":" << setw(4) << st.c_str() << "|\n";
			ss << "|_____________|___________|________________|\n";
		}

	}
}