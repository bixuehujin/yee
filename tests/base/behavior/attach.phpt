--TEST--
yii\base\Behavior::attach() - basic testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\base\Behavior;

class Handler {
	public function on($event, $handler) {
		var_dump($event);
		var_dump(get_class($handler[0]), $handler[1]);
	}
}

class Test extends Behavior {
	public function events() {
		return array('onRead' => 'handleRead');
	}
}

$o = new Test();
var_dump($o->owner);

$o->attach(new Handler());

var_dump($o->owner);
?>
--EXPECTF--
NULL
string(6) "onRead"
string(4) "Test"
string(10) "handleRead"
object(Handler)#2 (0) {
}
