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

#include "zend/zend_class.h"

zend_bool zend_class_method_exists(zend_class_entry *ce, const char *method, const int method_len) {
	char * lcmethod;
	lcmethod = zend_str_tolower_dup(method, method_len);
	if (zend_hash_exists(&ce->function_table, lcmethod, method_len + 1)) {
		efree(lcmethod);
		return 1;
	}
	//TODO maybe needs more consideration
	efree(lcmethod);
	return 0;
 }

zend_bool zend_class_method_exists_ex(zend_class_entry *ce, const char *format, ...) {
	char method[100] = {0};
	va_list args;
	int len;
	va_start (args, format);
	len = vsprintf (method, format, args);
	va_end (args);
	
	zend_str_tolower(method, len);
	if (zend_hash_exists(&ce->function_table, method, len + 1)) {
		return 1;
	}
	return 0;
}

zend_bool zend_property_exists(zval *object, const char *property, const int property_len) {
	zend_class_entry *ce, **pce;
	zend_property_info *property_info;
	zval property_z;
	ulong h;

	if (property_len == 0) {
		return 0;
	}

	if (Z_TYPE_P(object) == IS_STRING) {
		if (zend_lookup_class(Z_STRVAL_P(object), Z_STRLEN_P(object), &pce TSRMLS_CC) == FAILURE) {
			return 0;
		}
		ce = *pce;
	} else if (Z_TYPE_P(object) == IS_OBJECT) {
		ce = Z_OBJCE_P(object);
	} else {
		zend_error(E_WARNING, "First parameter must either be an object or the name of an existing class");
		return 0;
	}

	h = zend_get_hash_value(property, property_len+1);
	if (zend_hash_quick_find(&ce->properties_info, property, property_len+1, h, (void **) &property_info) == SUCCESS
		&& (property_info->flags & ZEND_ACC_SHADOW) == 0) {
		return 1;
	}

	ZVAL_STRINGL(&property_z, property, property_len, 0);

	if (Z_TYPE_P(object) ==  IS_OBJECT &&
		Z_OBJ_HANDLER_P(object, has_property) && 
		Z_OBJ_HANDLER_P(object, has_property)(object, &property_z, 2, 0 TSRMLS_CC)) {
		return 1;
	}
	return 0;
}



zval * zend_object_get_vars(zval *object, zval *container) {
	zval * values;
	zval **value;
	HashTable *properties;
	HashPosition pos;
	char *key;
	const char *prop_name, *class_name;
	uint key_len, prop_len;
	ulong num_index;
	zend_object *zobj;
	
	if (container == NULL) {
		MAKE_STD_ZVAL(values);
	}else {
		values = container;
	}
	if (Z_TYPE_P(object) != IS_OBJECT) {
		ZVAL_NULL(values);
		return values;
	}
	
	if (Z_OBJ_HT_P(object)->get_properties == NULL) {
		ZVAL_FALSE(values);
		return values;
	}

	properties = Z_OBJ_HT_P(object)->get_properties(object TSRMLS_CC);

	if (properties == NULL) {
		ZVAL_FALSE(values);
		return values;
	}

	zobj = zend_objects_get_address(object TSRMLS_CC);

	array_init(values);

	zend_hash_internal_pointer_reset_ex(properties, &pos);

	while (zend_hash_get_current_data_ex(properties, (void **) &value, &pos) == SUCCESS) {
		if (zend_hash_get_current_key_ex(properties, &key, &key_len, &num_index, 0, &pos) == HASH_KEY_IS_STRING) {
			if (zend_check_property_access(zobj, key, key_len-1 TSRMLS_CC) == SUCCESS) {
				zend_unmangle_property_name_ex(key, key_len - 1, &class_name, &prop_name, (int*) &prop_len);
				/* Not separating references */
				Z_ADDREF_PP(value);
				add_assoc_zval_ex(values, prop_name, prop_len + 1, *value);
			}
		}
		zend_hash_move_forward_ex(properties, &pos);
	}
	
	return values;
}

