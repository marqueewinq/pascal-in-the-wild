FEATURES:
	* currently we accept only integer and double variable type;
	* the longest variable name may be 255 chars long;
	* currently we do not accept procedures/functions AT ALL;
	* according to canonical Pascal syntax, the last statement before END token should not be terminated with ';' symbol, e.x.:
		...
			func;
			func  // notice no linebreak symbol
		end;
	* ah, yes, curretly comments are disabled
	* write() built-in prints a EOL symbol at the end of your output
	* write() built-in accepts only one argument

