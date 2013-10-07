/*
 * behavior.c
 *
 *  Created on: Oct 7, 2013
 *      Author: hujin
 */

#include "base/behavior.h"
#include "base/object.h"


void yee_behavior_attach(zval *self, zval *owner) {
	zend_class_entry *ce = Z_OBJCE_P(self);
	zval *events = NULL, **handler_ptr;
	char *event;
	int event_size;
	
	zend_update_property(ce, self, ZEND_STRL("owner"), owner);
	
	zend_call_method(&self, ce, NULL, ZEND_STRL("events"), &events, 0, NULL, NULL);
	
	if (events && Z_TYPE_P(events) == IS_ARRAY) {
		HashTable *ht = Z_ARRVAL_P(events);
		zend_hash_internal_pointer_reset(ht);
		while(zend_hash_has_more_elements(ht) == SUCCESS) {
			zend_hash_get_current_key_ex(ht, &event, &event_size, NULL, 0, NULL);
			zend_hash_get_current_data(ht, (void **)&handler_ptr);
			
			zval *zv_event, *zv_handler;
			
			MAKE_STD_ZVAL(zv_event);
			ZVAL_STRINGL(zv_event, event, event_size - 1, 0);
			
			if (Z_TYPE_PP(handler_ptr) == IS_STRING) {
				MAKE_STD_ZVAL(zv_handler);
				array_init(zv_handler);
				add_next_index_zval(zv_handler, self);
				add_next_index_zval(zv_handler, *handler_ptr);

				zval_addref_p(self);
			}else {
				zv_handler = *handler_ptr;
			}
			
			zend_call_method(&owner, Z_OBJCE_P(owner), NULL, ZEND_STRL("on"), NULL, 2, zv_event, zv_handler);
			
			efree(zv_event);
			if (zv_handler != *handler_ptr) {
				zval_ptr_dtor(&zv_handler);
			}
			zend_hash_move_forward(ht);
		}
		
	}
	zval_ptr_dtor(&events);
}


YEE_METHOD(Behavior, events) {
	array_init(return_value);
}

YEE_METHOD(Behavior, attach) {
	zval *owner;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &owner) == FAILURE) {
		return;
	}
	yee_behavior_attach(getThis(), owner);
}

YEE_METHOD(Behavior, detach) {
	
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_attach, 0, 0, 1)
	ZEND_ARG_INFO(0, owner)
ZEND_END_ARG_INFO()

static const zend_function_entry methods_Behavior[] = {
	YEE_ME(Behavior, events, arginfo_empty,  ZEND_ACC_PUBLIC)
	YEE_ME(Behavior, attach, arginfo_attach, ZEND_ACC_PUBLIC)
	YEE_ME(Behavior, detach, arginfo_empty,  ZEND_ACC_PUBLIC)
	PHP_FE_END
};

void yee_behavior_do_register_ce() {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "yii\\base\\Behavior", methods_Behavior);
	yee_ce_Behavior = zend_register_internal_class_ex(&ce, yee_ce_Object, "yii\\base\\Object");
	
	zend_declare_property_null(yee_ce_Behavior, ZEND_STRL("owner"), ZEND_ACC_PUBLIC);
}
