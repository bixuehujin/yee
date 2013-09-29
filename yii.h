/*
 * yii.h
 *
 *  Created on: Sep 30, 2013
 *      Author: hujin
 */

#ifndef YEE_YII_H_
#define YEE_YII_H_
#include "php_yee.h"

zend_class_entry *yee_ce_BaseYii;
zend_class_entry *yee_ce_Yii;


YEE_METHOD(BaseYii, configure);

ZEND_BEGIN_ARG_INFO_EX(arginfo_BaseYii_configure, 0, 0, 2)
	ZEND_ARG_INFO(0, object)
	ZEND_ARG_INFO(0, properties)
ZEND_END_ARG_INFO()

static const zend_function_entry methods_BaseYii[] = {
	YEE_ME(BaseYii, configure, arginfo_BaseYii_configure, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
};


#endif /* YEE_YII_H_ */
