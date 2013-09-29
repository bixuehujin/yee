--TEST--
yii\BaseYii::configure() - arguments testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php 
use yii\BaseYii;
BaseYii::configure();
BaseYii::configure(new stdClass, 3);
BaseYii::configure(null, 3);
?>
--EXPECTF--
Warning: yii\BaseYii::configure() expects exactly 2 parameters, 0 given in %s on line %d

Warning: Object configuration must be an array, integer given in %s on line %d

Fatal error: Only object can be configured, null given in %s on line %d
