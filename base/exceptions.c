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

zval * yee_exception_to_array(zval *self) {
	zend_object *object = zend_object_store_get_object(self);
	zval *ret = NULL;
	zend_call_method(&self, object->ce, NULL, ZEND_STRL("toarrayrecursive"), &ret, 1, self, NULL);
	return ret;
}

zval * yee_exception_to_array_recursive(zval *self, zval *exception) {
	zend_object *object        = (zend_object *)zend_object_store_get_object(self TSRMLS_CC);
	zend_object *except_object = (zend_object *)zend_object_store_get_object(exception TSRMLS_CC);
	zval *array;
	zval *name = NULL, *message = NULL, *code = NULL, *previous = NULL, *previous2 = NULL;
	
	MAKE_STD_ZVAL(array);
	array_init_size(array, 5);
	
	if (instanceof_function(except_object->ce, yee_ce_Exception)) {
		zend_call_method(&exception, except_object->ce, NULL, ZEND_STRL("getname"), &name, 0, NULL, NULL);
	}else {
		MAKE_STD_ZVAL(name);
		ZVAL_STRING(name, "Exception", 1);
	}
	
	zend_call_method(&exception, except_object->ce, NULL, ZEND_STRL("getmessage"), &message, 0, NULL, NULL);
	zend_call_method(&exception, except_object->ce, NULL, ZEND_STRL("getcode"), &code, 0, NULL, NULL);
	zend_call_method(&exception, except_object->ce, NULL, ZEND_STRL("getprevious"), &previous, 0, NULL, NULL);
	
	add_assoc_string_ex(array, ZEND_STRS("type"), (char *)except_object->ce->name, 1);
	add_assoc_zval_ex(array, ZEND_STRS("name"), name);
	add_assoc_zval_ex(array, ZEND_STRS("message"), message);
	add_assoc_zval_ex(array, ZEND_STRS("code"), code);
	
	if (previous && previous->type == IS_OBJECT) {
		previous2 = yee_exception_to_array_recursive(self, previous);
		add_assoc_zval_ex(array, ZEND_STRS("previous"), previous2);
	}
	zval_dtor(previous);
	efree(previous);
	
	return array;
}

YEE_METHOD(Exception, getName) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_STRING("Exception", 1)
}

YEE_METHOD(Exception, toArray) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zval * ret = yee_exception_to_array(getThis());
	RETURN_ZVAL(ret, 0, _zval_dtor)
}

YEE_METHOD(Exception, toArrayRecursive) {
	zval *except;
	zval *ret;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &except) == FAILURE) {
		return;
	}
	ret = yee_exception_to_array_recursive(getThis(), except);
	RETURN_ZVAL(ret, 0, _zval_dtor)
}


YEE_METHOD(InvalidCallException, getName) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_STRING("Invalid Call", 1);
}

YEE_METHOD(InvalidConfigException, getName) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_STRING("Invalid Configuration", 1);
}


YEE_METHOD(InvalidParamException, getName) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_STRING("Invalid Parameter", 1);
}

YEE_METHOD(InvalidRouteException, getName) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_STRING("Invalid Route", 1);
}

YEE_METHOD(NotSupportedException, getName) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_STRING("Not Supported", 1);
}

YEE_METHOD(UnknownClassException, getName) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_STRING("Unknown Class", 1);
}

YEE_METHOD(UnknownMethodException, getName) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_STRING("Unknown Method", 1);
}

YEE_METHOD(UnknownPropertyException, getName) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_STRING("Unknown Property", 1);
}



ZEND_BEGIN_ARG_INFO_EX(arginfo_Exception_getName, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_Exception_toArray, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_Exception_toArrayRecursive, 0, 0, 1)
ZEND_ARG_INFO(0, exception)
ZEND_END_ARG_INFO()


static const zend_function_entry exception_methods[] = {
	YEE_ME(Exception, getName,          arginfo_Exception_getName,          ZEND_ACC_PUBLIC)
	YEE_ME(Exception, toArray,          arginfo_Exception_toArray,          ZEND_ACC_PUBLIC)
	YEE_ME(Exception, toArrayRecursive, arginfo_Exception_toArrayRecursive, ZEND_ACC_PROTECTED)
	PHP_FE_END
};

