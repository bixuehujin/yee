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
	if (config) {
		yee_object_configure(self, config);
	}
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
	smart_str getter = {0};
	smart_str_appendl(&getter, "get", 3);
	smart_str_appendl(&getter, name, name_len);
	smart_str_0(&getter);
	zend_str_tolower(getter.c, getter.len);
	zval *ret = NULL;
	
	if (zend_class_method_exists(object->ce, getter.c, getter.len)) {
		zend_call_method(&self, object->ce, NULL, getter.c, getter.len, &ret, 0, NULL, NULL);
	}else {
		zend_throw_exception_ex(yee_ce_UnknownPropertyException, 0, "Getting unknown property: %s::%s", object->ce->name, name);
	}
	smart_str_free(&getter);
	return ret;
}

zend_bool yee_object___isset(zval *self, const char *name, const int name_len) {
	zend_object * object = (zend_object *)zend_object_store_get_object(self);
	smart_str getter = {0};
	smart_str_appendl(&getter, "get", 3);
	smart_str_appendl(&getter, name, name_len);
	smart_str_0(&getter);
	zend_str_tolower(getter.c, getter.len);
	zval *value = NULL;
	zend_bool ret = 0;
	
	if (zend_class_method_exists(object->ce, getter.c, getter.len)) {
		zend_call_method(&self, object->ce, NULL, getter.c, getter.len, &value, 0, NULL, NULL);
		ret = Z_TYPE_P(value) != IS_NULL;
	}
	smart_str_free(&getter);
	return ret;
}

void yee_object___unset(zval *self, const char *name, const int name_len) {
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
		zval * value;
		MAKE_STD_ZVAL(value);
		Z_TYPE_P(value) = IS_NULL;
		zend_call_method(&self, object->ce, NULL, setter.c, setter.len, NULL, 1, value, NULL);
		
	}else if (zend_class_method_exists(object->ce, getter.c, getter.len)) {
		zend_throw_exception_ex(yee_ce_InvalidCallException, 0, "Unsetting read-only property: %s::%s", object->ce->name, name);
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

void yee_object_configure(zval *object, zval *properties) {
	if (Z_TYPE_P(object) != IS_OBJECT) {
		zend_error(E_ERROR, "Only object can be configured, %s given", zend_zval_type_name(object));
	}
	if (Z_TYPE_P(properties) != IS_ARRAY) {
		zend_error(E_WARNING, "Object configuration must be an array, %s given", zend_zval_type_name(properties));
		return;
	}
	HashTable * ht = Z_ARRVAL_P(properties);
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
	yee_object___construct(getThis(), config);
	zend_call_method(&getThis(), Z_OBJCE_P(getThis()), NULL, ZEND_STRL("init"), NULL, 0, NULL, NULL);
}

YEE_METHOD(Object, init) {
	
}

YEE_METHOD(Object, className) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	if (EG(called_scope)) {
		RETURN_STRINGL(EG(called_scope)->name, EG(called_scope)->name_length, 1);
	} else if (!EG(scope))  {
		zend_error(E_WARNING, "get_called_class() called from outside a class");
	}
	RETURN_FALSE;
}

YEE_METHOD(Object, __get) {
	char *name;
	uint name_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}
	zval *ret = yee_object___get(getThis(), name, name_len);
	if (ret) {
		RETURN_ZVAL(ret, 0, _zval_dtor);
	}
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
	char *name;
	int name_len;
	zend_bool isset;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}
	
	isset = yee_object___isset(getThis(), name, name_len);
	RETURN_BOOL(isset);
}

YEE_METHOD(Object, __unset) {
	char *name;
	int name_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}
	
	yee_object___unset(getThis(), name, name_len);
}

YEE_METHOD(Object, __call) {
	char *name;
	uint name_len;
	zval *params;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &name, &name_len, &params) == FAILURE) {
		return;
	}
	zend_throw_exception_ex(yee_ce_InvalidCallException, 0 TSRMLS_CC, "Unknown method: %s::%s", Z_OBJCE_P(getThis())->name, name);
}

