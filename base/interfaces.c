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

#include "php_yee.h"
#include "base/interfaces.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_arrayable_toArray, 0, 0, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry iface_arrayable_methods[] = {
	YEE_ABSTRACT_ME(Arrayable, toArray, arginfo_arrayable_toArray)
	PHP_FE_END	
};

void yee_base_interfaces_do_register_ce() {
	zend_class_entry ce_arrayble;
	INIT_CLASS_ENTRY(ce_arrayble, "yii\\base\\Arrayable", iface_arrayable_methods);
	yee_ce_Arrayable = zend_register_internal_interface(&ce_arrayble TSRMLS_CC);
}
