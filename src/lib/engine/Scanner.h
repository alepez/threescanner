/*
 * Scanner.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_SCANNER_H_
#define threescanner_SCANNER_H_

namespace threescanner {

class Scanner {
public:
	Scanner();
	virtual ~Scanner();
	void run();
};

} /* namespace threescanner */

#endif /* threescanner_SCANNER_H_ */
