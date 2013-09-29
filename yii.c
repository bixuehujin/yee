/*
 * yii.c
 *
 *  Created on: Sep 30, 2013
 *      Author: hujin
 */

#include "yii.h"
#include "base/object.h"

YEE_METHOD(BaseYii, configure) {
	zval *object, *proterties;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &object, &proterties) == FAILURE) {
		return;
	}
	yee_object_configure(object, proterties);
}



