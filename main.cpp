#include "compilerSettings.h"


int main(int argc, char* argv[]) {
	string file_path = argv[argc - 1];

	Compiler compiler_instance;
	// open the test file to read its content
	compiler_instance.read_f(file_path);
 
	// formation of the identifiers table
	// There are actually three options of where to output the identifier table
	// 1 - STDOUT (Console); 2 - FILEOUT (txt FILE "IdnetifiersTable"); 3 - NOTOUT (No OUTPUT)
	// Should not run simultaneously with the methods calls below
	//compiler_instance.line_read(FILEOUT);
	
	// Determination of the number of bytes of the machine instruction with the False boolean of the first argument
	//compiler_instance.compose_full_compiled_file(false, FILEOUT); 
	// Numbers of byets plus generation of processor commands with the True boolean of the first argument
	compiler_instance.compose_full_compiled_file(true, FILEOUT); 

	return 0;
}