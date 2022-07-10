#include "compilerSettings.h"

string check_reg(string reg) {
	if (reg == "AX" || reg == "AL")
		return "000";
	else if(reg == "BX" || reg == "BL")
		return "011";
	else if (reg == "CX" || reg == "CL")
		return "001";
	else if (reg == "DX" || reg == "DL")
		return "010";
	else if (reg == "SP" || reg == "AH")
		return "100";
	else if (reg == "BP" || reg == "CH")
		return "101";
	else if (reg == "SI" || reg == "DH")
		return "110";
	else if (reg == "DI" || reg == "BH")
		return "111";
    return "";
}

void Compiler::size_count(vector<string> vec) {
	int num = -1, beg = 0, end = -2;
	int temp = -1, offs = 0, temp1 = -1;
	bool seg = false, cs = false, proc = false, laf = false, con_seg = true;
	string seg_n;
	tab teb;
	
	for (size_t i = 0; i < vec.size(); i++) {
		while (vec[i] != "nopi") {
			i++;
		}
		num++;
		beg = end + 2;
		end = i - 1;
		for (size_t h = 0; h < er.size(); h++) {
			if (num == er[h].line) {
				laf = true;
			}
		}
		if (laf == true) {
			laf = false;
			continue;
		}
		for (size_t j = 0; j < eb.size(); j++) {
			if (eb[j].r_type == "segment" && eb[j]._beg == num) {
				seg = true;
				seg_n = vec[beg];
				_offset = 0;
				temp = j;
			}
			if (eb[j].r_type == "proc" && eb[j]._beg == num) {
				proc = true;
				temp1 = j;
			}
		}
		if (seg == true) {
			if (num <= eb[temp]._end) {
				if (eb[temp]._beg == num) {
					_offset += offs;
					offs = 0;
					transcript trans;
					trans.of_size = offs;
					trans.main_offs = _offset;
					trans.number = num;
					trans.begg = beg;
					trans.endd = end;
					tr.push_back(trans);
					cs = false;
				}
				else if (eb[temp]._end == num) {
					_offset += offs;
					eb[temp].offset = _offset;
					offs = 0;
					transcript trans;
					trans.of_size = offs;
					trans.main_offs = _offset;
					trans.number = num;
					trans.begg = beg;
					trans.endd = end;
					tr.push_back(trans);
					cs = false;
				}
				for (size_t k = 0; k < eb.size(); k++) {
					if (eb[k].r_type == "var" && eb[k]._beg == num && eb[k]._end == num) {
						for (size_t o = 0; o < constant.size();o++) {
							if (vec[end] == constant[o]) {
								eb[temp]._seg = seg_n;
								_offset += offs;
								teb.nam = vec[beg];
								teb.sg = seg_n;
								teb._offset = _offset;
								tb.push_back(teb);
								_offset += offs;
								offs = constant[o].size();
								if (con_seg == true) {
									string s_mod, st;
									string _st = vec[beg + 2];
									for (size_t a = 0;a < _st.size();a++) {
										stringstream stream;
										int ia = (int)_st[a];
										stream << hex << ia;
										st = stream.str();
										s_mod += st;
										s_mod.push_back(' ');
									}
									transcript trans;
									trans.op = s_mod;
									trans.number = num;
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);
								}
								cs = true;
							}
						}
						if (vec[beg + 1] == "DD") {
							eb[temp]._seg = seg_n;
							_offset += offs;
							teb.nam = vec[beg];
							teb.sg = seg_n;
							teb._offset = _offset;
							tb.push_back(teb);
							offs = 4;
							if (con_seg == true) {
								string s_mod;
								string _st = vec[beg + 2];
								if (type(vec[beg + 2], false, vec[beg + 2].length()) == "Hexadecimal constant") {
									if (_st.size() > 9) {
										error rr;
										rr.line = num;
										rr.n_er = "ERROR";
										string ss;
										for (int i = beg; i <= end;i++) {
											ss += vec[i];
											ss += " ";
										}
										er.push_back(rr);
									}
									for (size_t a = 0;a < _st.size();a++) {
										if (_st[a] != 'H') {
											s_mod.push_back(_st[a]);
										}
									}
									transcript trans;
									trans.op = s_mod;
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);
								}
								else {
									transcript trans;
									trans.to_end = true;
									trans.nam_ = vec[beg + 2];
									trans.typ = "con";
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);
								}
							}
							cs = false;
						}
						else if (vec[beg + 1] == "DW") {
							eb[temp]._seg = seg_n;
							_offset += offs;
							teb.nam = vec[beg];
							teb.sg = seg_n;
							teb._offset = _offset;
							tb.push_back(teb);
							offs = 2;
							if (con_seg == true) {
								string s_mod;
								string _st = vec[beg + 2];
								if (type(vec[beg + 2], false, vec[beg + 2].length()) == "Hexadecimal constant") {
									if (_st.size() > 5) {
										error rr;
										rr.line = num;
										rr.n_er = "ERROR";
										string ss;
										for (int i = beg; i <= end;i++) {
											ss += vec[i];
											ss += " ";
										}
										er.push_back(rr);
									}
									for (size_t a = 0;a < _st.size();a++) {
										if (_st[a] != 'H') {
											s_mod.push_back(_st[a]);
										}
									}
									transcript trans;
									trans.op = s_mod;
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);
								}
								else {
									transcript trans;
									trans.to_end = true;
									trans.nam_ = vec[beg + 2];
									trans.typ = "con";
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);;
								}
							}
							cs = false;
						}
						else if (vec[beg + 1] == "DB" && cs == false) {
							eb[temp]._seg = seg_n;
							_offset += offs;
							teb.nam = vec[beg];
							teb.sg = seg_n;
							teb._offset = _offset;
							tb.push_back(teb);
							offs = 1;
							if (con_seg == true) {
								string s_mod;
								string _st = vec[beg + 2];
								if (type(vec[beg + 2], false, vec[beg + 2].length()) == "Hexadecimal constant") {
									if (_st.size() > 3) {
										error rr;
										rr.line = num;
										rr.n_er = "ERROR";
										string ss;
										for (int i = beg; i <= end;i++) {
											ss += vec[i];
											ss += " ";
										}
										er.push_back(rr);
									}
									for (size_t a = 0;a < _st.size();a++) {
										if (_st[a] != 'H') {
											s_mod.push_back(_st[a]);
										}
									}
									transcript trans;
									trans.op = s_mod;
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);
								}
								else {
									for (size_t g = 0; g < eb.size();g++) {
										if (vec[beg + 2] == eb[g]._name) {
											//of_ = eb[g].offset;
											;
										}
									}
									transcript trans;
									trans.op = s_mod;
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);;
								}
							}
						}
					}
				}
			}
			for (size_t l = 0; l < eb.size();l++) {
				if (eb[l].r_type == "label" && eb[l]._beg == num) {
					_offset += offs;
					eb[l]._seg = seg_n;
					eb[l].offset = _offset;
					offs = 0;
					transcript trans;
					trans.of_size = offs;
					trans.main_offs = _offset;
					trans.number = num;
					trans.begg = beg;
					trans.endd = end;
					tr.push_back(trans);
				}
			}
		}
		if (vec[beg] == "ASSUME") {
			transcript trans;
			trans.empty = true;
			trans.number = num;
			trans.begg = beg;
			trans.endd = end;
			tr.push_back(trans);
		}
		if (proc == true) {
			if (num <= eb[temp1]._end) {
				if (eb[temp1]._beg == num) {
					_offset += offs;
					eb[temp1]._seg = seg_n;
					eb[temp1].offset = _offset;
					offs = 0;
					transcript trans;
					trans.of_size = offs;
					trans.main_offs = _offset;
					trans.number = num;
					trans.begg = beg;
					trans.endd = end;
					tr.push_back(trans);
					cs = false;
				}
				else if (eb[temp1]._end == num) {
					_offset += offs;
					offs = 0;
					transcript trans;
					trans.of_size = offs;
					trans.main_offs = _offset;
					trans.number = num;
					trans.begg = beg;
					trans.endd = end;
					tr.push_back(trans);
					cs = false;
				}
				if (vec[beg] == "MOV") {
					bool st = false, if_ss = false;
					for (size_t h = 0; h < eb.size(); h++) {
						if (vec[beg + 3] == eb[h]._name || vec[beg + 4] == eb[h]._name) {
							st = true;
						}
						if (h == eb.size() - 1 && st == false) {
								error rr;
								transcript trans;
								trans.number = num;
								trans.er_s = true;
								rr.line = num;
								rr.n_er = "ERROR";
								string ss;
								for (int i = beg; i <= end;i++) {
									ss += vec[i];
									ss += " ";
								}
								er.push_back(rr);
								continue;
						}
					}
					if ((vec[beg + 1] == "AX" || vec[beg + 1] == "AL" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI" || vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH")) {
						int q = beg + 3;
						if (end - q == 0) {
							_offset += offs;
							string nam, seg1, seg2, _seg1, _seg2;
							int b;
							bool s1 = false, s2 = false;
							transcript trans;
							for (size_t e = 0;e < sb.size(); e++) {
								if (eb[temp]._name == sb[e].name_cs) {
									nam = eb[temp]._name;
									seg1 = sb[e].name_es;
									seg2 = sb[e].name_ds;

									for (size_t r = 0; r < eb.size();r++) {
										if (eb[r]._seg == seg1) {
											b = r;
										}
										if (eb[r]._seg == seg2) {
											e = r;
										}
										if (vec[beg + 3] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
											s1 = true;
											if_ss = true;
										}
										else if (vec[beg + 3] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
											s2 = true;
											if_ss = true;
										}
									}
								}
							}
							if (s2) {
								_offset += offs;
								offs = 4;
								if (vec[beg + 1] == "AX" || vec[beg + 1] == "AL") {
									offs = 3;
								}
								if (con_seg == true) {
									int of;
									for (size_t ii = 0; ii < tb.size(); ii++) {
										if (tb[ii].nam == vec[beg+3]) {
											of = tb[ii]._offset;
											break;
										}
									}
									if ((vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH"))
										trans.op = "8A";
									else if ((vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))
										trans.op = "8B";
									trans.rm = "110";
									trans.reg = check_reg(vec[beg + 1]);
									if (vec[beg + 1] == "AL") {
										trans.op = "A0";
										trans.rm = "";
										trans.reg = "";
									}
									else if (vec[beg + 1] == "AX") {
										trans.op = "A1";
										trans.rm = "";
										trans.reg = "";
									}
									trans.off_set = of;
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);
								}
							}
							else if (s1) {
								_offset += offs;
								offs = 5;
								if (vec[beg + 1] == "AX" || vec[beg + 1] == "AL") {
									offs = 4;
								}
								if (con_seg == true) {
									int of;
									for (size_t ii = 0; ii < tb.size(); ii++) {
										if (tb[ii].nam == vec[beg + 3]) {
											of = tb[ii]._offset;
											break;
										}
									}
									trans.pref = "26";
									if ((vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH"))
										trans.op = "8A";
									else if ((vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))
										trans.op = "8B";
									trans.rm = "110";
									trans.reg = check_reg(vec[beg + 1]);
									if (vec[beg + 1] == "AL") {
										trans.op = "A0";
										trans.rm = "";
										trans.reg = "";
									}
									else if (vec[beg + 1] == "AX") {
										trans.op = "A1";
										trans.rm = "";
										trans.reg = "";
									}
									trans.off_set = of;
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);
								}
							}
						}
						else if (end - q == 1 && vec[q] == "OFFSET") {
							_offset += offs;
							offs = 3;
							if (con_seg == true) {
								int oof = 0;
								bool tril = false;
								transcript trans;
								if_ss = true;
								for (size_t ii = 0; ii < tb.size(); ii++) {
									if (tb[ii].nam == vec[end]) {
										oof = tb[ii]._offset;
										tril = true;
										break;
									}
								}
								if (tril == false) {
									for (size_t iii = 0; iii < eb.size(); iii++) {
										if (eb[iii]._name == vec[end]) {
											oof = eb[iii].offset;
											break;
										}
									}
								}
								if (vec[beg + 1] == "AL")
									trans.op = "B0";
								else if (vec[beg + 1] == "CL")
									trans.op = "B1";
								else if (vec[beg + 1] == "DL")
									trans.op = "B2";
								else if (vec[beg + 1] == "BL")
									trans.op = "B3";
								else if (vec[beg + 1] == "AH")
									trans.op = "B4";
								else if (vec[beg + 1] == "CH")
									trans.op = "B5";
								else if (vec[beg + 1] == "DH")
									trans.op = "B6";
								else if (vec[beg + 1] == "BH")
									trans.op = "B7";
								else if (vec[beg + 1] == "AX")
									trans.op = "B8";
								else if (vec[beg + 1] == "CX")
									trans.op = "B9";
								else if (vec[beg + 1] == "DX")
									trans.op = "BA";
								else if (vec[beg + 1] == "BX")
									trans.op = "BB";
								else if (vec[beg + 1] == "SP")
									trans.op = "BC";
								else if (vec[beg + 1] == "BP")
									trans.op = "BD";
								else if (vec[beg + 1] == "SI")
									trans.op = "BE";
								else if (vec[beg + 1] == "DI")
									trans.op = "BF";
								trans.off_set = oof;
								trans.of_size = offs;
								trans.main_offs = _offset;
								trans.number = num;
								trans.begg = beg;
								trans.endd = end;
								tr.push_back(trans);
							}
						}
						else if (end - q == 5) {
							if (vec[q + 1] == "[" && vec[q + 2] == "BX" && vec[q + 3] == "+" && (vec[q + 4] == "DI" || vec[q + 4] == "SI") && vec[q + 5] == "]") {
								string nam, seg1, seg2, _seg1, _seg2;
								int b;
								bool s1 = false, s2 = false;
								transcript trans;
								for (size_t e = 0;e < sb.size(); e++) {
									if (eb[temp]._name == sb[e].name_cs) {
										nam = eb[temp]._name;
										seg1 = sb[e].name_es;
										seg2 = sb[e].name_ds;

										for (size_t r = 0; r < eb.size();r++) {
											if (eb[r]._seg == seg1) {
												b = r;
											}
											if (eb[r]._seg == seg2) {
												e = r;
											}
											if (vec[beg + 3] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
												s1 = true;
												if_ss = true;
											}
											else if (vec[beg + 3] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
												s2 = true;
												if_ss = true;
												
											}
										}
									}
								}
								if (s2) {
									_offset += offs;
									offs = 4;
									if (con_seg == true) {
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == vec[beg+3]) {
												of = tb[ii]._offset;
												break;
											}
										}
										if ((vec[beg + 1] == "AL" || vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH"))
											trans.op = "8A";
										else if ((vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))
											trans.op = "8B";
										trans.mod = "10";
										if (vec[q + 4] == "SI")
											trans.rm = "000";
										else if (vec[q + 4] == "DI")
											trans.rm = "001";
										trans.reg = check_reg(vec[beg + 1]);
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
								else if (s1) {
									_offset += offs;
									offs = 5;
									if (con_seg == true) {
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == vec[beg+3]) {
												of = tb[ii]._offset;
												break;
											}
										}
										trans.pref = "26";
										if ((vec[beg + 1] == "AL" || vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH"))
											trans.op = "8A";
										else if ((vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))
											trans.op = "8B";
										trans.mod = "10";
										if (vec[q + 4] == "SI")
											trans.rm = "000";
										else if (vec[q + 4] == "DI")
											trans.rm = "001";
										trans.reg = check_reg(vec[beg + 1]);
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}

							}
							else if (vec[q + 1] == "[" && vec[q + 2] == "BP" && vec[q + 3] == "+" && (vec[q + 4] == "SI" || vec[q+4] == "DI") && vec[q + 5] == "]") {
								string nam, seg1, seg2;
								int b;
								bool s1 = false, s2 = false;
								transcript trans;
								for (size_t e = 0;e < sb.size(); e++) {
									if (eb[temp]._name == sb[e].name_cs) {
										nam = eb[temp]._name;
										seg1 = sb[e].name_es;
										seg2 = sb[e].name_ds;
										for (size_t r = 0; r < eb.size();r++) {
											if (eb[r]._seg == seg1) {
												b = r;
											}
											if (eb[r]._seg == seg2) {
												e = r;
											}
											if (vec[beg + 3] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
												s1 = true;
												if_ss = true;
												if (con_seg == true) {
													int of;
													for (size_t ii = 0; ii < tb.size(); ii++) {
														if (tb[ii].nam == eb[r]._name) {
															of = tb[ii]._offset;
															break;
														}
													}
													trans.pref = "26";
													if ((vec[beg + 1] == "AL" || vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH"))
														trans.op = "8A";
													else if ((vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))
														trans.op = "8B";
													trans.mod = "10";
													if (vec[q + 4] == "SI")
														trans.rm = "010";
													else if (vec[q + 4] == "DI")
														trans.rm = "011";
													trans.reg = check_reg(vec[beg + 1]);
													trans.off_set = of;
													trans.of_size = offs;
													trans.main_offs = _offset;
													trans.number = num;
													trans.begg = beg;
													trans.endd = end;
													tr.push_back(trans);
												}
											}
											else if (vec[beg + 3] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
												s1 = true;
												if_ss = true;
												if (con_seg == true) {
													int of;
													for (size_t ii = 0; ii < tb.size(); ii++) {
														if (tb[ii].nam == eb[r]._name) {
															of = tb[ii]._offset;
															break;
														}
													}
													trans.pref = "3E";
													if ((vec[beg + 1] == "AL" || vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH"))
														trans.op = "8A";
													else if ((vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))
														trans.op = "8B";
													trans.mod = "10";
													if (vec[q + 4] == "SI")
														trans.rm = "010";
													else if (vec[q + 4] == "DI")
														trans.rm = "011";
													trans.reg = check_reg(vec[beg + 1]);
													trans.off_set = of;
													trans.of_size = offs;
													trans.main_offs = _offset;
													trans.number = num;
													trans.begg = beg;
													trans.endd = end;
													tr.push_back(trans);
												}
											}
										}
									}
								}
								if (s2) {
									_offset += offs;
									offs = 4;
									for (size_t u = 0; u < tr.size(); u++) {
										if (tr[u].number == num) {
											tr[u].of_size = offs;
											tr[u].main_offs = _offset;
										}
									}
								}
								else if (s1) {
									_offset += offs;
									offs = 5;
									for (size_t u = 0; u < tr.size(); u++) {
										if (tr[u].number == num) {
											tr[u].of_size = offs;
											tr[u].main_offs = _offset;
										}
									}
								}
							}
							else {
								error rr;
								string li;
								rr.line = num;
								rr.n_er = "ERROR";
								for (int i = beg; i < end; i++) {
									li += vec[i];
									li += " ";
								}
								rr.row = li;
								er.push_back(rr);
								continue;
							}
						}
						else if (end - q == 3) {
							if ((vec[q + 1] == "[" && vec[q + 2] == "BP" && vec[q + 3] == "]")) {
								string nam, seg1, seg2;
								int b;
								bool s1 = false, s2 = false;
								transcript trans;
								for (size_t e = 0;e < sb.size(); e++) {
									if (eb[temp]._name == sb[e].name_cs) {
										nam = eb[temp]._name;
										seg1 = sb[e].name_es;
										seg2 = sb[e].name_ds;
										for (size_t r = 0; r < eb.size();r++) {
											if (eb[r]._seg == seg1) {
												b = r;
											}
											if (eb[r]._seg == seg2) {
												e = r;
											}
											if (vec[beg + 3] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
												s1 = true;
												if_ss = true;
												if (con_seg == true) {
													int of;
													for (size_t ii = 0; ii < tb.size(); ii++) {
														if (tb[ii].nam == eb[r]._name) {
															of = tb[ii]._offset;
															break;
														}
													}
													trans.pref = "26";
													if ((vec[beg + 1] == "AL" || vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH"))
														trans.op = "8A";
													else if ((vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))
														trans.op = "8B";
													trans.mod = "10";
													trans.rm = "110";
													trans.reg = check_reg(vec[beg + 1]);
													trans.off_set = of;
													trans.of_size = offs;
													trans.main_offs = _offset;
													trans.number = num;
													trans.begg = beg;
													trans.endd = end;
													tr.push_back(trans);
												}
											}
											else if (vec[beg + 3] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
												s1 = true;
												if_ss = true;
												if (con_seg == true) {
													int of;
													for (size_t ii = 0; ii < tb.size(); ii++) {
														if (tb[ii].nam == eb[r]._name) {
															of = tb[ii]._offset;
															break;
														}
													}
													trans.pref = "3E";
													if ((vec[beg + 1] == "AL" || vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH"))
														trans.op = "8A";
													else if ((vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))
														trans.op = "8B";
													trans.mod = "10";
													trans.rm = "110";
													trans.reg = check_reg(vec[beg + 1]);
													trans.off_set = of;
													trans.of_size = offs;
													trans.main_offs = _offset;
													trans.number = num;
													trans.begg = beg;
													trans.endd = end;
													tr.push_back(trans);
												}
											}
										}
									}
								}
								if (s2) {
									_offset += offs;
									offs = 4;
									for (size_t u = 0; u < tr.size(); u++) {
										if (tr[u].number == num) {
											tr[u].of_size = offs;
											tr[u].main_offs = _offset;
										}
									}
								}
								else if (s1) {
									_offset += offs;
									offs = 5;
									for (size_t u = 0; u < tr.size(); u++) {
										if (tr[u].number == num) {
											tr[u].of_size = offs;
											tr[u].main_offs = _offset;
										}
									}
								}
							}
							else if (((vec[q + 1] == "[" && vec[q + 2] == "DI" && vec[q + 3] == "]")) || ((vec[q + 1] == "[" && vec[q + 2] == "SI" && vec[q + 3] == "]")) || ((vec[q + 1] == "[" && vec[q + 2] == "BX" && vec[q + 3] == "]"))) {
								string nam, seg1, seg2, _seg1, _seg2;
								int b;
								bool s1 = false, s2 = false;
								transcript trans;
								for (size_t e = 0;e < sb.size(); e++) {
									if (eb[temp]._name == sb[e].name_cs) {
										nam = eb[temp]._name;
										seg1 = sb[e].name_es;
										seg2 = sb[e].name_ds;

										for (size_t r = 0; r < eb.size();r++) {
											if (eb[r]._seg == seg1) {
												b = r;
											}
											if (eb[r]._seg == seg2) {
												e = r;
											}
											if (vec[beg + 3] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
												s1 = true;
												if_ss = true;
												if (con_seg == true) {
													int of;
													for (size_t ii = 0; ii < tb.size(); ii++) {
														if (tb[ii].nam == eb[r]._name) {
															of = tb[ii]._offset;
															break;
														}
													}
													trans.pref = "26";
													if ((vec[beg + 1] == "AL" || vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH"))
														trans.op = "8A";
													else if ((vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))
														trans.op = "8B";
													trans.mod = "10";
													if (vec[q + 2] == "SI")
														trans.rm = "100";
													else if (vec[q + 2] == "DI")
														trans.rm = "101";
													else if (vec[q + 2] == "BX")
														trans.rm = "111";
													trans.reg = check_reg(vec[beg + 1]);
													trans.off_set = of;
													trans.of_size = offs;
													trans.main_offs = _offset;
													trans.number = num;
													trans.begg = beg;
													trans.endd = end;
													tr.push_back(trans);
												}
											}
											else if (vec[beg + 3] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
												s2 = true;
												if_ss = true;
												if (con_seg == true) {
													int of;
													for (size_t ii = 0; ii < tb.size(); ii++) {
														if (tb[ii].nam == eb[r]._name) {
															of = tb[ii]._offset;
															break;
														}
													}
													if ((vec[beg + 1] == "AL" || vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH"))
														trans.op = "8A";
													else if ((vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))
														trans.op = "8B";
													trans.mod = "10";
													if (vec[q + 2] == "SI")
														trans.rm = "100";
													else if (vec[q + 2] == "DI")
														trans.rm = "101";
													else if (vec[q + 2] == "BX")
														trans.rm = "111";
													trans.reg = check_reg(vec[beg + 1]);
													trans.off_set = of;
													trans.of_size = offs;
													trans.main_offs = _offset;
													trans.number = num;
													trans.begg = beg;
													trans.endd = end;
													tr.push_back(trans);
												}
											}
										}
									}
								}
								if (s2) {
									_offset += offs;
									offs = 4;
									for (size_t u = 0; u < tr.size(); u++) {
										if (tr[u].number == num) {
											tr[u].of_size = offs;
											tr[u].main_offs = _offset;
										}
									}
								}
								else if (s1) {
									_offset += offs;
									offs = 5;
									for (size_t u = 0; u < tr.size(); u++) {
										if (tr[u].number == num) {
											tr[u].of_size = offs;
											tr[u].main_offs = _offset;
										}
									}
								}
							}
							else {
								error rr;
								string li;
								rr.line = num;
								rr.n_er = "ERROR";
								for (int i = beg; i < end; i++) {
									li += vec[i];
									li += " ";
								}
								rr.row = li;
								er.push_back(rr);
								continue;
							}
						}
					}
					else {
						error rr;
						string li;
						rr.line = num;
						rr.n_er = "ERROR";
						for (int i = beg; i < end; i++) {
							li += vec[i];
							li += " ";
						}
						rr.row = li;
						er.push_back(rr);
						continue;
					}
					if (if_ss == false) {
						error rr;
						rr.line = num;
						rr.n_er = "ERROR";
						string ss;
						for (int i = beg; i <= end;i++) {
							ss += vec[i];
							ss += " ";
						er.push_back(rr);
						continue;
						}
					}
				}
				else if (vec[beg] == "JNE") {
				bool st = false;
				for (size_t h = 0; h < eb.size(); h++) {
					if (vec[beg + 1] == eb[h]._name) {
						st = true;
					}
					if (h == eb.size() - 1 && st == false) {
						error rr;
						rr.line = num;
						rr.n_er = "ERROR";
						string ss;
						for (int i = beg; i <= end;i++) {
							ss += vec[i];
							ss += " ";
						}
						er.push_back(rr);
						continue;
					}
				}
					for (size_t i = 0; i < eb.size();i++) {
						transcript trans;
						if (eb[i]._name == vec[end]) {
							if (num > eb[i]._beg) {
								_offset += offs;
								offs = 2;
								int ss;
								int s_e;
								if (con_seg == true) {
									trans.op = "75";
									for (size_t i = 0; i < eb.size(); i++) {
										if (eb[i]._name == vec[beg + 1]) {
											s_e = eb[i].offset;
											break;
										}
									}
									ss = 255 - (_offset + (s_e + 1));
									trans.jm = ss;
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);
								}
							}
							else {
								_offset += offs;
								offs = 4;
								if (con_seg == true) {
									trans.op = "75";
									trans.nam_ = vec[beg + 1];
									trans.dest = _offset;
									trans.typ = "lab";
									trans.to_end = true;
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);
								}
							}
						}
					}
				}
				else if (vec[beg] == "PUSH") {
				bool st = false, if_ss = false;
				for (size_t h = 0; h < eb.size(); h++) {
					if (vec[beg + 1] == eb[h]._name || vec[beg + 2] == eb[h]._name || ((vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))) {
						st = true;
					}
					if (h == eb.size() - 1 && st == false) {
						error rr;
						rr.line = num;
						rr.n_er = "ERROR";
						string ss;
						for (int i = beg; i <= end;i++) {
							ss += vec[i];
							ss += " ";
						}
						er.push_back(rr);
						continue;
					}
				}
					if (vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI") {
						transcript trans;
						if_ss = true;
						if (vec[beg + 1] == "AX")
							trans.op = "50";
						else if (vec[beg + 1] == "CX")
							trans.op = "51";
						else if (vec[beg + 1] == "DX")
							trans.op = "52";
						else if (vec[beg + 1] == "BX")
							trans.op = "53";
						else if (vec[beg + 1] == "SP")
							trans.op = "54";
						else if (vec[beg + 1] == "BP")
							trans.op = "55";
						else if (vec[beg + 1] == "SI")
							trans.op = "56";
						else if (vec[beg + 1] == "DI")
							trans.op = "57";
						trans.of_size = offs;
						trans.main_offs = _offset;
						trans.number = num;
						trans.begg = beg;
						trans.endd = end;
						tr.push_back(trans);
					}
					else if (vec[beg + 1] == "OFFSET") {
						_offset += offs;
						if_ss = true;
						if (con_seg == true) {
							offs = 3;
							if (con_seg == true) {
								int oof = 0;
								bool tril = false;
								transcript trans;
								for (size_t ii = 0; ii < tb.size(); ii++) {
									if (tb[ii].nam == vec[end]) {
										oof = tb[ii]._offset;
										tril = true;
										break;
									}
								}
								if (tril == false) {
									for (size_t iii = 0; iii < eb.size(); iii++) {
										if (eb[iii]._name == vec[end]) {
											oof = eb[iii].offset;
											break;
										}
									}
								}
						
								trans.op = "68";
								trans.off_set = oof;
								trans.of_size = offs;
								trans.main_offs = _offset;
								trans.number = num;
								trans.begg = beg;
								trans.endd = end;
								tr.push_back(trans);
							}
						}
					}
					string nam, seg1, seg2, _seg1, _seg2;
					int b;
					bool s1 = false, s2 = false;
					transcript trans;
					for (size_t e = 0;e < sb.size(); e++) {
						if (eb[temp]._name == sb[e].name_cs) {
							nam = eb[temp]._name;
							seg1 = sb[e].name_es;
							seg2 = sb[e].name_ds;

							for (size_t r = 0; r < eb.size();r++) {
								if (eb[r]._seg == seg1) {
									b = r;
								}
								if (eb[r]._seg == seg2) {
									e = r;
								}
								if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
									s1 = true;
									if_ss = true;
									if (con_seg == true) {
										int pos;
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == eb[r]._name) {
												of = tb[ii]._offset;
												pos = r;
												break;
											}
										}
										trans.pref = "26";
										trans.op = "FF";
										trans.rm = "110";
										trans.reg = "110";
										if (eb[pos]._type == "Dword")
											trans.b_over = "66";
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
								else if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
									s2 = true;
									if_ss = true;
									if (con_seg == true) {
										int of;
										int pos;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == eb[r]._name) {
												of = tb[ii]._offset;
												pos = r;
												break;
											}
										}
										trans.op = "FF";
										trans.rm = "110";
										trans.reg = "110";
										if (eb[pos]._type == "Dword")
											trans.b_over = "66";
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
							}
						}
					}
					if (s2) {
						_offset += offs;
						offs = 4;
						for (size_t u = 0; u < tr.size(); u++) {
							if (tr[u].number == num) {
								tr[u].of_size = offs;
								tr[u].main_offs = _offset;
							}
						}
					}
					else if (s1) {
						_offset += offs;
						offs = 5;
						for (size_t u = 0; u < tr.size(); u++) {
							if (tr[u].number == num) {
								tr[u].of_size = offs;
								tr[u].main_offs = _offset;
							}
						}
					}
					if (if_ss == false) {
						error rr;
						rr.line = num;
						rr.n_er = "ERROR";
						string ss;
						for (int i = beg; i <= end;i++) {
							ss += vec[i];
							ss += " ";
						}
						er.push_back(rr);
						continue;
					}
				}
				else if (beg == end && vec[beg] == "RET") {
					_offset += offs;
					offs = 1;
					if (con_seg == true) {
						transcript trans;
						trans.op = "C3";
						trans.of_size = offs;
						trans.main_offs = _offset;
						trans.number = num;
						trans.begg = beg;
						trans.endd = end;
						tr.push_back(trans);
					}
				}
				else if (vec[beg] == "POP") {
				bool st = false, if_ss = false;
				for (size_t h = 0; h < eb.size(); h++) {
					if (vec[beg + 1] == eb[h]._name) {
						st = true;
					}
					if (h == eb.size() - 1 && st == false) {
						error rr;
						rr.line = num;
						rr.n_er = "ERROR";
						string ss;
						for (int i = beg; i <= end;i++) {
							ss += vec[i];
							ss += " ";
						}
						er.push_back(rr);
						continue;
					}
				}
				if (vec[beg + 2] == "[" && vec[beg + 3] == "BX" && vec[beg + 4] == "+" && (vec[beg + 5] == "DI" || vec[beg + 5] == "SI") && vec[beg + 6] == "]") {
					string nam, seg1, seg2, _seg1, _seg2;
					int b;
					bool s1 = false, s2 = false;
					transcript trans;
					for (size_t e = 0;e < sb.size(); e++) {
						if (eb[temp]._name == sb[e].name_cs) {
							nam = eb[temp]._name;
							seg1 = sb[e].name_es;
							seg2 = sb[e].name_ds;

							for (size_t r = 0; r < eb.size();r++) {
								if (eb[r]._seg == seg1) {
									b = r;
								}
								if (eb[r]._seg == seg2) {
									e = r;
								}
								if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
									s1 = true;
									if_ss = true;
									if (con_seg == true) {
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == eb[r]._name) {
												of = tb[ii]._offset;
												break;
											}
										}
										trans.pref = "26";
										trans.op = "8F";
										trans.mod = "10";
										if (vec[beg + 5] == "SI")
											trans.rm = "000";
										else if (vec[beg + 5] == "DI")
											trans.rm = "001";
										trans.reg = "000";
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
								else if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
									s2 = true;
									if_ss = true;
									if (con_seg == true) {
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == eb[r]._name) {
												of = tb[ii]._offset;
												break;
											}
										}
										trans.op = "8F";
										trans.mod = "10";
										if (vec[beg + 5] == "SI")
											trans.rm = "000";
										else if (vec[beg + 5] == "DI")
											trans.rm = "001";
										trans.reg = "000";;
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
							}
						}
					}
					if (s2) {
						_offset += offs;
						offs = 4;
						for (size_t u = 0; u < tr.size(); u++) {
							if (tr[u].number == num) {
								tr[u].of_size = offs;
								tr[u].main_offs = _offset;
							}
						}
					}
					else if (s1) {
						_offset += offs;
						offs = 5;
						for (size_t u = 0; u < tr.size(); u++) {
							if (tr[u].number == num) {
								tr[u].of_size = offs;
								tr[u].main_offs = _offset;
							}
						}
					}

				}
				else if (vec[beg + 2] == "[" && vec[beg + 3] == "BP" && vec[beg + 4] == "+" && (vec[beg + 5] == "SI" || vec[beg + 5] == "DI") && vec[beg + 6] == "]") {
					string nam, seg1, seg2;
					int b;
					bool s1 = false, s2 = false;
					transcript trans;
					for (size_t e = 0;e < sb.size(); e++) {
						if (eb[temp]._name == sb[e].name_cs) {
							nam = eb[temp]._name;
							seg1 = sb[e].name_es;
							seg2 = sb[e].name_ds;
							for (size_t r = 0; r < eb.size();r++) {
								if (eb[r]._seg == seg1) {
									b = r;
								}
								if (eb[r]._seg == seg2) {
									e = r;
								}
								if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
									s1 = true;
									if_ss = true;
									if (con_seg == true) {
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == eb[r]._name) {
												of = tb[ii]._offset;
												break;
											}
										}
										trans.pref = "26";
										trans.op = "8F";
										trans.mod = "10";
										if (vec[beg + 5] == "SI")
											trans.rm = "010";
										else if (vec[beg + 5] == "DI")
											trans.rm = "011";
										trans.reg = "000";
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
								else if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
									s1 = true;
									if_ss = true;
									if (con_seg == true) {
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == eb[r]._name) {
												of = tb[ii]._offset;
												break;
											}
										}
										trans.pref = "3E";
										trans.op = "8F";
										trans.mod = "10";
										if (vec[beg + 5] == "SI")
											trans.rm = "010";
										else if (vec[beg + 5] == "DI")
											trans.rm = "011";
										trans.reg = "000";
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
							}
						}
					}
					if (s2) {
						_offset += offs;
						offs = 4;
						for (size_t u = 0; u < tr.size(); u++) {
							if (tr[u].number == num) {
								tr[u].of_size = offs;
								tr[u].main_offs = _offset;
							}
						}
					}
					else if (s1) {
						_offset += offs;
						offs = 5;
						for (size_t u = 0; u < tr.size(); u++) {
							if (tr[u].number == num) {
								tr[u].of_size = offs;
								tr[u].main_offs = _offset;
							}
						}
					}
				}
				else if ((vec[beg + 2] == "[" && vec[beg + 3] == "BP" && vec[beg + 4] == "]")) {
					string nam, seg1, seg2;
					int b;
					bool s1 = false, s2 = false;
					transcript trans;
					for (size_t e = 0;e < sb.size(); e++) {
						if (eb[temp]._name == sb[e].name_cs) {
							nam = eb[temp]._name;
							seg1 = sb[e].name_es;
							seg2 = sb[e].name_ds;
							for (size_t r = 0; r < eb.size();r++) {
								if (eb[r]._seg == seg1) {
									b = r;
								}
								if (eb[r]._seg == seg2) {
									e = r;
								}
								if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
									s1 = true;
									if_ss = true;
									if (con_seg == true) {
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == eb[r]._name) {
												of = tb[ii]._offset;
												break;
											}
										}
										trans.pref = "26";
										trans.op = "8F";
										trans.mod = "10";
										trans.rm = "110";
										trans.reg = "000";
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
								else if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
									s1 = true;
									if_ss = true;
									if (con_seg == true) {
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == eb[r]._name) {
												of = tb[ii]._offset;
												break;
											}
										}
										trans.pref = "3E";
										trans.op = "8F";
										trans.mod = "10";
										trans.rm = "110";
										trans.reg = "000";
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
							}
						}
					}
					if (s2) {
						_offset += offs;
						offs = 4;
						for (size_t u = 0; u < tr.size(); u++) {
							if (tr[u].number == num) {
								tr[u].of_size = offs;
								tr[u].main_offs = _offset;
							}
						}
					}
					else if (s1) {
						_offset += offs;
						offs = 5;
						for (size_t u = 0; u < tr.size(); u++) {
							if (tr[u].number == num) {
								tr[u].of_size = offs;
								tr[u].main_offs = _offset;
							}
						}
					}
				}
				else if (((vec[beg + 2] == "[" && vec[beg + 3] == "DI" && vec[beg + 4] == "]")) || ((vec[beg + 2] == "[" && vec[beg + 3] == "SI" && vec[beg + 4] == "]")) || ((vec[beg + 2] == "[" && vec[beg + 3] == "BX" && vec[beg + 4] == "]"))) {
					string nam, seg1, seg2, _seg1, _seg2;
					int b;
					bool s1 = false, s2 = false;
					transcript trans;
					for (size_t e = 0;e < sb.size(); e++) {
						if (eb[temp]._name == sb[e].name_cs) {
							nam = eb[temp]._name;
							seg1 = sb[e].name_es;
							seg2 = sb[e].name_ds;

							for (size_t r = 0; r < eb.size();r++) {
								if (eb[r]._seg == seg1) {
									b = r;
								}
								if (eb[r]._seg == seg2) {
									e = r;
								}
								if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
									s1 = true;
									if_ss = true;
									if (con_seg == true) {
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == eb[r]._name) {
												of = tb[ii]._offset;
												break;
											}
										}
										trans.pref = "26";
										trans.op = "8F";
										trans.mod = "10";
										if (vec[beg + 3] == "SI")
											trans.rm = "100";
										else if (vec[beg + 3] == "DI")
											trans.rm = "101";
										else if (vec[beg + 3] == "BX")
											trans.rm = "111";
										trans.reg = "000";
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
								else if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
									s2 = true;
									if_ss = true;
									if (con_seg == true) {
										int of;
										for (size_t ii = 0; ii < tb.size(); ii++) {
											if (tb[ii].nam == eb[r]._name) {
												of = tb[ii]._offset;
												break;
											}
										}
										trans.op = "8F";
										trans.mod = "10";
										if (vec[beg + 3] == "SI")
											trans.rm = "100";
										else if (vec[beg + 3] == "DI")
											trans.rm = "101";
										else if (vec[beg + 3] == "BX")
											trans.rm = "111";
										trans.reg = "000";
										trans.off_set = of;
										trans.of_size = offs;
										trans.main_offs = _offset;
										trans.number = num;
										trans.begg = beg;
										trans.endd = end;
										tr.push_back(trans);
									}
								}
							}
						}
					}
					if (s2) {
						_offset += offs;
						offs = 4;
						for (size_t u = 0; u < tr.size(); u++) {
							if (tr[u].number == num) {
								tr[u].of_size = offs;
								tr[u].main_offs = _offset;
							}
						}
					}
					else if (s1) {
						_offset += offs;
						offs = 5;
						for (size_t u = 0; u < tr.size(); u++) {
							if (tr[u].number == num) {
								tr[u].of_size = offs;
								tr[u].main_offs = _offset;
							}
						}
					}
				}
				else {
					error rr;
					string li;
					rr.line = num;
					rr.n_er = "ERROR";
					for (int i = beg; i < end; i++) {
						li += vec[i];
						li += " ";
					}
					rr.row = li;
					er.push_back(rr);
				}
				if (if_ss == false) {
					error rr;
					rr.line = num;
					rr.n_er = "ERROR";
					string ss;
					for (int i = beg; i <= end;i++) {
						ss += vec[i];
						ss += " ";
					}
					er.push_back(rr);
					continue;
				}
				}
				else if (vec[beg] == "CALL") {
				bool st = false, if_ss = false;
				for (size_t h = 0; h < eb.size(); h++) {
					if (vec[beg + 1] == eb[h]._name || ((vec[beg + 1] == "AX" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI"))) {
						st = true;
					}
					if (h == eb.size() - 1 && st == false) {
						error rr;
						rr.line = num;
						rr.n_er = "ERROR";
						string ss;
						for (int i = beg; i <= end;i++) {
							ss += vec[i];
							ss += " ";
						}
						er.push_back(rr);
						continue;
					}
				}
					if ((vec[beg + 1] == "AX" || vec[beg + 1] == "AL" || vec[beg + 1] == "BX" || vec[beg + 1] == "CX" || vec[beg + 1] == "DX" || vec[beg + 1] == "SP" || vec[beg + 1] == "BP" || vec[beg + 1] == "SI" || vec[beg + 1] == "DI" || vec[beg + 1] == "BL" || vec[beg + 1] == "CL" || vec[beg + 1] == "DL" || vec[beg + 1] == "CH" || vec[beg + 1] == "AH" || vec[beg + 1] == "BH" || vec[beg + 1] == "DH")) {
						transcript trans;
						_offset += offs;
						offs = 2;
						if_ss = true;
						if (con_seg == true) {
							trans.op = "FF";
							trans.mod = "11";
							trans.reg = "010";
							trans.rm = check_reg(vec[beg + 1]);
							trans.of_size = offs;
							trans.main_offs = _offset;
							trans.number = num;
							trans.begg = beg;
							trans.endd = end;
							tr.push_back(trans);
						}
					}
					if ((vec[beg + 2] == "[" && vec[beg + 3] == "BX" && vec[beg + 4] == "+" && (vec[beg + 5] == "DI" || vec[beg + 5] == "SI") && vec[beg + 6] == "]") || (vec[beg + 2] == "[" && vec[beg + 3] == "DI" && vec[beg + 4] == "]") || (vec[beg + 2] == "[" && vec[beg + 3] == "BX" && vec[beg + 4] == "]") || (vec[beg + 2] == "[" && vec[beg + 3] == "SI" && vec[beg + 4] == "]")) {
						string nam, seg1, seg2, _seg1, _seg2;
						int b;
						bool s1 = false, s2 = false;
						transcript trans;
						for (size_t e = 0;e < sb.size(); e++) {
							if (eb[temp]._name == sb[e].name_cs) {
								nam = eb[temp]._name;
								seg1 = sb[e].name_es;
								seg2 = sb[e].name_ds;

								for (size_t r = 0; r < eb.size();r++) {
									if (eb[r]._seg == seg1) {
										b = r;
									}
									if (eb[r]._seg == seg2) {
										e = r;
									}
									if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
										s1 = true;
										if_ss = true;
										if (con_seg == true) {
											int of;
											for (size_t ii = 0; ii < tb.size(); ii++) {
												if (tb[ii].nam == eb[r]._name) {
													of = tb[ii]._offset;
													break;
												}
											}
											trans.pref = "26";
											trans.op = "FF";
							
											trans.mod = "10";
											if (vec[beg + 5] == "SI")
												trans.rm = "000";
											else if (vec[beg + 5] == "DI")
												trans.rm = "001";
											else if (vec[beg + 3] == "DI")
												trans.rm = "101";
											else if (vec[beg + 3] == "SI")
												trans.rm = "100";
											else if (vec[beg + 3] == "BX")
												trans.rm = "111";
											if(eb[r]._type == "Dword")
												trans.reg = "011";
											else if (eb[r]._type == "Word")
												trans.reg = "010";
											trans.off_set = of;
											trans.of_size = offs;
											trans.main_offs = _offset;
											trans.number = num;
											trans.begg = beg;
											trans.endd = end;
											tr.push_back(trans);
										}
									}
									else if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
										s2 = true;
										if_ss = true;
										if (con_seg == true) {
											int of;
											for (size_t ii = 0; ii < tb.size(); ii++) {
												if (tb[ii].nam == eb[r]._name) {
													of = tb[ii]._offset;
													break;
												}
											}
											trans.op = "FF";
											trans.mod = "10";
											if (vec[beg + 5] == "SI")
												trans.rm = "000";
											else if (vec[beg + 5] == "DI")
												trans.rm = "001";
											else if (vec[beg + 3] == "DI")
												trans.rm = "101";
											else if (vec[beg + 3] == "SI")
												trans.rm = "100";
											else if (vec[beg + 3] == "BX")
												trans.rm = "111";
											if (eb[r]._type == "Dword")
												trans.reg = "011";
											else if (eb[r]._type == "Word")
												trans.reg = "010";
											trans.off_set = of;
											trans.of_size = offs;
											trans.main_offs = _offset;
											trans.number = num;
											trans.begg = beg;
											trans.endd = end;
											tr.push_back(trans);
										}
									}
								}
							}
						}
						if (s2) {
							_offset += offs;
							offs = 4;
							for (size_t u = 0; u < tr.size(); u++) {
								if (tr[u].number == num) {
									tr[u].of_size = offs;
									tr[u].main_offs = _offset;
								}
							}
						}
						else if (s1) {
							_offset += offs;
							offs = 5;
							for (size_t u = 0; u < tr.size(); u++) {
								if (tr[u].number == num) {
									tr[u].of_size = offs;
									tr[u].main_offs = _offset;
								}
							}
						}

					}
					else if ((vec[beg + 2] == "[" && vec[beg + 3] == "BP" && vec[beg + 4] == "+" && (vec[beg + 5] == "SI" || vec[beg + 5] == "DI") && vec[beg + 6] == "]") || (vec[beg + 2] == "[" && vec[beg + 3] == "BP" && vec[beg + 4] == "]")) {
						string nam, seg1, seg2;
						int b;
						bool s1 = false, s2 = false;
						transcript trans;
						for (size_t e = 0;e < sb.size(); e++) {
							if (eb[temp]._name == sb[e].name_cs) {
								nam = eb[temp]._name;
								seg1 = sb[e].name_es;
								seg2 = sb[e].name_ds;
								for (size_t r = 0; r < eb.size();r++) {
									if (eb[r]._seg == seg1) {
										b = r;
									}
									if (eb[r]._seg == seg2) {
										e = r;
									}
									if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
										s1 = true;
										if_ss = true;
										if (con_seg == true) {
											int of;
											for (size_t ii = 0; ii < tb.size(); ii++) {
												if (tb[ii].nam == eb[r]._name) {
													of = tb[ii]._offset;
													break;
												}
											}
											trans.pref = "26";
											trans.op = "FF";
											trans.mod = "10";
											if (vec[beg + 5] == "SI")
												trans.rm = "010";
											else if (vec[beg + 5] == "DI")
												trans.rm = "011";
											else if (vec[beg + 3] == "BP")
												trans.rm = "110";
											if (eb[r]._type == "Dword")
												trans.reg = "011";
											else if (eb[r]._type == "Word")
												trans.reg = "010";
											trans.off_set = of;
											trans.of_size = offs;
											trans.main_offs = _offset;
											trans.number = num;
											trans.begg = beg;
											trans.endd = end;
											tr.push_back(trans);
										}
									}
									else if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
										s1 = true;
										if_ss = true;
										if (con_seg == true) {
											int of;
											for (size_t ii = 0; ii < tb.size(); ii++) {
												if (tb[ii].nam == eb[r]._name) {
													of = tb[ii]._offset;
													break;
												}
											}
											trans.pref = "3E";
											trans.op = "FF";
											trans.mod = "10";
											if (vec[beg + 5] == "SI")
												trans.rm = "010";
											else if (vec[beg + 5] == "DI")
												trans.rm = "011";
											else if (vec[beg + 3] == "BP")
												trans.rm = "110";
											if (eb[r]._type == "Dword")
												trans.reg = "011";
											else if (eb[r]._type == "Word")
												trans.reg = "010";
											trans.off_set = of;
											trans.of_size = offs;
											trans.main_offs = _offset;
											trans.number = num;
											trans.begg = beg;
											trans.endd = end;
											tr.push_back(trans);
										}
									}
								}
							}
						}
						if (s2) {
							_offset += offs;
							offs = 4;
							for (size_t u = 0; u < tr.size(); u++) {
								if (tr[u].number == num) {
									tr[u].of_size = offs;
									tr[u].main_offs = _offset;
								}
							}
						}
						else if (s1) {
							_offset += offs;
							offs = 5;
							for (size_t u = 0; u < tr.size(); u++) {
								if (tr[u].number == num) {
									tr[u].of_size = offs;
									tr[u].main_offs = _offset;
								}
							}
						}
					}
					else {
						for (size_t i = 0;i < eb.size(); i++) {
							if (vec[beg + 1] == eb[i]._name && (eb[i].r_type == "proc" || eb[i].r_type == "label")) {
								_offset += offs;
								offs = 3;
								if_ss = true;
								if (con_seg == true) {
									transcript trans;
									int s_e, ss;
									trans.op = "E8";
									for (size_t i = 0; i < eb.size(); i++) {
										if (eb[i]._name == vec[beg + 1]) {
											s_e = eb[i].offset;
											break;
										}
									}
									ss = 65535 - (_offset + (s_e + 2));
									trans.jm = ss;
									trans.of_size = offs;
									trans.main_offs = _offset;
									trans.number = num;
									trans.begg = beg;
									trans.endd = end;
									tr.push_back(trans);
								}
							}
							else if (vec[beg + 1] == eb[i]._name && (eb[i].r_type == "var")) {
								string nam, seg1, seg2, _seg1, _seg2;
								int b;
								bool s1 = false, s2 = false;
								transcript trans;
								for (size_t e = 0;e < sb.size(); e++) {
									if (eb[temp]._name == sb[e].name_cs) {
										nam = eb[temp]._name;
										seg1 = sb[e].name_es;
										seg2 = sb[e].name_ds;

										for (size_t r = 0; r < eb.size();r++) {
											if (eb[r]._seg == seg1) {
												b = r;
											}
											if (eb[r]._seg == seg2) {
												e = r;
											}
											if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[b]._beg&& eb[r]._beg < eb[b]._end)) {
												s1 = true;
												if_ss = true;
												if (con_seg == true) {
													int pos;
													int of;
													for (size_t ii = 0; ii < tb.size(); ii++) {
														if (tb[ii].nam == eb[r]._name) {
															of = tb[ii]._offset;
															pos = r;
															break;
														}
													}
													trans.pref = "26";
													trans.op = "FF";
													trans.rm = "110";
													if (eb[pos]._type == "Dword")
														trans.reg = "011";
													else if (eb[pos]._type == "Word")
														trans.reg = "010";
													trans.off_set = of;
													trans.of_size = offs;
													trans.main_offs = _offset;
													trans.number = num;
													trans.begg = beg;
													trans.endd = end;
													tr.push_back(trans);

												}
											}
											else if (vec[beg + 1] == eb[r]._name && (eb[r]._beg > eb[e]._beg&& eb[r]._beg < eb[e]._end)) {
												s2 = true;
												if_ss = true;
												if (con_seg == true) {
													int of;
													int pos;
													for (size_t ii = 0; ii < tb.size(); ii++) {
														if (tb[ii].nam == eb[r]._name) {
															of = tb[ii]._offset;
															pos = r;
															break;
														}
													}
													trans.op = "FF";
													trans.rm = "110";
													if (eb[pos]._type == "Dword")
														trans.reg = "011";
													else if (eb[pos]._type == "Word")
														trans.reg = "010";
													trans.off_set = of;
													trans.of_size = offs;
													trans.main_offs = _offset;
													trans.number = num;
													trans.begg = beg;
													trans.endd = end;
													tr.push_back(trans);
												}
											}
										}
									}
								}
								if (s2) {
									_offset += offs;
									offs = 4;
									for (size_t u = 0; u < tr.size(); u++) {
										if (tr[u].number == num) {
											tr[u].of_size = offs;
											tr[u].main_offs = _offset;
										}
									}
								}
								else if (s1) {
									_offset += offs;
									offs = 5;
									for (size_t u = 0; u < tr.size(); u++) {
										if (tr[u].number == num) {
											tr[u].of_size = offs;
											tr[u].main_offs = _offset;
										}
									}
								}
							}
						}
					}
					if (if_ss == false) {
						error rr;
						rr.line = num;
						rr.n_er = "ERROR";
						string ss;
						for (int i = beg; i <= end;i++) {
							ss += vec[i];
							ss += " ";
							er.push_back(rr);
							continue;
						}
					}
				}
			}
			if (vec[beg] == "END") {
				transcript trans;
				trans.number = num;
				trans.begg = beg;
				trans.endd = end;
				trans.empty = true;
				tr.push_back(trans);
			}
		}
		if (i == vec.size() - 1) {
			for (size_t k = 0; k < tr.size(); k++) {
				int ss, s_e;
				if (tr[k].to_end == true) {
					if (tr[k].typ == "lab") {
						for (size_t iq = 0; iq < eb.size(); iq++) {
							if (eb[iq]._name == tr[k].nam_) {
								s_e = eb[iq].offset;
								break;
							}
						}
						ss = s_e - (tr[k].dest + 2);
						tr[k].jm = ss;
						tr[k].j_m = "90 90";
					}
					else if (tr[k].typ == "con") {
						for (size_t iq = 0; iq < eb.size(); iq++) {
							if (eb[iq]._name == tr[k].nam_) {
								s_e = eb[iq].offset;
								break;
							}
						}
						tr[k].off_set = s_e;
					}
				}
			}
		}
	}
}