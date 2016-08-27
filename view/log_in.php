<?php

// @ common setting
include_once("../common.inc");

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
<?php
	// @ required
	ViewRenderer::render(
		$PROPS->SERVICE_PATH->HEAD_FILE_PATH
		,$PROPS->SERVICE_VIEW->HEAD_VIEW_RENDER_VAR_ARR_FORCE_PC_VIEW
	);
?>
</head>
<body role="document">

	<!-- KAKAO LOGIN BUTTON - INIT -->
	<a id="custom-login-btn" href="javascript:login_with_kakao()">
		<img src="/titan/images/login/log_in_button_kakao.png" width="205"/>
	</a><br/><br/>
	<!-- KAKAO LOGIN BUTTON - DONE -->

	<!-- FACEBOOK LOGIN BUTTON - INIT -->
	<a id="custom-login-btn" href="javascript:login_with_facebook()">
		<img src="/titan/images/login/log_in_button_facebook.png" width="390"/>
	</a>
	<!-- FACEBOOK LOGIN BUTTON - DONE -->

	

<script>

var PROPS = <?php echo json_encode($PROPS);?>;
console.log("PROPS ::: ",PROPS);

</script>
</body>
</html>
