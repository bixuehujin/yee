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

#ifndef YEE_BASE_OBJECT_H_
#define YEE_BASE_OBJECT_H_
#include "php_yee.h"

zend_class_entry *yee_ce_Object;

typedef struct _yee_object {
	zend_object std;
}yee_object_t;


void yee_object___construct(zval *self, zval *config);
void yee_object_init(zval *self);

zval * yee_object___get(zval *self, const char *name, const int name_len);
void yee_object___set(zval *self, const char *name, const int name_len, zval *value);
zend_bool yee_object___isset(zval *self, const char *name, const int name_len);
void yee_object___unset(zval *self, const char *name, const int name_len);

void yee_object_configure(zval *object, zval *properties);

void yee_object_register();

#endif /* YEE_BASE_OBJECT_H_ */
