<?php

	// /api/v1/user/update.php

	// common setting
	include_once("../../../common.inc");

	$SCOPE = "/api/v1/user/update.php";
	$APIPostProcessor = 
	new APIPostProcessor(
		// $mysql_interface=null
		$mysql_interface
		// $scope=""
		, $SCOPE
	);


	// GET PARAMS
	$EVENT_TYPE = $param->get_param_string($param->EVENT_TYPE);

	$USER_ID_FACEBOOK = $param->get_param_string($param->USER_ID_FACEBOOK);
	$USER_ID_GOOGLE = $param->get_param_string($param->USER_ID_GOOGLE);

	$USER_ID = $param->get_param_number($param->USER_ID, -1);
	$USER_NICKNAME = $param->get_param_string($param->USER_NICKNAME);
	$USER_STATUS = $param->get_param_string($param->USER_STATUS);
	$USER_PERMISSION = $param->get_param_string($param->USER_PERMISSION);
	$USER_QUOTA = $param->get_param_string($param->USER_QUOTA); // FIXME - 0을 null로 변환하는 문제가 있음.
	if(!empty($USER_QUOTA) || strcmp($USER_QUOTA,"0") == 0) {
		$USER_QUOTA = intval($USER_QUOTA);	
	} else {
		$USER_QUOTA = -1;
	}
	
	$USER_EMAIL = $param->get_param_string($param->USER_EMAIL);

	$QUIZ_REGION = $param->get_param_string($param->QUIZ_REGION, "");
	$QUIZ_LANGUAGE = $param->get_param_string($param->QUIZ_LANGUAGE, "");
	$QUIZ_CATEGORY = $param->get_param_string($param->QUIZ_CATEGORY, "");

	$USER_ACCESS_STATUS = $param->get_param_string($param->USER_ACCESS_STATUS, "");

	$FACEBOOK_USER_PROFILE_PICTURE = $param->get_param_string($param->FACEBOOK_USER_PROFILE_PICTURE);
	$FACEBOOK_USER_GENDER = $param->get_param_string($param->FACEBOOK_USER_GENDER);
	$FACEBOOK_USER_LOCALE = $param->get_param_string($param->FACEBOOK_USER_LOCALE);
	$FACEBOOK_USER_AGE_RANGE = $param->get_param_number($param->FACEBOOK_USER_AGE_RANGE, -1);

	$CLIENT_IP = Checker::get_client_ip();
	$CLIENT_OS = Checker::get_client_os();;
	$CLIENT_BROWSER = Checker::get_client_browser();




	// DEBUG
	$REQ_PARAM = new stdClass();
	$REQ_PARAM->{$param->SCOPE} = $SCOPE;
	$REQ_PARAM->{$param->EVENT_TYPE} = $EVENT_TYPE;

	$REQ_PARAM->{$param->USER_ID_FACEBOOK} = $USER_ID_FACEBOOK;
	$REQ_PARAM->{$param->USER_ID_GOOGLE} = $USER_ID_GOOGLE;
		
	$REQ_PARAM->{$param->USER_ID} = $USER_ID;
	$REQ_PARAM->{$param->USER_NICKNAME} = $USER_NICKNAME;
	$REQ_PARAM->{$param->USER_STATUS} = $USER_STATUS;
	$REQ_PARAM->{$param->USER_PERMISSION} = $USER_PERMISSION;
	$REQ_PARAM->{$param->USER_QUOTA} = $USER_QUOTA;
	$REQ_PARAM->{$param->USER_EMAIL} = $USER_EMAIL;

	$REQ_PARAM->{$param->QUIZ_REGION} = $QUIZ_REGION;
	$REQ_PARAM->{$param->QUIZ_LANGUAGE} = $QUIZ_LANGUAGE;
	$REQ_PARAM->{$param->QUIZ_CATEGORY} = $QUIZ_CATEGORY;

	$REQ_PARAM->{$param->USER_ACCESS_STATUS} = $USER_ACCESS_STATUS;

	$REQ_PARAM->{$param->FACEBOOK_USER_PROFILE_PICTURE} = $FACEBOOK_USER_PROFILE_PICTURE;
	$REQ_PARAM->{$param->FACEBOOK_USER_GENDER} = $FACEBOOK_USER_GENDER;
	$REQ_PARAM->{$param->FACEBOOK_USER_LOCALE} = $FACEBOOK_USER_LOCALE;
	$REQ_PARAM->{$param->FACEBOOK_USER_AGE_RANGE} = $FACEBOOK_USER_AGE_RANGE;

	$REQ_PARAM->{$param->CLIENT_IP} = $CLIENT_IP;
	$REQ_PARAM->{$param->CLIENT_OS} = $CLIENT_OS;
	$REQ_PARAM->{$param->CLIENT_BROWSER} = $CLIENT_BROWSER;
	

	if(!empty($USER_ID_FACEBOOK)) {
		$USER_INFO = $mysql_interface->select_user_simple_by_id_facebook($REQ_PARAM);	
	}
	if(!empty($USER_ID_GOOGLE) && is_null($USER_INFO)) {
		$USER_INFO = $mysql_interface->select_user_simple_by_id_google($REQ_PARAM);
	}
	if(!is_null($USER_INFO)) {
		$USER_ID = intval($USER_INFO->__id);
		$REQ_PARAM->{$param->USER_ID} = $USER_ID;
	}
	$REQ_PARAM->{$param->USER_INFO} = $USER_INFO;

	// @ required
	// $REQ_PARAM = $param->get_valid_value_set($REQ_PARAM); // 유효한 값을 가지고 있는 필드만 남기고 모두 제거합니다. / 0을 null로 처리하는 이슈 있음 - wonder.jung
	$feedback_manager->add_custom_key_value($param->REQ_PARAM, $REQ_PARAM);







	// CHECK PARAM VALIDATION - INIT
	$is_not_valid = 
	$param->is_not_valid(
		// $param_std=null
		$REQ_PARAM
		// $key_arr=null
		, array(
			$param->SCOPE
			, $param->EVENT_TYPE
			, $param->USER_INFO
			, $param->USER_ID
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






	if(strcmp($EVENT_TYPE, $param->EVENT_TYPE_USER_INFO_UPDATE) == 0) {

		$APIPostProcessor->pin("1. strcmp(\$EVENT_TYPE, \$param->EVENT_TYPE_USER_INFO_UPDATE) == 0");

		// USER INFO - BEFORE
		$USER_INFO = $mysql_interface->select_user_by_id($REQ_PARAM);
		if(is_null($USER_INFO)) {
			$APIPostProcessor->error("1-1. is_null(\$USER_INFO)");
		}
		$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);

		// CHECK PARAM VALIDATION - INIT
		$is_valid_user_status = 
		$param->is_valid_no_feedback(
			// $key_arr=null
			array(
				$param->USER_STATUS
			)
			// $scope=null
			, $SCOPE . " / $EVENT_TYPE / USER_STATUS"
		);
		if($is_valid_user_status) {
			$APIPostProcessor->pin("1-2. update_user_status");
			$mysql_interface->update_user_status($REQ_PARAM);
		}
		// CHECK PARAM VALIDATION - END


		// CHECK PARAM VALIDATION - INIT
		$is_valid_user_permission = 
		$param->is_valid_no_feedback(
			// $key_arr=null
			array(
				$param->USER_PERMISSION
			)
			// $scope=null
			, $SCOPE . " / $EVENT_TYPE / USER_PERMISSION"
		);
		if($is_valid_user_permission) {
			$APIPostProcessor->pin("1-3. update_user_permission");
			$mysql_interface->update_user_permission($REQ_PARAM);
		}
		// CHECK PARAM VALIDATION - END


		// CHECK PARAM VALIDATION - INIT
		$is_valid_user_nickname = 
		$param->is_valid_no_feedback(
			// $key_arr=null
			array(
				$param->USER_NICKNAME
			)
			// $scope=null
			, $SCOPE . " / $EVENT_TYPE / USER_NICKNAME"
		);
		if($is_valid_user_nickname) {
			$APIPostProcessor->pin("1-4. update_user_nickname");
			$mysql_interface->update_user_nickname($REQ_PARAM);
		}
		// CHECK PARAM VALIDATION - END


		// CHECK PARAM VALIDATION - INIT
		$is_valid_user_quota = 
		$param->is_valid_no_feedback(
			// $key_arr=null
			array(
				$param->USER_QUOTA
			)
			// $scope=null
			, $SCOPE . " / $EVENT_TYPE / USER_QUOTA"
		);
		if($is_valid_user_quota) {
			$APIPostProcessor->pin("1-5. update_user_quota");
			$feedback_manager->add_custom_key_value($param->USER_QUOTA, $REQ_PARAM->{$param->USER_QUOTA});
			$mysql_interface->update_user_quota($REQ_PARAM);
		}
		$feedback_manager->add_custom_key_value("is_valid_user_quota", $is_valid_user_quota);
		$feedback_manager->add_custom_key_value($param->USER_QUOTA, $USER_QUOTA);
		// CHECK PARAM VALIDATION - END

		$USER_INFO_UPDATED = $mysql_interface->select_user_by_id($REQ_PARAM);
		if(is_null($USER_INFO_UPDATED)) {
			$APIPostProcessor->error("1-6. is_null(\$USER_INFO_UPDATED)");
		}
		$feedback_manager->add_custom_key_value($param->USER_INFO_UPDATED, $USER_INFO_UPDATED);

		// ACTION LOG
		$REQ_PARAM->{$param->ACTION_TYPE} = $EVENT_TYPE;
		$REQ_PARAM->{$param->ACTION_MSG} = json_encode($feedback_manager->get());
		$mysql_interface->insert_action_log($REQ_PARAM);

		$APIPostProcessor->pin("1-7. DONE");

	} else if(strcmp($EVENT_TYPE, $param->EVENT_TYPE_USER_LOG_IN) == 0) {

		$APIPostProcessor->pin("2. strcmp(\$EVENT_TYPE, \$param->EVENT_TYPE_USER_LOG_IN) == 0");

		// CHECK PARAM VALIDATION - INIT
		$is_valid_user_facebook_info = 
		$param->is_valid_no_feedback(
			// $key_arr=null
			array(
				$param->USER_ID_FACEBOOK
				, $param->FACEBOOK_USER_PROFILE_PICTURE
				, $param->FACEBOOK_USER_GENDER
				, $param->FACEBOOK_USER_LOCALE
				, $param->FACEBOOK_USER_AGE_RANGE
			)
			// $scope=null
			, $SCOPE . " / $EVENT_TYPE / FACEBOOK_USER_INFO"
		);
		if($is_valid_user_facebook_info) {
			$APIPostProcessor->pin("2-1. update_user_detail_by_facebook");

			// USER INFO - BEFORE
			$USER_INFO = $mysql_interface->select_user_by_id($REQ_PARAM);
			if(is_null($USER_INFO)) {
				$APIPostProcessor->error("2-2. is_null(\$USER_INFO)");
			}
			$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);

			// UPDATE
			$mysql_interface->update_user_detail_by_facebook($REQ_PARAM);

			// USER INFO - AFTER
			$USER_INFO_UPDATED = $mysql_interface->select_user_by_id($REQ_PARAM);
			if(is_null($USER_INFO_UPDATED)) {
				$APIPostProcessor->error("2-3. is_null(\$USER_INFO_UPDATED)");
			}
			$feedback_manager->add_custom_key_value($param->USER_INFO_UPDATED, $USER_INFO_UPDATED);

		}
		// CHECK PARAM VALIDATION - END

		// ACTION LOG
		$REQ_PARAM->{$param->ACTION_TYPE} = $EVENT_TYPE;
		$REQ_PARAM->{$param->ACTION_MSG} = json_encode($feedback_manager->get());
		$mysql_interface->insert_action_log($REQ_PARAM);

		$APIPostProcessor->pin("2. DONE");

	} else if(strcmp($EVENT_TYPE, $param->EVENT_TYPE_USER_CATEGORY_ACCESS_UPDATE) == 0) {

		$APIPostProcessor->pin("3. strcmp(\$EVENT_TYPE, \$param->EVENT_TYPE_USER_CATEGORY_ACCESS_UPDATE) == 0");

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid_user_access_status = 
		$param->is_not_valid_no_feedback(
			// $key_arr=null
			array(
				$param->QUIZ_REGION
				, $param->QUIZ_LANGUAGE
				, $param->QUIZ_CATEGORY
				, $param->USER_ACCESS_STATUS
			)
			// $scope=null
			, $SCOPE . " / $EVENT_TYPE / FACEBOOK_USER_INFO"
		);
		if($is_not_valid_user_access_status) {
			$APIPostProcessor->pin("3-1. update_user_category_access");
			return;
		}
		// CHECK PARAM VALIDATION - END	


		$APIPostProcessor->pin("3-1. update_user_category_access");
		// CHECK - BEFORE
		$CATEGORY_ACCESS_ARR =
		$mysql_interface->select_user_category_access($REQ_PARAM);
		if(is_null($CATEGORY_ACCESS_ARR)) {
			$APIPostProcessor->error("3-2. is_null(\$CATEGORY_ACCESS_ARR)");
		}
		$feedback_manager->add_custom_key_value($param->CATEGORY_ACCESS_ARR, $CATEGORY_ACCESS_ARR);

		// UPDATE
		$APIPostProcessor->pin("3-3. update_user_category_access");
		$mysql_interface->update_user_category_access($REQ_PARAM);

		// CHECK - AFTER
		$CATEGORY_ACCESS_ARR_UPDATED = $mysql_interface->select_user_category_access($REQ_PARAM);
		if(is_null($CATEGORY_ACCESS_ARR_UPDATED)) {
			$APIPostProcessor->error("is_null(\$CATEGORY_ACCESS_ARR_UPDATED)");
		}
		$feedback_manager->add_custom_key_value($param->CATEGORY_ACCESS_ARR_UPDATED, $CATEGORY_ACCESS_ARR_UPDATED);

		// ACTION LOG
		$REQ_PARAM->{$param->ACTION_TYPE} = $EVENT_TYPE;
		$REQ_PARAM->{$param->ACTION_MSG} = json_encode($feedback_manager->get());
		$mysql_interface->insert_action_log($REQ_PARAM);

		$APIPostProcessor->pin("3-4. DONE");

		

	} else if(strcmp($EVENT_TYPE, $param->EVENT_TYPE_USER_CATEGORY_ACCESS_UPDATE_TOGGLE) == 0) {

		$APIPostProcessor->pin("4. strcmp(\$EVENT_TYPE, \$param->EVENT_TYPE_USER_CATEGORY_ACCESS_UPDATE_TOGGLE) == 0");

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid_user_access_status = 
		$param->is_not_valid_no_feedback(
			// $key_arr=null
			array(
				$param->QUIZ_REGION
				, $param->QUIZ_LANGUAGE
				, $param->USER_ACCESS_STATUS
			)
			// $scope=null
			, $SCOPE . " / $EVENT_TYPE / FACEBOOK_USER_INFO"
		);
		if($is_not_valid_user_access_status) {
			$APIPostProcessor->pin("4-1. update_user_category_access");
			return;
		}
		// CHECK PARAM VALIDATION - END	


		$APIPostProcessor->pin("4-1. update_user_category_access");
		// CHECK - BEFORE
		$CATEGORY_ACCESS_ARR =
		$mysql_interface->select_user_category_access($REQ_PARAM);
		if(is_null($CATEGORY_ACCESS_ARR)) {
			$APIPostProcessor->error("4-2. is_null(\$CATEGORY_ACCESS_ARR)");
		}
		$feedback_manager->add_custom_key_value($param->CATEGORY_ACCESS_ARR, $CATEGORY_ACCESS_ARR);

		// UPDATE
		$APIPostProcessor->pin("4-3. toggle_user_category_access");
		$mysql_interface->toggle_user_category_access($REQ_PARAM);

		// CHECK - AFTER
		$CATEGORY_ACCESS_ARR_UPDATED = $mysql_interface->select_user_category_access($REQ_PARAM);
		if(is_null($CATEGORY_ACCESS_ARR_UPDATED)) {
			$APIPostProcessor->error("is_null(\$CATEGORY_ACCESS_ARR_UPDATED)");
		}
		$feedback_manager->add_custom_key_value($param->CATEGORY_ACCESS_ARR_UPDATED, $CATEGORY_ACCESS_ARR_UPDATED);

		// ACTION LOG
		$REQ_PARAM->{$param->ACTION_TYPE} = $EVENT_TYPE;
		$REQ_PARAM->{$param->ACTION_MSG} = json_encode($feedback_manager->get());
		$mysql_interface->insert_action_log($REQ_PARAM);

		$APIPostProcessor->pin("4-4. DONE");


	} else if(strcmp($EVENT_TYPE, $param->EVENT_TYPE_ADMIN_CATEGORY_ACCESS_UPDATE_TOGGLE) == 0) {

		// 운영자로 변경시 모든 언어의 카테고리에 접근할수 있도록 해준다.
		$APIPostProcessor->pin("5. EVENT_TYPE_ADMIN_CATEGORY_ACCESS_UPDATE_TOGGLE");

		$APIPostProcessor->pin("5-1. update_user_category_access");
		// CHECK - BEFORE
		$CATEGORY_ACCESS_ARR =
		$mysql_interface->select_user_category_access($REQ_PARAM);
		if(is_null($CATEGORY_ACCESS_ARR)) {
			$APIPostProcessor->error("5-2. is_null(\$CATEGORY_ACCESS_ARR)");
		}
		$feedback_manager->add_custom_key_value($param->CATEGORY_ACCESS_ARR, $CATEGORY_ACCESS_ARR);

		// UPDATE
		$APIPostProcessor->pin("5-3. toggle_user_category_access");
		$mysql_interface->toggle_on_admin_category_access($REQ_PARAM);

		// CHECK - AFTER
		$CATEGORY_ACCESS_ARR_UPDATED = $mysql_interface->select_user_category_access($REQ_PARAM);
		if(is_null($CATEGORY_ACCESS_ARR_UPDATED)) {
			$APIPostProcessor->error("is_null(\$CATEGORY_ACCESS_ARR_UPDATED)");
		}
		$feedback_manager->add_custom_key_value($param->CATEGORY_ACCESS_ARR_UPDATED, $CATEGORY_ACCESS_ARR_UPDATED);

		// ACTION LOG
		$REQ_PARAM->{$param->ACTION_TYPE} = $EVENT_TYPE;
		$REQ_PARAM->{$param->ACTION_MSG} = json_encode($feedback_manager->get());
		$mysql_interface->insert_action_log($REQ_PARAM);

		$APIPostProcessor->pin("5-4. DONE");

	} else if(strcmp($EVENT_TYPE, $param->EVENT_TYPE_UPDATE_USER_FACEBOOK_ID) == 0) {

		$APIPostProcessor->pin("6. EVENT_TYPE_UPDATE_USER_FACEBOOK_ID");

		// CHECK
		$USER_INFO = $mysql_interface->select_user_by_id($REQ_PARAM);
		$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);		

		// 구글 아이디 설정한뒤, 페북 아이디를 수동으로 업데이트.
		$mysql_interface->update_user_id_facebook($REQ_PARAM);

		// CHECK
		$USER_INFO_UPDATED = $mysql_interface->select_user_by_id($REQ_PARAM);
		$feedback_manager->add_custom_key_value($param->USER_INFO_UPDATED, $USER_INFO_UPDATED);		

		// ACTION LOG
		$REQ_PARAM->{$param->ACTION_TYPE} = $EVENT_TYPE;
		$REQ_PARAM->{$param->ACTION_MSG} = json_encode($feedback_manager->get());
		$mysql_interface->insert_action_log($REQ_PARAM);

		$APIPostProcessor->pin("6-1. DONE");

	} else if(strcmp($EVENT_TYPE, $param->EVENT_TYPE_UPDATE_USER_EMAIL) == 0) {

		$APIPostProcessor->pin("7. EVENT_TYPE_UPDATE_USER_EMAIL");

		// CHECK
		$USER_INFO = $mysql_interface->select_user_by_id($REQ_PARAM);
		$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);

		// TEST
		$feedback_manager->add_custom_key_value($param->USER_EMAIL, $REQ_PARAM->USER_EMAIL);

		// 페북 아이디로 가압하였으나, 이메일이 없어 수동으로 업데이트.
		$mysql_interface->update_user_email($REQ_PARAM);

		// CHECK
		$USER_INFO_UPDATED = $mysql_interface->select_user_by_id($REQ_PARAM);
		$feedback_manager->add_custom_key_value($param->USER_INFO_UPDATED, $USER_INFO_UPDATED);

		// ACTION LOG
		$REQ_PARAM->{$param->ACTION_TYPE} = $EVENT_TYPE;
		$REQ_PARAM->{$param->ACTION_MSG} = json_encode($feedback_manager->get());
		$mysql_interface->insert_action_log($REQ_PARAM);

		$APIPostProcessor->pin("7-1. DONE");

	}

	$feedback_manager->add_custom_key_value($param->REQ_PARAM, $REQ_PARAM);
	$APIPostProcessor->ok(MYSQLFeedback::$FEEDBACK_EVENT_DONE);

?>

