<?php

	// /api/v1/action/error/update.php

	// common setting
	include_once("../../../common.inc");
	

	$SCOPE = "/api/v1/quiz/error/update.php";
	$APIPostProcessor = 
	new APIPostProcessor(
		// $mysql_interface=null
		$mysql_interface
		// $scope=""
		, $SCOPE
	);






	// GET PARAMS
	// MEETING AGENDA COMMON
	$EVENT_TYPE = $param->get_param_string($param->EVENT_TYPE);
	$ERROR_MSG = $param->get_param_string($param->ERROR_MSG);

	$CLIENT_IP = Checker::get_client_ip();
	$CLIENT_OS = Checker::get_client_os();;
	$CLIENT_BROWSER = Checker::get_client_browser();

	// DEBUG
	$REQ_PARAM->{$param->EVENT_TYPE} = $EVENT_TYPE;
	$REQ_PARAM->{$param->ERROR_MSG} = $ERROR_MSG;

	$REQ_PARAM->{$param->CLIENT_IP} = $CLIENT_IP;
	$REQ_PARAM->{$param->CLIENT_OS} = $CLIENT_OS;
	$REQ_PARAM->{$param->CLIENT_BROWSER} = $CLIENT_BROWSER;

	// @ required
	$REQ_PARAM = $param->get_valid_value_set($REQ_PARAM); // 유효한 값을 가지고 있는 필드만 남기고 모두 제거합니다.
	$feedback_manager->add_custom_key_value($param->REQ_PARAM, $REQ_PARAM);


	// CHECK VALIDATION - INIT
	$is_not_valid = 
	$param->is_not_valid(
		// $param_std=null
		$REQ_PARAM
		// $key_arr=null
		, array(
			$param->SCOPE
			, $param->EVENT_TYPE
			, $param->ERROR_MSG
			, $param->CLIENT_IP
			, $param->CLIENT_OS
			, $param->CLIENT_BROWSER
		)
		// $feedback_manager=null
		, $feedback_manager
		// $scope=null
		, $SCOPE
	);
	if($is_not_valid) {
		$APIPostProcessor->error("\$is_not_valid");
	}
	// CHECK VALIDATION - END	

	if(strcmp($EVENT_TYPE, $param->EVENT_TYPE_INSERT_ERROR_MSG) == 0) {

		// check already registered quiz.
		$mysql_interface->insert_error_log($REQ_PARAM);

	}

	$APIPostProcessor->ok(MYSQLFeedback::$FEEDBACK_EVENT_DONE);

?>

