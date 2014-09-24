/*
 * prerequisites.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_PREREQUISITES_H_
#define threescanner_PREREQUISITES_H_

#include <memory>

namespace threescanner {

class Config;
class Engine;
class ImageInput;
class Projector;
class Quad;
class Scanner;

typedef std::shared_ptr<Engine> EnginePtr;
typedef std::shared_ptr<ImageInput> ImageInputPtr;
typedef std::shared_ptr<Projector> ProjectorPtr;
typedef std::shared_ptr<Scanner> ScannerPtr;

}

#endif /* threescanner_PREREQUISITES_H_ */
