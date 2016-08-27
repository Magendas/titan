<?php

// @ common setting
include_once("../../common.inc");

$preprocessor = 
new TitanPreprocessor(
	// $mysql_interface=null
	$mysql_interface
	// $permission_arr=null / null for everyone
	, array(
		$const->USER_PERMISSION_CODE_MANAGER			// Admin(모든거, Quota 설정)
	)
	, __FILE__
);
$PROPS = $preprocessor->get_props();



?>
<html>
<head>
</head>
<body role="document">

<script>

var PROPS = <?php echo json_encode($PROPS);?>;
console.log("PROPS ::: ",PROPS);

</script>
</body>
</html>
