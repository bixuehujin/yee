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

#ifndef YEE_BASE_EXCEPTIONS_H_
#define YEE_BASE_EXCEPTIONS_H_

#include "php_yee.h"

zend_class_entry *yee_ce_Exception;

zend_class_entry *yee_ce_UnknownClassException;
zend_class_entry *yee_ce_UnknownMethodException;
zend_class_entry *yee_ce_UnknownPropertyException;

zend_class_entry *yee_ce_InvalidCallException;
zend_class_entry *yee_ce_InvalidConfigException;
zend_class_entry *yee_ce_InvalidParamException;
zend_class_entry *yee_ce_InvalidRouteException;

zend_class_entry *yee_ce_NotSupportedException;

zend_class_entry *yee_ce_ErrorException;

zend_class_entry *yee_ce_UserException;

zval * yee_exception_to_array(zval *self);
zval * yee_exception_to_array_recursive(zval *self, zval *exception);

void yee_base_exceptions_do_register_ce();

#endif /* YEE_BASE_EXCEPTIONS_H_ */