YEE_METHOD(Object, hasProperty) {
	char *name;
	char name_len;
	zend_bool check_vars = 1;
	zend_bool ret;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &name, &name_len, &check_vars) == FAILURE) {
		return;
	}
	
	ret = zend_class_method_exists_ex(Z_OBJCE_P(getThis()), "get%s", name) ||
			(check_vars && zend_property_exists(getThis(), name, name_len)) ||
			zend_class_method_exists_ex(Z_OBJCE_P(getThis()), "set%s", name);
	RETURN_BOOL(ret);
}

YEE_METHOD(Object, canGetProperty) {
	char *name;
	char name_len;
	zend_bool check_vars = 1;
	zend_bool ret;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &name, &name_len, &check_vars) == FAILURE) {
		return;
	}
	
	ret = zend_class_method_exists_ex(Z_OBJCE_P(getThis()), "get%s", name) ||
			(check_vars && zend_property_exists(getThis(), name, name_len));
	RETURN_BOOL(ret);
}

YEE_METHOD(Object, canSetProperty) {
	char *name;
	char name_len;
	zend_bool check_vars = 1;
	zend_bool ret;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &name, &name_len, &check_vars) == FAILURE) {
		return;
	}
	
	ret = zend_class_method_exists_ex(Z_OBJCE_P(getThis()), "set%s", name) ||
			(check_vars && zend_property_exists(getThis(), name, name_len));
	RETURN_BOOL(ret);
}

YEE_METHOD(Object, hasMethod) {
	char *name;
	int name_len;
	zend_bool exist;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}
	exist = zend_class_method_exists(Z_OBJCE_P(getThis()), name, name_len);
	RETVAL_BOOL(exist);
}

YEE_METHOD(Object, toArray) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	
	zend_object_get_vars(getThis(), return_value);
}


ZEND_BEGIN_ARG_INFO_EX(arginfo_class___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_hasProperty, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, checkVars)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_canGetProperty, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, checkVars)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_canSetProperty, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, checkVars)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_hasMethod, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()


static const zend_function_entry class_methods[] = {
	YEE_ME(Object, __construct,    arginfo_class___construct,    ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	YEE_ME(Object, init,           arginfo_empty,                ZEND_ACC_PUBLIC)
	YEE_ME(Object, __set,          arginfo_magic_set,            ZEND_ACC_PUBLIC)
	YEE_ME(Object, __get,          arginfo_magic_get,            ZEND_ACC_PUBLIC)
	YEE_ME(Object, __isset,        arginfo_magic_isset,          ZEND_ACC_PUBLIC)
	YEE_ME(Object, __unset,        arginfo_magic_unset,          ZEND_ACC_PUBLIC)
	YEE_ME(Object, __call,         arginfo_magic_call,           ZEND_ACC_PUBLIC)
	YEE_ME(Object, toArray,        arginfo_empty,                ZEND_ACC_PUBLIC)
	YEE_ME(Object, hasProperty,    arginfo_class_hasProperty,    ZEND_ACC_PUBLIC)
	YEE_ME(Object, hasMethod,      arginfo_class_hasMethod,      ZEND_ACC_PUBLIC)
	YEE_ME(Object, canSetProperty, arginfo_class_canSetProperty, ZEND_ACC_PUBLIC)
	YEE_ME(Object, canGetProperty, arginfo_class_canGetProperty, ZEND_ACC_PUBLIC)
	YEE_ME(Object, className,      arginfo_empty,                ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_FE_END
};

void yee_object_register_class() {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "yii\\base\\Object", class_methods);
	yee_ce_Object = zend_register_internal_class(&ce TSRMLS_CC);
	
	zend_class_implements(yee_ce_Object TSRMLS_CC, 1, yee_ce_Arrayable);
}

