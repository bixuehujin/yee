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

#include "yii.h"
#include "base/object.h"
#include "base/exceptions.h"


char * yee_base_yii_get_alias(zval *target, const char *alias, int alias_len, zend_bool throw) {
	char * root_alias, *alias_copy = estrndup(alias, alias_len);
	zval ** zv_root_alias_ptr, *zv_root_alias;
	char *path;
	if (alias[0] != '@') {
		return alias_copy;
	}
	char * pos = strchr(alias_copy, '/');
	if (pos != NULL) {
		*pos = '\0';
	}
	root_alias = alias_copy;
	
	if (zend_hash_num_elements(Z_ARRVAL_P(target)) && zend_hash_find(Z_ARRVAL_P(target), root_alias, strlen(root_alias) + 1, (void **)&zv_root_alias_ptr) == SUCCESS) {
		if (Z_TYPE_P(*zv_root_alias_ptr) == IS_STRING) {
			if (pos == NULL) {
				efree(alias_copy);
				return Z_STRVAL_P(*zv_root_alias_ptr);
			}else {
				smart_str tmp = {NULL, 0, 0};
				
				smart_str_appendl(&tmp, Z_STRVAL_PP(zv_root_alias_ptr), Z_STRLEN_PP(zv_root_alias_ptr));
				smart_str_appendl(&tmp, (char *)alias + (pos - alias_copy), alias_len - (pos - alias_copy));
				smart_str_0(&tmp);
				efree(alias_copy);
				return tmp.c;
			}
		}else {
			HashTable *ht = Z_ARRVAL_PP(zv_root_alias_ptr);
			char *name;
			int name_len;
			zval **path;
			
			zend_hash_internal_pointer_reset(ht);
			while(zend_hash_has_more_elements(ht) == SUCCESS) {
				zend_hash_get_current_key_ex(ht, &name, &name_len, NULL, 0, NULL);
				zend_hash_get_current_data(ht, (void **)&path);
				name_len -= 1;
				
				if ((name_len  == alias_len || alias[name_len] == '/') && strncmp(alias, name, name_len) >= 0) {
					smart_str tmp = {NULL, 0, 0};
					smart_str_appendl(&tmp, Z_STRVAL_PP(path), Z_STRLEN_PP(path));
					smart_str_appendl(&tmp, alias + name_len, alias_len - name_len);
					
					smart_str_0(&tmp);
					efree(alias_copy);
					return tmp.c;
				}
				zend_hash_move_forward(ht);
			}
		}
	}
	efree(alias_copy);
	if (throw) {
		zend_throw_exception_ex(yee_ce_InvalidParamException, 0, "Invalid path alias: %s", alias);
	}
	return NULL;
}

/* copy and modified from ext/standard/array.c */
static reverse_key_compare(const void *a, const void *b TSRMLS_DC) {
	Bucket *f;
	Bucket *s;
	zval result;
	zval first;
	zval second;

	f = *((Bucket **) a);
	s = *((Bucket **) b);

	if (f->nKeyLength == 0) {
		Z_TYPE(first) = IS_LONG;
		Z_LVAL(first) = f->h;
	} else {
		Z_TYPE(first) = IS_STRING;
		Z_STRVAL(first) = (char*)f->arKey;
		Z_STRLEN(first) = f->nKeyLength - 1;
	}

	if (s->nKeyLength == 0) {
		Z_TYPE(second) = IS_LONG;
		Z_LVAL(second) = s->h;
	} else {
		Z_TYPE(second) = IS_STRING;
		Z_STRVAL(second) = (char*)s->arKey;
		Z_STRLEN(second) = s->nKeyLength - 1;
	}

	if (Z_TYPE(result) == IS_DOUBLE) {
		if (Z_DVAL(result) < 0) {
			return -1;
		} else if (Z_DVAL(result) > 0) {
			return 1;
		} else {
			return 0;
		}
	}

	convert_to_long(&result);

	if (Z_LVAL(result) < 0) {
		return -1;
	} else if (Z_LVAL(result) > 0) {
		return 1;
	}

	return 0;
}

