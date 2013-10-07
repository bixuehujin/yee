--TEST--
yii\base\Behavior::events() - basic testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\base\Behavior;
$o = new Behavior();
var_dump($o->events());
?>
--EXPECTF--
array(0) {
}
