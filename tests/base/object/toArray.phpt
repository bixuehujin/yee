--TEST--
yii\base\Object::toArray() - basic testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\base\Object;

class Test extends Object {
	public $a = 'a';
	protected $b = 'b';
}

$o = new Test();
var_dump($o->toArray());
?>
--EXPECTF--
array(1) {
  ["a"]=>
  string(1) "a"
}
