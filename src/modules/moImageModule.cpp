/***********************************************************************
 ** Copyright (C) 2010 Movid Authors.  All rights reserved.
 **
 ** This file is part of the Movid Software.
 **
 ** This file may be distributed under the terms of the Q Public License
 ** as defined by Trolltech AS of Norway and appearing in the file
 ** LICENSE included in the packaging of this file.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** Contact info@movid.org if any conditions of this licensing are
 ** not clear to you.
 **
 **********************************************************************/


#include <assert.h>

#include "../moLog.h"
#include "../moModule.h"
#include "../moDataStream.h"
#include "moImageModule.h"
#include "highgui.h"

MODULE_DECLARE(Image, "native", "Use a static image as source");

moImageModule::moImageModule() : moModule(MO_MODULE_OUTPUT, 0, 1) {

	MODULE_INIT();

	this->image = NULL;
	this->stream = new moDataStream("IplImage");

	// declare outputs
	this->output_infos[0] = new moDataStreamInfo(
			"image", "IplImage", "Image stream on a static image");

	// declare properties
	this->properties["filename"] = new moProperty("");
}

moImageModule::~moImageModule() {
}

void moImageModule::start() {
	if ( this->image != NULL )
		this->stop();

	moModule::start();

	this->image = cvLoadImage(this->property("filename").asString().c_str());
	if ( this->image == NULL ) {
		LOGM(MO_ERROR) << "could not load image: " << this->property("filename").asString();
		this->setError("unable to load image");
	}
}

void moImageModule::stop() {
	moModule::stop();
	if ( this->image != NULL ) {
		LOGM(MO_TRACE) << "release Image";
		cvReleaseImage((IplImage **)this->image);
		this->image = NULL;
	}
}

void moImageModule::update() {
	if ( this->image != NULL ) {
		// push a new image on the stream
		LOGM(MO_TRACE) << "push a new image on the stream";
		this->stream->push(this->image);
	}
}

void moImageModule::setInput(moDataStream* input, int n) {
	this->setError("no input supported");
}

moDataStream* moImageModule::getOutput(int n) {
	if ( n != 0 ) {
		this->setError("Invalid output index");
		return NULL;
	}
	return this->stream;
}

moDataStream* moImageModule::getInput(int n) {
	return NULL;
}

