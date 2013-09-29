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

#ifndef PHP_YEE_H
#define PHP_YEE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"

extern zend_module_entry yee_module_entry;
#define phpext_yee_ptr &yee_module_entry

#ifdef PHP_WIN32
#	define PHP_YEE_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_YEE_API __attribute__ ((visibility("default")))
#else
#	define PHP_YEE_API
#endif


#ifdef ZTS
#include "TSRM.h"
#endif

#define YEE_MN(name) zim_yee_##name
#define YEE_METHOD(classname, name)	ZEND_NAMED_FUNCTION(YEE_MN(classname##_##name))

#define YEE_ME(classname, name, arg_info, flags)	ZEND_FENTRY(name, YEE_MN(classname##_##name), arg_info, flags)
#define YEE_ABSTRACT_ME ZEND_ABSTRACT_ME

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(yee)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(yee)
*/

/* In every utility function you add that needs to use variables 
   in php_yee_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as YEE_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define YEE_G(v) TSRMG(yee_globals_id, zend_yee_globals *, v)
#else
#define YEE_G(v) (yee_globals.v)
#endif

#endif	/* PHP_YEE_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