static const zend_function_entry invalid_call_exception_methods[] = {
	YEE_ME(InvalidCallException, getName, arginfo_Exception_getName, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static const zend_function_entry invalid_config_exception_methods[] = {
	YEE_ME(InvalidCallException, getName, arginfo_Exception_getName, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static const zend_function_entry invalid_param_exception_methods[] = {
	YEE_ME(InvalidCallException, getName, arginfo_Exception_getName, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static const zend_function_entry invalid_route_exception_methods[] = {
	YEE_ME(InvalidCallException, getName, arginfo_Exception_getName, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static const zend_function_entry not_supported_exception_methods[] = {
	YEE_ME(InvalidCallException, getName, arginfo_Exception_getName, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static const zend_function_entry unknown_class_exception_methods[] = {
	YEE_ME(InvalidCallException, getName, arginfo_Exception_getName, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static const zend_function_entry unknown_method_exception_methods[] = {
	YEE_ME(InvalidCallException, getName, arginfo_Exception_getName, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static const zend_function_entry unknown_property_exception_methods[] = {
	YEE_ME(InvalidCallException, getName, arginfo_Exception_getName, ZEND_ACC_PUBLIC)
	PHP_FE_END
};


void yee_base_register_exceptions() {
	zend_class_entry ce_exception;
	INIT_CLASS_ENTRY(ce_exception, "yii\\base\\Exception", exception_methods);
	yee_ce_Exception = zend_register_internal_class_ex(&ce_exception, zend_exception_get_default(TSRMLS_CC), "Exception");
	zend_class_implements(yee_ce_Exception TSRMLS_CC, 1, yee_ce_Arrayable);
	
	zend_class_entry ce_invalid_call;
	INIT_CLASS_ENTRY(ce_invalid_call, "yii\\base\\InvalidCallException", invalid_call_exception_methods);
	yee_ce_InvalidCallException = zend_register_internal_class_ex(&ce_invalid_call, yee_ce_Exception, "yii\\base\\Exception");
	
	zend_class_entry ce_invalid_config;
	INIT_CLASS_ENTRY(ce_invalid_config, "yii\\base\\InvalidConfigException", invalid_config_exception_methods);
	yee_ce_InvalidConfigException = zend_register_internal_class_ex(&ce_invalid_config, yee_ce_Exception, "yii\\base\\Exception");
	
	zend_class_entry ce_invalid_param;
	INIT_CLASS_ENTRY(ce_invalid_param, "yii\\base\\InvalidParamException", invalid_param_exception_methods);
	yee_ce_InvalidParamException = zend_register_internal_class_ex(&ce_invalid_param, yee_ce_Exception, "yii\\base\\Exception");
	
	zend_class_entry ce_invalid_route;
	INIT_CLASS_ENTRY(ce_invalid_route, "yii\\base\\InvalidRouteException", invalid_route_exception_methods);
	yee_ce_InvalidRouteException = zend_register_internal_class_ex(&ce_invalid_route, yee_ce_Exception, "yii\\base\\Exception");
		
	zend_class_entry ce_not_supported;
	INIT_CLASS_ENTRY(ce_not_supported, "yii\\base\\NotSupportedException", not_supported_exception_methods);
	yee_ce_NotSupportedException = zend_register_internal_class_ex(&ce_not_supported, yee_ce_Exception, "yii\\base\\Exception");
	
	zend_class_entry ce_unknown_class;
	INIT_CLASS_ENTRY(ce_unknown_class, "yii\\base\\UnknowClassException", unknown_class_exception_methods);
	yee_ce_UnknownClassException = zend_register_internal_class_ex(&ce_unknown_class, yee_ce_Exception, "yii\\base\\Exception");
	
	zend_class_entry ce_unknown_method;
	INIT_CLASS_ENTRY(ce_unknown_method, "yii\\base\\UnknowMethodException", unknown_method_exception_methods);
	yee_ce_UnknownMethodException = zend_register_internal_class_ex(&ce_unknown_method, yee_ce_Exception, "yii\\base\\Exception");
			
	zend_class_entry ce_unknown_property;
	INIT_CLASS_ENTRY(ce_unknown_property, "yii\\base\\UnknowPropertyException", unknown_property_exception_methods);
	yee_ce_UnknownPropertyException = zend_register_internal_class_ex(&ce_unknown_property, yee_ce_Exception, "yii\\base\\Exception");
	
	zend_class_entry ce_user_excepton;
	INIT_CLASS_ENTRY(ce_user_excepton, "yii\\base\\UserException", NULL);
	yee_ce_UserException = zend_register_internal_class_ex(&ce_user_excepton, yee_ce_Exception, "yii\\base\\Exception");
}
