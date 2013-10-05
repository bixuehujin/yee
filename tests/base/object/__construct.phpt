--TEST--
yii\base\Object::__construct() - basic testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\base\Object;

class Test extends Object {
	public $name;
	private $value;
	
	public function setValue($value) {
		$this->value = $value;
	} 
}

$o = new Test(['name' => 'foo', 'value' => 'bar']);
var_dump($o);
?>
--EXPECTF--
object(Test)#%d (2) {
  ["name"]=>
  string(3) "foo"
  ["value":"Test":private]=>
  string(3) "bar"
}
