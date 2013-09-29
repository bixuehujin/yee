/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  Jin Hu <bixuehujin@gmail.com>                               |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#include "base/interfaces.h"
#include "base/object.h"

void yee_object___construct(yee_object_t *object, zval *config) {
	
}

char * yee_object_class_name(zval *self, int *len) {
	return NULL;
}



void yee_object_init(yee_object_t *object) {
	
}

zval * yee_object___get(yee_object_t *object, const char *name, const int name_len) {
	smart_str method = {0};
	smart_str_appendl(&method, "get", 3);
	smart_str_appendl(&method, name, name_len);
	smart_str_0(&method);
	
	if (zend_class_method_exists(object->std.ce, method.c, method.len)) {
		
		return NULL;
	}else {
		zend_throw_exception_ex(object->std.ce, 0, "Getting unknown property: %s::%s", object->std.ce->name, name);
		return NULL;
	}
}

YEE_METHOD(Object, __construct) {
	yee_object_t *object;
	zval *config;
	
	object = (yee_object_t *)zend_object_store_get_object(getThis());
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &config) == FAILURE) {
		return;
	}
	yee_object___construct(object, config);
	yee_object_init(object);
}

YEE_METHOD(Object, init) {
	
}

YEE_METHOD(Object, className) {
	
}

YEE_METHOD(Object, __get) {
	
}

YEE_METHOD(Object, __set) {
	
}

YEE_METHOD(Object, __isset) {
	
}

YEE_METHOD(Object, __unset) {
	
}

YEE_METHOD(Object, __call) {
	
}

YEE_METHOD(Object, hasProperty) {
	
}

YEE_METHOD(Object, canGetProperty) {
	
}

YEE_METHOD(Object, canSetProperty) {
	
}

YEE_METHOD(Object, hasMethod) {
	
}

YEE_METHOD(Object, toArray) {
	
}


ZEND_BEGIN_ARG_INFO_EX(arginfo_class___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_init, 0, 0, 0)
ZEND_END_ARG_INFO()



static const zend_function_entry class_methods[] = {
	YEE_ME(Object, __construct,    arginfo_class___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	YEE_ME(Object, init,           arginfo_class_init,        ZEND_ACC_PUBLIC)
	PHP_FE_END
};

void yee_object_register_class() {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "yii\\base\\Object", class_methods);
	yee_ce_Object = zend_register_internal_class(&ce TSRMLS_CC);
	
	zend_class_implements(yee_ce_Object TSRMLS_CC, 1, yee_ce_Arrayable);
}

