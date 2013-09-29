dnl $Id$
dnl config.m4 for extension yee


PHP_ARG_ENABLE(yee, whether to enable yee support,
[  --enable-yee           Enable yee support])

if test "$PHP_YEE" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-yee -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/yee.h"  # you most likely want to change this
  dnl if test -r $PHP_YEE/$SEARCH_FOR; then # path given as parameter
  dnl   YEE_DIR=$PHP_YEE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for yee files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       YEE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$YEE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the yee distribution])
  dnl fi

  dnl # --with-yee -> add include path
  dnl PHP_ADD_INCLUDE($YEE_DIR/include)

  dnl # --with-yee -> check for lib and symbol presence
  dnl LIBNAME=yee # you may want to change this
  dnl LIBSYMBOL=yee # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YEE_DIR/lib, YEE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_YEELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong yee lib version or lib not found])
  dnl ],[
  dnl   -L$YEE_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(YEE_SHARED_LIBADD)
  
  sources="yee.c              \
  	zend/zend_class.c         \
  	yii.c                     \
  	base/interfaces.c         \
  	base/exceptions.c         \
    base/object.c             \
    base/component.c          "
  
  PHP_NEW_EXTENSION(yee, $sources, $ext_shared)
fi
