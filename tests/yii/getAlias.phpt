--TEST--
yii\BaseYii::getAlias() - basic testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php
var_dump(Yii::$aliases); 
Yii::$aliases = array(
 	'@foo' => '/path/to/foo',
 	'@yii' => array(
 		'@yii/web' => '/yii/web',
 		'@yii' => '/path/to/yii',
 	),
);

var_dump(Yii::getAlias('@foo'));
var_dump(Yii::getAlias('@foo/bar'));
var_dump(Yii::getAlias('@foo/bar/bar'));

echo PHP_EOL;
var_dump(Yii::getAlias('@yii/web'));
var_dump(Yii::getAlias('@yii/web/foo'));
var_dump(Yii::getAlias('@yii/foo/bar'));
var_dump(Yii::getAlias('@yii'));

echo PHP_EOL;
var_dump(Yii::getAlias('@yii/webbar'));

echo PHP_EOL;
try {
	var_dump(Yii::getAlias('@bar'));
} catch (Exception $e) {
	echo $e->getMessage();
}
?>
--EXPECTF--
array(0) {
}
string(12) "/path/to/foo"
string(16) "/path/to/foo/bar"
string(20) "/path/to/foo/bar/bar"

string(8) "/yii/web"
string(12) "/yii/web/foo"
string(20) "/path/to/yii/foo/bar"
string(12) "/path/to/yii"

string(19) "/path/to/yii/webbar"

Invalid path alias: @bar