void yee_base_yii_set_alias(zval *target, const char *alias, int alias_len, char *path, int path_len TSRMLS_DC) {
	char *alias_copy, *pos, *root_alias, *new_path;
	int alias_copy_len, i, root_alias_len;
	
	alias_copy_len = alias_len;
	root_alias_len = alias_len;
	alias_copy = estrndup(alias, alias_len + 1);
	
	if (alias[0] != '@') {
		alias_copy_len += 1;
		root_alias_len += 1;
		i = alias_len;

		do {
			alias_copy[i] = alias_copy[i - 1];
		}while (-- i);
		
		alias_copy[0] = '@';
		alias_copy[alias_copy_len] = '\0';
	}
	
	pos = strchr(alias_copy, '/');
	
	if (pos) {
		root_alias_len = pos - alias_copy;
		root_alias = estrndup(alias_copy, root_alias_len);
		root_alias[root_alias_len] = '\0';
	}else {
		root_alias = alias_copy;
	}
	
	ulong root_alias_idx = zend_get_hash_value(root_alias, root_alias_len + 1);
	zval **zv_root_alias_ptr = NULL, *zv_root_alias;
	
	if (zend_hash_num_elements(Z_ARRVAL_P(target))) {
		zend_hash_quick_find(Z_ARRVAL_P(target), root_alias, root_alias_len + 1, root_alias_idx, (void **)&zv_root_alias_ptr);
	}
	
	if (path != NULL) {
		if (path[0] == '@') {
			new_path = yee_base_yii_get_alias(target, path, path_len, 1);
			if (!new_path) {
				goto out;
			}
		}else {
			i = path_len - 1;
			while(path[i] == '/' || path[i] == '\\') i--;
			path[i + 1] = '\0';
			
			new_path = estrdup(path);
		}
		
		if (!zv_root_alias_ptr) {
			MAKE_STD_ZVAL(zv_root_alias);
			if (pos == NULL) {
				ZVAL_STRING(zv_root_alias, new_path, 0);
			}else {
				array_init(zv_root_alias);
				add_assoc_string_ex(zv_root_alias, alias_copy, alias_copy_len + 1, new_path, 0);
			}
			zend_hash_quick_add(Z_ARRVAL_P(target), root_alias, root_alias_len + 1, root_alias_idx, &zv_root_alias, sizeof(zval *), NULL);
		}else if (Z_TYPE_PP(zv_root_alias_ptr) == IS_STRING) {
			
			if (pos == NULL) {
				zval_dtor(*zv_root_alias_ptr);
				ZVAL_STRING(*zv_root_alias_ptr, new_path, 0);
			}else {
				MAKE_STD_ZVAL(zv_root_alias);
				array_init(zv_root_alias);
				
				add_assoc_string(zv_root_alias, alias_copy, new_path, 0);
				add_assoc_string(zv_root_alias, root_alias, Z_STRVAL_PP(zv_root_alias_ptr), 1);
				zval_dtor(*zv_root_alias_ptr);
				ZVAL_ZVAL(*zv_root_alias_ptr, zv_root_alias, 0, _zval_dtor);
			}
		}else {
			zval **tmp;
			if (zend_hash_find(Z_ARRVAL_PP(zv_root_alias_ptr), alias_copy, alias_copy_len + 1, (void **)&tmp) == SUCCESS) {
				zval_dtor(*tmp);
				ZVAL_STRING(*tmp, new_path, 0);
			}else {
				add_assoc_string_ex(*zv_root_alias_ptr, alias_copy, alias_copy_len + 1, new_path, 0);
			}
			
			zend_hash_sort(Z_ARRVAL_PP(zv_root_alias_ptr), zend_qsort, reverse_key_compare, 0 TSRMLS_CC);
		}
	}else if (zv_root_alias_ptr) {
		if (Z_TYPE_PP(zv_root_alias_ptr) == IS_ARRAY) {
			zend_hash_del(Z_ARRVAL_PP(zv_root_alias_ptr), alias_copy, alias_copy_len + 1);
		}else if (pos == NULL){
			zend_hash_quick_del(Z_ARRVAL_P(target), root_alias, root_alias_len + 1, root_alias_idx);
		}
	}
	
out:
	efree(alias_copy);
	if (alias_copy != root_alias) {
		efree(root_alias);
	}
}


YEE_METHOD(BaseYii, getVersion) {
	RETURN_STRING("2.0-dev", 1);
}

