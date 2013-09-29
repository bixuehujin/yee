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
#include "base/exceptions.h"
#include "base/object.h"


void yee_object___construct(zval *self, zval *config) {
	
}

char * yee_object_class_name(zval *self, int *len) {
	return NULL;
}



void yee_object_init(zval *self) {
	
}

void yee_object___set(zval *self, const char *name, const int name_len, zval *value) {
	zend_object * object = (zend_object *)zend_object_store_get_object(self);
	int result = 0;
	
	smart_str setter = {0};
	smart_str_appendl(&setter, "set", 3);
	smart_str_appendl(&setter, name, name_len);
	smart_str_0(&setter);
	
	smart_str getter = {0};
	smart_str_appendl(&getter, "get", 3);
	smart_str_appendl(&getter, name, name_len);
	smart_str_0(&getter);
	
	zend_str_tolower(setter.c, setter.len);
	zend_str_tolower(getter.c, getter.len);
	
	if (zend_class_method_exists(object->ce, setter.c, setter.len)) {
		zend_call_method(&self, object->ce, NULL, setter.c, setter.len, NULL, 1, value, NULL);
		result = 1;
	}else if (zend_class_method_exists(object->ce, getter.c, getter.len)) {
		zend_throw_exception_ex(yee_ce_InvalidCallException, 0, "Setting read-only property: %s::%s", object->ce->name, name);
	}else {
		zend_throw_exception_ex(yee_ce_UnknownPropertyException, 0, "Setting unknown property: %s::%s", object->ce->name, name);
	}
	/*
	if (!result) {
		zval_dtor(value);
		efree(value);
	}
	*/
	smart_str_free(&setter);
	smart_str_free(&getter);
}

zval * yee_object___get(zval *self, const char *name, const int name_len) {
	zend_object * object = (zend_object *)zend_object_store_get_object(self);
	smart_str method = {0};
	smart_str_appendl(&method, "get", 3);
	smart_str_appendl(&method, name, name_len);
	smart_str_0(&method);
	
	if (zend_class_method_exists(object->ce, method.c, method.len)) {
		
		return NULL;
	}else {
		zend_throw_exception_ex(yee_ce_UnknownPropertyException, 0, "Getting unknown property: %s::%s", object->ce->name, name);
		return NULL;
	}
}

void yee_object_configure(zval *object, zval *properties) {
	if (Z_TYPE_P(object) != IS_OBJECT) {
		zend_error(E_ERROR, "Only object can be configured, %s given", zend_zval_type_name(object));
	}
	if (Z_TYPE_P(properties) != IS_ARRAY) {
		zend_error(E_WARNING, "Object configuration must be an array, %s given", zend_zval_type_name(properties));
		return;
	}
	HashTable * ht = properties->value.ht;
	zval **value, key;

	zend_hash_internal_pointer_reset(ht);
	while (zend_hash_has_more_elements(ht) == SUCCESS) {
		zend_hash_get_current_key_zval(ht, &key);
		zend_hash_get_current_data(ht, (void **)&value);
		
		Z_OBJ_HT_P(object)->write_property(object, &key, *value, NULL);
		zval_dtor(&key);
		zend_hash_move_forward(ht);
	}
}

YEE_METHOD(Object, __construct) {
	zval *config = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}
	if (config) {
		yee_object___construct(getThis(), config);
	}
	yee_object_init(getThis());
}

YEE_METHOD(Object, init) {
	
}

YEE_METHOD(Object, className) {
	
}

YEE_METHOD(Object, __get) {
	
}

YEE_METHOD(Object, __set) {
	char *name;
	int name_len;
	zval *value;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &name, &name_len, &value) == FAILURE) {
		return;
	}
	yee_object___set(getThis(), name, name_len, value);
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


ZEND_BEGIN_ARG_INFO_EX(arginfo_class___set, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()


static const zend_function_entry class_methods[] = {
	YEE_ME(Object, __construct,    arginfo_class___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	YEE_ME(Object, init,           arginfo_empty,             ZEND_ACC_PUBLIC)
	YEE_ME(Object, __set,          arginfo_class___set,       ZEND_ACC_PUBLIC)
	YEE_ME(Object, toArray,        arginfo_empty,             ZEND_ACC_PUBLIC)
	PHP_FE_END
};

void yee_object_register_class() {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "yii\\base\\Object", class_methods);
	yee_ce_Object = zend_register_internal_class(&ce TSRMLS_CC);
	
	zend_class_implements(yee_ce_Object TSRMLS_CC, 1, yee_ce_Arrayable);
}

