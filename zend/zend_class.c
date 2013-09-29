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
