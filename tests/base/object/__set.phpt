--TEST--
yii\base\Object::__set() - basic testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\base\Object;

class Test extends Object {
	private $_name;
	private $_value;
	
	public function setName($name) {
		$this->_name = $name;
	}
	
	public function getName() {
		return $this->_name;
	}
	
	public function getValue() {
		return $this->_value;
	}
}

$o = new Test();
var_dump($o->getName());
$o->name = 'foo';
var_dump($o->getName());

try {
	$o->value = 'bar';
} catch(Exception $e) {
	echo $e->getMessage(), PHP_EOL;
}

try {
	$o->undefined_prop = 'bar';
} catch(Exception $e) {
	echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
NULL
string(3) "foo"
Setting read-only property: Test::value
Setting unknown property: Test::undefined_prop
