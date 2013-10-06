--TEST--
yii\BaseYii::setAlias() - basic testing
--SKIPIF--
<?php if (!extension_loaded("yee")) print "skip"; ?>
--FILE--
<?php
Yii::setAlias('@yii', '/path/to/yii');
Yii::setAlias('foo/bar', '/path/to/foo/bar//');
Yii::setAlias('@yii/web', '/path/to/web');
Yii::setAlias('@yii', '/path/to/other/yii');
Yii::setAlias('@yii/web/asset', '/path/to/asset');
Yii::setAlias('@aliasofyii', '@yii/bar');
var_dump(Yii::$aliases);

Yii::setAlias('@yii', null);
Yii::setAlias('@yii/web', null);
Yii::setAlias('@aliasofyii', null);

var_dump(Yii::$aliases);

try {
	Yii::setAlias('@hoho', '@bala/bala');
}catch(Exception $e) {
	echo $e->getMessage();
}
?>
--EXPECTF--
array(3) {
  ["@yii"]=>
  array(3) {
    ["@yii/web/asset"]=>
    string(14) "/path/to/asset"
    ["@yii/web"]=>
    string(12) "/path/to/web"
    ["@yii"]=>
    string(18) "/path/to/other/yii"
  }
  ["@foo"]=>
  array(1) {
    ["@foo/bar"]=>
    string(16) "/path/to/foo/bar"
  }
  ["@aliasofyii"]=>
  string(22) "/path/to/other/yii/bar"
}
array(2) {
  ["@yii"]=>
  array(1) {
    ["@yii/web/asset"]=>
    string(14) "/path/to/asset"
  }
  ["@foo"]=>
  array(1) {
    ["@foo/bar"]=>
    string(16) "/path/to/foo/bar"
  }
}
Invalid path alias: @bala/bala
