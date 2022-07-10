#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <bitset>


using namespace std;

typedef enum OUTPUT {NOTOUT, STDOUT, FILEOUT} OUTPUT;

const string identifiersTableFilePath = "data/IdentifiersTable.txt";
const string listingFilePath = "data/ASM_listing.lst";

struct uName
{
	string _name;
	string _type;
	string r_type;
	string _seg ;
	string value;
	int offset;
	int _beg = -1;
	int _end = -1;
};
struct tab 
{
	int _offset;
	int num;
	string nam;
	string sg;
	int bg;
	int ed;
};
struct transcript
{
	string pref;
	string b_over;
	string op;
	string mod_r_m;
	string mod;
	string reg;
	string rm;
	int off_set = -1;
	int number;
	bool er_s = false;
	int jm = -1;
	string j_m;
	string nam_;
	int dest;
	string typ;
	string e_type;
	string row_;
	bool to_end = false;
	bool empty = false;
	int of_size;
	int main_offs;
	int begg;
	int endd;
};
struct error {
	string n_er;
	string row;
	int line = -1;
};
struct segDest
{
	string name_cs;
	string name_es;
	string name_ss;
	string name_gs;
	string name_fs;
	string name_ds;
	string seg_cs;
	string seg_es;
	string seg_ss;
	string seg_gs;
	string seg_fs;
	string seg_ds;
};

class Compiler {
public:
	// reading the file that contains the text of the test file
	void read_f(string name);
	// Check if the given file is not empty
	int not_open();
	/*
	Checks for admissibility and legitimacy of tokens, words and symbols. 
	If errors are found, they will be displayed when printing the listing in this line. 
	Also, this function is the basis of lexical analysis where each character is checked for validity. 
	Words of different lengths are formed, the type of words is checked by type(string w, bool s_const, size_t len), 
	and returns the type we need. At the user's request, the token table is displayed, or the analysis is performed without it. 
	At the same time, having finished the analysis of the sentence, it is possible to display a table of the structure of the sentence, 
	the implementation of which is in the function lex_par(vector<string> lex).
	*/
	void analize(string word, bool lex_part, bool stg, stringstream &ss);
	// Outputs the main structure of the token table
	void print_header(stringstream &ss);
	// Reads a text file and sequentially takes data from the test file and forwards them for analysis.
	void line_read(OUTPUT out_parameter);
	// Gets the parsed words and checks the token's data type. If the words are correct, it returns the name
	// of the type, otherwise it returns an error message that will be taken into account when creating the listing.
	string type(string w, bool s_const, size_t len);
	// Gets parsed lexemes and outputs data for one lexeme at each call, forming a lexical analysis table.
	void print_title(size_t num, string lex, size_t len, string descr, stringstream &ss);
	// This function receives the processed sentence. As a result, verification and formation of data to the sentence structure table is carried out.
	void lex_par(vector<string> lex, stringstream &ss);
	// Outputs a sentence structure table based on the received data for each field.
	void print_lex_part(int n_field, int f_mnem, int l_mnem, int f_op, int f_op_len, int s_op, int s_op_len, int t_op, int t_op_len, stringstream &ss);
	// A check is performed for the presence of segments, the repetition of the name of the identifier, and the presence of correct opening and closing of the segment.
	void seg_analize(vector<string> vec);
	// A check is performed for the correctness of the names of the identifiers and the correct setting of the segment registers, and the belonging of each segment name to the segment register is also remembered.
	void assume_analize(vector<string> vec, bool pr, stringstream &ss);
	// Constants and labels are checked for repeatability. Data is also saved for later use.
	void var_analize(vector<string> vec);
	// Outputs a table of segment registers
	void print_assume(string cs, string ds, string ss, string es, string gs, string fs, stringstream &strs);
	// Outputs a table of segments
	void print_seg(stringstream &ss);
	// Scans existing procedures for errors and remembers their names and locations for further printing of information in the identification plate
	void proc_analize(vector<string> vec);
	// This module deals with the calculation of the sizes of each machine command, their offset. Numbers lines and forms command bytes, checks for correctness of data and commands
	void size_count(vector<string> vec);
	// Collects all data and outputs a file listing, as well as a table of segment registers, a table of segments and a table of identifiers
	void compose_full_compiled_file(bool is_, OUTPUT out_parameter);
	// Outputs the identifier table data
	void pr_use(stringstream &ss);
	// Structures and formats data, forms a table in which it displays a part of the listing consisting of command lines, command bytes, 
	// displacement bytes and size bytes, as well as line numbering, and reporting data on the presence of errors in the code.
	void pr_table(bool stga, stringstream &ss);
	// Supplements and structures data about program errors.
	void er_check();

private:
	vector<string> lex_em, segm, _var, lab, constant;
	vector<uName> eb;
	vector<segDest> sb;
	vector<tab> tb;
	vector<error> er;
	vector<transcript> tr;
	int beg_seg = -1, end_seg = -1, num_of=0;
	int Active_seg = 0, _offset = 0, numm = 0;
	bool act = false;
	string line_f;
	string line;
	ifstream my_f;
	ofstream identifiersTable;
};