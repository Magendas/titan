<?php

	// /api/v1/user/select.php

	// common setting
	include_once("../../../common.inc");

	$SCOPE = "/api/v1/user/select.php";
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

	$FACEBOOK_USER_ID = $param->get_param_string($param->FACEBOOK_USER_ID);
	$FACEBOOK_USER_EMAIL = $param->get_param_string($param->FACEBOOK_USER_EMAIL);
	$FACEBOOK_USER_FIRST_NAME = $param->get_param_string($param->FACEBOOK_USER_FIRST_NAME);
	$FACEBOOK_USER_LAST_NAME = $param->get_param_string($param->FACEBOOK_USER_LAST_NAME);

	$GOOGLE_USER_ID = $param->get_param_string($param->GOOGLE_USER_ID);
	$GOOGLE_USER_ID_TO_ENCODE_MD5 = $param->get_param_string($param->GOOGLE_USER_ID_TO_ENCODE_MD5);
	// GOOGLE_USER_ID_TO_ENCODE_MD5
	// google user token을 user id로 받아옵니다. 이 데이터는 1000글자가 넘으므로 DB 인덱싱이 불가능합니다.
	// 그러므로 이 키를 MD5로 해시키로 변경, 사용합니다.
	// 이 해시키가 google user id가 됩니다.
	$GOOGLE_USER_HASH_KEY = "";
	if(!empty($GOOGLE_USER_ID_TO_ENCODE_MD5)) {
		$GOOGLE_USER_ID = $GOOGLE_USER_HASH_KEY = MD5($GOOGLE_USER_ID_TO_ENCODE_MD5);
	}
	$GOOGLE_USER_EMAIL = $param->get_param_string($param->GOOGLE_USER_EMAIL);
	$GOOGLE_USER_FIRST_NAME = $param->get_param_string($param->GOOGLE_USER_FIRST_NAME);
	$GOOGLE_USER_LAST_NAME = $param->get_param_string($param->GOOGLE_USER_LAST_NAME);

	$CLIENT_IP = Checker::get_client_ip();
	$CLIENT_OS = Checker::get_client_os();;
	$CLIENT_BROWSER = Checker::get_client_browser();



	// DEBUG
	$QUERY_PARAM = new stdClass();
	$QUERY_PARAM->{$param->SCOPE} = $SCOPE;
	$QUERY_PARAM->{$param->EVENT_TYPE} = $EVENT_TYPE;

	$QUERY_PARAM->{$param->FACEBOOK_USER_ID} = $FACEBOOK_USER_ID;
	$QUERY_PARAM->{$param->FACEBOOK_USER_EMAIL} = $FACEBOOK_USER_EMAIL;
	$QUERY_PARAM->{$param->FACEBOOK_USER_FIRST_NAME} = $FACEBOOK_USER_FIRST_NAME;
	$QUERY_PARAM->{$param->FACEBOOK_USER_LAST_NAME} = $FACEBOOK_USER_LAST_NAME;

	$QUERY_PARAM->{$param->GOOGLE_USER_ID} = $GOOGLE_USER_ID;
	$QUERY_PARAM->{$param->GOOGLE_USER_ID_TO_ENCODE_MD5} = $GOOGLE_USER_ID_TO_ENCODE_MD5;
	$QUERY_PARAM->{$param->GOOGLE_USER_HASH_KEY} = $GOOGLE_USER_HASH_KEY;
	$QUERY_PARAM->{$param->GOOGLE_USER_EMAIL} = $GOOGLE_USER_EMAIL;
	$QUERY_PARAM->{$param->GOOGLE_USER_FIRST_NAME} = $GOOGLE_USER_FIRST_NAME;
	$QUERY_PARAM->{$param->GOOGLE_USER_LAST_NAME} = $GOOGLE_USER_LAST_NAME;

	$QUERY_PARAM->{$param->CLIENT_IP} = $CLIENT_IP;
	$QUERY_PARAM->{$param->CLIENT_OS} = $CLIENT_OS;
	$QUERY_PARAM->{$param->CLIENT_BROWSER} = $CLIENT_BROWSER;

	// @ required
	$QUERY_PARAM = $param->get_valid_value_set($QUERY_PARAM);
	$feedback_manager->add_custom_key_value($param->QUERY_PARAM, $QUERY_PARAM);





	$APIPostProcessor->pin("0. CHECK PARAM VALIDATION - INIT");
	$is_not_valid = 
	$param->is_not_valid(
		// $param_std=null
		$QUERY_PARAM
		// $key_arr=null
		, array(
			$param->SCOPE
			, $param->EVENT_TYPE
		)
		// $feedback_manager=null
		, $feedback_manager
		// $scope=null
		, $SCOPE
	);
	if($is_not_valid) {
		$APIPostProcessor->error(
			// $reason=""
			"\$is_not_valid"
			// $extra_data=null
			, $QUERY_PARAM
		);
	}
	$APIPostProcessor->pin("0. CHECK PARAM VALIDATION - END");

	// 1. 로그인 프로세스 정의
	// 1-1. 유저는 구글, 또는 페이스북으로 로그인하여 계정을 자동으로 생성할 수 있습니다.
	// 1-2. 유저가 구글, 페이스북 양쪽에서 email 주소를 가져올 수 있다면, 
	// 구글, 페이스북로그인시 동일인으로 등록하여 로그인이 가능하게 됩니다.

	// 1-3. 유저가 구글 또는 페이스북에서 이메일 주소를 가져오기가 불가능한 경우, 
	// 운영자에게 요청하여 등록되지 않은 플랫폼의 user id를 등록하여 동일인으로 로그인 할 수 있습니다.

	// - 구글 : 운영자에게 gmail 계정을 등록 요청, 매칭합니다. 만일 다른 email의 계정이라면 다른 유저로 등록됩니다.
	// - 페이스북 : 운영자에게 facebook 계정을 등록 요청, 매칭합니다. 만일 다른 email의 계정이라면 다른 유저로 등록됩니다.

	// ex> 페이스북 아이디찾기 - http://findmyfbid.com/success/100010608246259

	if(strcmp($EVENT_TYPE, $param->IS_SELECT_USER) == 0) {

		$APIPostProcessor->pin("1. strcmp(\$EVENT_TYPE, \$param->IS_SELECT_USER) == 0");
		$APIPostProcessor->pin("1. FACEBOOK_USER_ID");
		$is_valid_facebook_user_id = 
		$param->is_valid_no_feedback(
			// $key_arr=null
			array(
				$param->FACEBOOK_USER_ID
				, $param->FACEBOOK_USER_FIRST_NAME
			)
			// $scope=null
			, $SCOPE
		);
		if($is_valid_facebook_user_id) {

			// FACEBOOK ACCOUNT

			$APIPostProcessor->pin("1-1. \$is_valid_facebook_user_id");

			// 전달 받은 facebook id로 등록된 유저인지 확인합니다.

			$USER_INFO = $mysql_interface->select_user_by_fb_id($QUERY_PARAM);
			$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);

			if(!is_null($USER_INFO)) {
				// 0. 페이스북 id로 조회, 회원 정보가 있다면, 바로 로그인.
				$APIPostProcessor->pin("1-2. User has facebook account in service. log in!");

			} else if(!empty($FACEBOOK_USER_EMAIL)) {
				$APIPostProcessor->pin("1-3-1. User has no facebook account. but has email.");
				// 1. 페이스북 id로 조회, 회원 정보가 없음.
				// 1. 페이스북에 등록된 이메일이 있음. 
				// 1. 페이스북 등록된 이메일 주소로  매칭되는 google_id가 있는지 확인합니다.
				// 1. 페이스북 등록된 이메일과 동일한 이메일을 가지는 google id가 있다면 해당 유저에 facebook id 정보를 추가합니다.

				$QUERY_PARAM->{$param->USER_EMAIL} = $FACEBOOK_USER_EMAIL;

				// 이미 등록된 구글 계정 email은 없는지 확인.
				$user_info_on_google = $mysql_interface->select_user_by_email($QUERY_PARAM);
				if(!is_null($user_info_on_google)) {
					$APIPostProcessor->pin("1-3-2. User has no facebook account. but has email.");
					// 해당 이메일로 등록된 구글 계정을 찾았습니다. 
					// 해당 유저의 facebook id를 업데이트합니다.
					$QUERY_PARAM->{$param->USER_ID} = intval($user_info_on_google->__id);
					$mysql_interface->update_user_fb_id($QUERY_PARAM);

					// CHECK
					$USER_INFO = $mysql_interface->select_user_by_fb_id($QUERY_PARAM);
					$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);

					// ACTION LOG
					$ACTION_MSG = json_encode($feedback_manager->get());
					$QUERY_PARAM->{$param->ACTION_TYPE} = $param->ACTION_TYPE_ADD_FACEBOOK_ACCOUNT;
					$QUERY_PARAM->{$param->ACTION_MSG} = $ACTION_MSG;
					$QUERY_PARAM->{$param->USER_ID} = intval($USER_INFO->__id);
					$mysql_interface->insert_action_log($QUERY_PARAM);

				} else {
					// 해당 이메일로 등록된 구글 계정이 없습니다. 
					// 신규 회원등록.
					
					// 유저 등록시 바로 일반상식은 풀수 있게 세팅
					$APIPostProcessor->pin("1-3-3. Add category access G_COMMON");
					$CATEGORY_ACCESS_ARR = $param->get_initial_user_access_token();
					$QUERY_PARAM->{$param->CATEGORY_ACCESS_ARR} = $CATEGORY_ACCESS_ARR;

					$APIPostProcessor->pin("1-3-4. Register service user with facebook account");
					$mysql_interface->insert_user_from_fb_user($QUERY_PARAM);

					// CHECK
					$USER_INFO = $mysql_interface->select_user_by_fb_id($QUERY_PARAM);
					$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);

					// ACTION LOG
					$ACTION_MSG = json_encode($feedback_manager->get());
					$QUERY_PARAM->{$param->ACTION_TYPE} = $param->ACTION_TYPE_INSERT_NEW_USER_ON_FACEBOOK;
					$QUERY_PARAM->{$param->ACTION_MSG} = $ACTION_MSG;
					$QUERY_PARAM->{$param->USER_ID} = intval($USER_INFO->__id);
					$mysql_interface->insert_action_log($QUERY_PARAM);
				}

			} else if(empty($FACEBOOK_USER_EMAIL)) {
				$APIPostProcessor->pin("1-4-1. User has no facebook account and has no email as well.");
				// 2. 페이스북 id로 조회, 회원 정보가 없음.
				// 2. 페이스북에 등록된 이메일이 없음. 
				// 2. 페이스북 아이디로만 유저 등록. 
				// 2. 유저가 요청할 경우, 운영툴을 통해 google id - gmail 계정을 등록.(수동으로 처리.)

				// 신규 회원등록.
				$APIPostProcessor->pin("1-4-2. Add category access G_COMMON");
				$CATEGORY_ACCESS_ARR = $param->get_initial_user_access_token();
				$QUERY_PARAM->{$param->CATEGORY_ACCESS_ARR} = $CATEGORY_ACCESS_ARR;

				$APIPostProcessor->pin("1-4-3. Register service user with facebook account");
				$mysql_interface->insert_user_from_fb_user($QUERY_PARAM);

				// CHECK
				$USER_INFO = $mysql_interface->select_user_by_fb_id($QUERY_PARAM);
				$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);

				// ACTION LOG
				$ACTION_MSG = json_encode($feedback_manager->get());
				$QUERY_PARAM->{$param->ACTION_TYPE} = $param->ACTION_TYPE_INSERT_NEW_USER_ON_FACEBOOK;
				$QUERY_PARAM->{$param->ACTION_MSG} = $ACTION_MSG;
				$QUERY_PARAM->{$param->USER_ID} = intval($USER_INFO->__id);
				$mysql_interface->insert_action_log($QUERY_PARAM);

			}

		} else {

			// GOOGLE ACCOUNT

			$APIPostProcessor->pin("2. GOOGLE_USER_ID");
			$is_valid_google_user_hash_key = 
			$param->is_valid_no_feedback(
				// $key_arr=null
				array(
					$param->GOOGLE_USER_HASH_KEY
					, $param->GOOGLE_USER_FIRST_NAME
					, $param->GOOGLE_USER_EMAIL
				)
				// $scope=null
				, $SCOPE
			);
			if($is_valid_google_user_hash_key) {

				$APIPostProcessor->pin("2-1. \$is_valid_google_user_hash_key");

				// 구글 계정은 반드시 gmail 주소를 가집니다.
				// gmail 주소를 키로 사용합니다.

				$QUERY_PARAM->{$param->USER_EMAIL} = $GOOGLE_USER_EMAIL;
				$USER_INFO = $mysql_interface->select_user_by_email($QUERY_PARAM);

				if(!is_null($USER_INFO)) {
					$APIPostProcessor->pin("2-2. User has google account in service. log in!");
					// 0. gmail로 조회, 회원 정보가 있다면, 바로 로그인.

					if(!empty($USER_INFO->__fb_id) && empty($USER_INFO->__google_id)) {
						// 페이스북 계정의 메일 정보로 로그인된 경우. 
						// 아직 구글 계정 등록은 안되있음. 구글 계정 등록함.
						$QUERY_PARAM->{$param->USER_ID} = intval($USER_INFO->__id);
						$mysql_interface->update_user_google_id($QUERY_PARAM);
					}

					// CHECK
					$USER_INFO = $mysql_interface->select_user_by_email($QUERY_PARAM);
					$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);					
					
				} else {
					
					$APIPostProcessor->pin("2-3-1. User has no google account in service. but has email.");
					// 1. gmail로 id로 조회, 회원 정보가 없음.
					// 1. gmail로 등록된 이메일 주소로  매칭되는 facebook_id가 있는지 확인합니다.
					// 1. gmail로 등록된 이메일과 동일한 이메일을 가지는 facebook_id가 있다면 해당 유저에 google id 정보를 추가합니다.
					// (이메일은 이미 동일한 주소로 등록.)
					$user_info_on_facebook = $mysql_interface->select_user_by_email($QUERY_PARAM);
					if(!is_null($user_info_on_facebook)) {
						$APIPostProcessor->pin("2-3-2. Has found facebook account in service with google email.");
						// 해당 이메일로 등록된 페이스북 계정을 찾았습니다. 
						// 해당 유저의 facebook id를 업데이트합니다.
						$QUERY_PARAM->{$param->USER_ID} = intval($user_info_on_facebook->__id);
						$mysql_interface->update_user_google_id($QUERY_PARAM);

						// CHECK
						$USER_INFO = $mysql_interface->select_user_by_email($QUERY_PARAM);
						$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);

						// ACTION LOG
						$ACTION_MSG = json_encode($feedback_manager->get());
						$QUERY_PARAM->{$param->ACTION_TYPE} = $param->ACTION_TYPE_ADD_GOOGLE_ACCOUNT;
						$QUERY_PARAM->{$param->ACTION_MSG} = $ACTION_MSG;
						$QUERY_PARAM->{$param->USER_ID} = intval($USER_INFO->__id);
						$mysql_interface->insert_action_log($QUERY_PARAM);

					} else {
						$APIPostProcessor->pin("2-3-3. No facebook account in service with google email.");
						// 해당 이메일로 등록된 구글 계정이 없습니다. 
						// 신규 회원등록.
						
						// UPDATE CATEGORY ACCESS
						// 유저 등록시 바로 일반상식은 풀수 있게 세팅
						$APIPostProcessor->pin("2-3-4. add category access G_COMMON");
						$CATEGORY_ACCESS_ARR = $param->get_initial_user_access_token();
						$QUERY_PARAM->{$param->CATEGORY_ACCESS_ARR} = $CATEGORY_ACCESS_ARR;

						$APIPostProcessor->pin("2-3-5. Register service user with google account");
						$mysql_interface->insert_user_from_google_user($QUERY_PARAM);

						// CHECK
						$USER_INFO = $mysql_interface->select_user_by_email($QUERY_PARAM);
						$feedback_manager->add_custom_key_value($param->USER_INFO, $USER_INFO);

						// ACTION LOG
						$ACTION_MSG = json_encode($feedback_manager->get());
						$QUERY_PARAM->{$param->ACTION_TYPE} = $param->ACTION_TYPE_INSERT_NEW_USER_ON_GOOGLE;
						$QUERY_PARAM->{$param->ACTION_MSG} = $ACTION_MSG;
						$QUERY_PARAM->{$param->USER_ID} = intval($USER_INFO->__id);
						$mysql_interface->insert_action_log($QUERY_PARAM);

					} // end inner if


				} // end outer if

			} else {

				$APIPostProcessor->pin("2-2. \$is_not_valid_google_user_hash_key");

			} // end if

		} // end if

	}
	// @ required
	$QUERY_PARAM = $param->get_valid_value_set($QUERY_PARAM); // 유효한 값을 가지고 있는 필드만 남기고 모두 제거합니다.
	$feedback_manager->add_custom_key_value($param->QUERY_PARAM, $QUERY_PARAM);

	$APIPostProcessor->ok(MYSQLFeedback::$FEEDBACK_EVENT_DONE);
?>

