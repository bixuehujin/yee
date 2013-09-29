--TEST--
yii\base\Exception - Basic with previous exception support
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\base\Exception;

$previous = new Exception('bar');

class SubException extends Exception {
	public function getName() {
		return 'SubException';
	}
}
$e = new SubException('foo', 0, $previous);
var_dump($e->toArray());
?>
--EXPECT--
array(5) {
  ["type"]=>
  string(12) "SubException"
  ["name"]=>
  string(12) "SubException"
  ["message"]=>
  string(3) "foo"
  ["code"]=>
  int(0)
  ["previous"]=>
  array(4) {
    ["type"]=>
    string(18) "yii\base\Exception"
    ["name"]=>
    string(9) "Exception"
    ["message"]=>
    string(3) "bar"
    ["code"]=>
    int(0)
  }
}
