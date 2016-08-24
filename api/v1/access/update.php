<?php

	// /api/v1/action/access/update.php

	// common setting
	include_once("../../../common.inc");

	$APIPostProcessor = 
	new APIPostProcessor(
		// $mysql_interface=null
		$mysql_interface
		// $scope=""
		, __FILE__
	);

	
	// GET PARAMS
	// MEETING AGENDA COMMON
	$EVENT_TYPE = $param->get_param_string($param->EVENT_TYPE);
	$ACCESS_MSG = $param->get_param_string($param->ACCESS_MSG);

	$CLIENT_IP = Checker::get_client_ip();
	$CLIENT_OS = Checker::get_client_os();;
	$CLIENT_BROWSER = Checker::get_client_browser();

	// DEBUG
	$QUERY_PARAM = new stdClass();
	$QUERY_PARAM->{$param->EVENT_TYPE} = $EVENT_TYPE;
	$QUERY_PARAM->{$param->ACCESS_MSG} = $ACCESS_MSG;

	$QUERY_PARAM->{$param->CLIENT_IP} = $CLIENT_IP;
	$QUERY_PARAM->{$param->CLIENT_OS} = $CLIENT_OS;
	$QUERY_PARAM->{$param->CLIENT_BROWSER} = $CLIENT_BROWSER;

	// @ required
	$QUERY_PARAM = $param->get_valid_value_set($QUERY_PARAM);
	$feedback_manager->add_custom_key_value($param->QUERY_PARAM, $QUERY_PARAM);






	// CHECK PARAM VALIDATION - INIT
	$is_not_valid = 
	$param->is_not_valid(
		// $param_std=null
		$QUERY_PARAM
		// $key_arr=null
		, array(
			$param->EVENT_TYPE
			, $param->ACCESS_MSG
		)
		// $feedback_manager=null
		, $feedback_manager
		// $scope=null
		, __FILE__
	);
	if($is_not_valid) {
		$APIPostProcessor->error("\$is_not_valid");
	}
	// CHECK VALIDATION - END	





	if(strcmp($EVENT_TYPE, $param->EVENT_TYPE_INSERT_ACCESS_MSG) == 0) {
		$APIPostProcessor->pin("1. insert_access_log");
		// check already registered quiz.
		$mysql_interface->insert_access_log($QUERY_PARAM);

	}

	$APIPostProcessor->ok(MYSQLFeedback::$FEEDBACK_EVENT_DONE);

?>

