--TEST--
yii\BaseYii::configure() - basic testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\BaseYii;

class Test extends \yii\base\Object {
	private $name;
	private $value;
	public $prop;
	
	public function setName($name) {
		$this->name = $name;
	}
	
	public function setValue($value) {
		$this->value = $value;
	}
}
$object = new Test();
BaseYii::configure($object, ['name' => 'foo', 'value' => 'bar', 'prop' => 'prop']);
var_dump($object);
?>
--EXPECTF--
object(Test)#%d (3) {
  ["name":"Test":private]=>
  string(3) "foo"
  ["value":"Test":private]=>
  string(3) "bar"
  ["prop"]=>
  string(4) "prop"
}
