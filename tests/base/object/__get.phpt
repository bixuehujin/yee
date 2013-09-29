--TEST--
yii\base\Object::__get() - basic testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\base\Object;

class Test extends Object {
	private $_name = 'foo';
	public $value = 'bar';
	
	public function getName() {
		return $this->_name;
	}
}

$o = new Test();
try{
	var_dump($o->name);
	var_dump($o->value);
	var_dump($o->undefined);
}catch(Exception $e) {
	echo $e->getMessage();
}
?>
--EXPECT--
string(3) "foo"
string(3) "bar"
Getting unknown property: Test::undefined
