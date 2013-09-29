--TEST--
yii\base\Exception - Basic
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\base\Exception;

$e = new Exception('foo');
var_dump($e->getName());
var_dump($e->toArray());

class SubException extends Exception {
	public function getName() {
		return 'SubException';
	}
}
$e = new SubException('foo');
var_dump($e->getName());
var_dump($e->toArray());
?>
--EXPECT--
string(9) "Exception"
array(4) {
  ["type"]=>
  string(18) "yii\base\Exception"
  ["name"]=>
  string(9) "Exception"
  ["message"]=>
  string(3) "foo"
  ["code"]=>
  int(0)
}
string(12) "SubException"
array(4) {
  ["type"]=>
  string(12) "SubException"
  ["name"]=>
  string(12) "SubException"
  ["message"]=>
  string(3) "foo"
  ["code"]=>
  int(0)
}
