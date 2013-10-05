--TEST--
yii\base\Object::__call() - basic testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\base\Object;

class Test extends Object {
	
}

$o = new Test();

try {
	$o->func();
} catch(Exception $e) {
	echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
Unknown method: Test::func