YEE_METHOD(BaseYii, getAlias) {
	char *alias;
	int alias_len;
	zend_bool throw = 1;
	char * ret;
	zval **target;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &alias, &alias_len, &throw) == FAILURE) {
		return;
	}
	
	target = zend_std_get_static_property(yee_ce_BaseYii, ZEND_STRL("aliases"), 0, NULL);
	
	ret = yee_base_yii_get_alias(*target, alias, alias_len, 0);
	if (ret) {
		RETURN_STRING(ret, 0);
	}else if (throw){
		zend_throw_exception_ex(yee_ce_InvalidParamException, 0, "Invalid path alias: %s", alias);
	}else {
		RETURN_FALSE;
	}
}

YEE_METHOD(BaseYii, setAlias) {
	char *alias, *path;
	int alias_len, path_len;
	zval* zv_path;
	zval **target;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &alias, &alias_len, &zv_path) == FAILURE) {
		return;
	}
	target = zend_std_get_static_property(yee_ce_BaseYii, ZEND_STRL("aliases"), 0, NULL);
	if (Z_TYPE_P(zv_path) == IS_NULL) {
		yee_base_yii_set_alias(*target, alias, alias_len, NULL, 0);
		return;
	}
	
	if (Z_TYPE_P(zv_path) != IS_STRING){
		zend_error(E_WARNING, "The $path parameter of %s::setAlias except string, but %s given", 
				EG(called_scope)->name, zend_zval_type_name(zv_path));
		convert_to_string(zv_path);
	}
	yee_base_yii_set_alias(*target, alias, alias_len, Z_STRVAL_P(zv_path), Z_STRLEN_P(zv_path));
}

YEE_METHOD(BaseYii, importNamespaces) {
	
}

YEE_METHOD(BaseYii, configure) {
	zval *object, *proterties;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &object, &proterties) == FAILURE) {
		return;
	}
	yee_object_configure(object, proterties);
}


YEE_METHOD(BaseYii, getObjectVars) {
	zval *object;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &object) == FAILURE) {
		return;
	}
	zend_object_get_vars(object, return_value);
}



ZEND_BEGIN_ARG_INFO_EX(arginfo_BaseYii_configure, 0, 0, 2)
	ZEND_ARG_INFO(0, object)
	ZEND_ARG_INFO(0, properties)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_BaseYii_getObjectVars, 0, 0, 1)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_BaseYii_setAlias, 0, 0, 2)
	ZEND_ARG_INFO(0, alias)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_BaseYii_getAlias, 0, 0, 1)
	ZEND_ARG_INFO(0, alias)
	ZEND_ARG_INFO(0, throwException)
ZEND_END_ARG_INFO()


static const zend_function_entry methods_BaseYii[] = {
	YEE_ME(BaseYii, configure,     arginfo_BaseYii_configure,      ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	YEE_ME(BaseYii, getObjectVars, arginfo_BaseYii_getObjectVars,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	YEE_ME(BaseYii, getAlias,      arginfo_BaseYii_getAlias,       ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	YEE_ME(BaseYii, setAlias,      arginfo_BaseYii_setAlias,       ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
};

void yee_base_yii_do_register_ce() {
	zend_class_entry ce_BaseYii;
	INIT_CLASS_ENTRY(ce_BaseYii, "yii\\BaseYii", methods_BaseYii);
	yee_ce_BaseYii = zend_register_internal_class(&ce_BaseYii);
	
	zend_declare_property_null(yee_ce_BaseYii, ZEND_STRL("aliases"),      ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
	zend_declare_property_null(yee_ce_BaseYii, ZEND_STRL("app"),          ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
	zend_declare_property_null(yee_ce_BaseYii, ZEND_STRL("classMap"),     ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
	zend_declare_property_null(yee_ce_BaseYii, ZEND_STRL("objectConfig"), ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
	
	zend_class_entry ce_Yii;
	INIT_CLASS_ENTRY(ce_Yii, "Yii", NULL);
	yee_ce_Yii = zend_register_internal_class_ex(&ce_Yii, yee_ce_BaseYii, "yii\\BaseYii");
}

void yee_base_yii_do_initialize_static_properties() {
	zval **aliases_ptr, **class_map_ptr, **object_config;
	
	aliases_ptr   = zend_std_get_static_property(yee_ce_BaseYii, ZEND_STRL("aliases"), 1, NULL);
	class_map_ptr = zend_std_get_static_property(yee_ce_BaseYii, ZEND_STRL("classMap"), 1, NULL);
	object_config = zend_std_get_static_property(yee_ce_BaseYii, ZEND_STRL("objectConfig"), 1, NULL);
	
	array_init(*aliases_ptr);
	array_init(*class_map_ptr);
	array_init(*object_config);
}
