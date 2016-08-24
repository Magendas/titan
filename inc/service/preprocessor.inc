<?php

class TitanPreprocessor{ 

	private $version="v0.520160823";
	private $param;
	public function TitanPreprocessor($mysql_interface=null, $param=null, $permission_arr=null) {

		$service_root_path = TitanLinkManager::get_service_root_path();
		$file_root_path = TitanLinkManager::get_file_root_path();

		$this->param = ParamManager::get();

		$this->set_props(
			// $mysql_interface=null
			$mysql_interface
			// $service_root_path=null
			, $service_root_path
			// $file_root_path=null
			, $file_root_path
			// $permission_arr=null
			, $permission_arr
		);

	}


	// PAGENATION - INIT
	private $pagenation_std;
	private $total_cnt;
	public function get_pagination_tag() {

		if(is_null($this->pagenation_std)) {
			return;
		}

		return $this->pagenation_std->html_bootstrap_tag;
	}
	public function set_pagination_total_cnt($total_cnt=-1) {

		if(!(-1 < $total_cnt)) {
			return;
		}
		$this->total_cnt = $total_cnt;

		$props = $this->get_props();
		if(is_null($props)) {
			return;
		}

		$param = $this->param;
		if(is_null($param)) {
			return;
		}
		$const = $param->get_service_const();
		if(is_null($const)) {
			$this->set_prop_fail($result, "is_null(\$const)");
			return;
		}
		$var_name = $param->get_service_var_name();
		if(is_null($var_name)) {
			$this->set_prop_fail($result, "is_null(\$var_name)");
			return;
		}

		$this->pagenation_std = 
		PageManager::get_pagenation(
			// $total_row_cnt=-1
			$total_cnt
			// $cursor_page_num=PageManager::$CURSOR_PAGE_NUM_DEFAULT
			, $props->{$var_name->PAGE_NUM}
			// $row_cnt_per_page=PageManager::$ROW_CNT_DEFAULT
			, $props->{$var_name->ROW_CNT}
			// $page_cnt_on_pagination=PageManager::$PAGE_RANGE_DEFAULT
			, $props->{$var_name->PAGE_RANGE}
		);
		$props->{$var_name->PAGINATION} = $this->pagenation_std;

	}
	// PAGENATION - DONE


	private $props;
	public function get_props() {
		return $this->props;
	}
	private function set_prop_success($props=null, $error_msg="") {

		if(is_null($props)) {
			return;
		}

		$param = $this->param;
		$const = $param->get_service_const();
		$var_name = $param->get_service_var_name();

		$props->{$var_name->SUCCESS} = true;
		$props->{$var_name->ERROR} = $error_msg;

		$this->props = $props;

	}
	private function set_prop_fail($props=null, $error_msg="") {

		if(is_null($props)) {
			return;
		}

		$param = $this->param;
		$const = $param->get_service_const();
		$var_name = $param->get_service_var_name();		

		$props->{$var_name->SUCCESS} = false;
		$props->{$var_name->ERROR} = $error_msg;

		$this->props = $props;

	}

	private function set_props($mysql_interface=null, $service_root_path=null, $file_root_path=null, $permission_arr=null){

		// 1. 건네받은 파라미터가 유효한 경우에만 프로퍼티로 등록합니다.
		$result = new stdClass();

		$param = $this->param;
		$const = $param->get_service_const();
		if(is_null($const)) {
			$this->set_prop_fail($result, "is_null(\$const)");
			return;
		}
		$var_name = $param->get_service_var_name();
		if(is_null($var_name)) {
			$this->set_prop_fail($result, "is_null(\$var_name)");
			return;
		}
		$result->{$var_name->SERVICE_VAR_NAME} = $var_name;
		$result->{$var_name->SERVICE_CONST} = $const;

		// STATUS
		$result->{$var_name->SUCCESS} = true;
		$result->{$var_name->ERROR} = $const->ERROR_MSG_NO_ERROR;
		$result->{$var_name->VERSION} = $this->version;

		if(is_null($mysql_interface)) {
			$this->set_prop_fail($result, "is_null(\$mysql_interface)");
			return;
		}

		$mysql_feedback = $mysql_interface->get_mysql_feedback();
		if(is_null($mysql_feedback)) {
			$this->set_prop_fail($result, "is_null(\$mysql_feedback)");
			return;
		}

		// SERVICE_ROOT_PATH can be empty!
		$result->{$var_name->SERVICE_ROOT_PATH} = $service_root_path;

		if(empty($file_root_path)) {
			$this->set_prop_fail($result, "empty(\$file_root_path)");
			return;
		}
		$result->{$var_name->FILE_ROOT_PATH} = $file_root_path;

		if(is_null($permission_arr)) {
			$permission_arr = array();
		}

		// 0. CHECK AGENT / MOBILE OR PC?
		$user_agent = strtolower($_SERVER['HTTP_USER_AGENT']);
		$IS_MOBILE = false;
		if ((strpos($user_agent,'iphone') !== false) || (strpos($user_agent,'android') !== false)) {
			$IS_MOBILE = true;
		}	
		$result->{$var_name->IS_MOBILE} = $IS_MOBILE;



		// HEAD_FILE_PATH - DEFAULT
		$HEAD_FILE_PATH = "$file_root_path/template/head.include.template";
		$result->{$var_name->HEAD_FILE_PATH} = $HEAD_FILE_PATH;

		$view_port = $const->DEVICE_REPONSIVE_VIEW_PORT;
		$HEAD_VIEW_RENDER_VAR_ARR = 
		array(
			"[__ROOT_PATH__]"=>$service_root_path
			, "[__VIEW_PORT__]"=>$view_port
		);
		$result->{$var_name->HEAD_VIEW_RENDER_VAR_ARR} = $HEAD_VIEW_RENDER_VAR_ARR;

		$view_port = $const->DEVICE_REPONSIVE_VIEW_PORT;
		if($IS_MOBILE) {
			$view_port = $const->FORCE_PC_VIEW_PORT;
		}
		$HEAD_VIEW_RENDER_VAR_ARR_FORCE_PC_VIEW = 
		array(
			"[__ROOT_PATH__]"=>$service_root_path
			, "[__VIEW_PORT__]"=>$view_port
		);
		$result->{$var_name->HEAD_VIEW_RENDER_VAR_ARR_FORCE_PC_VIEW} = $HEAD_VIEW_RENDER_VAR_ARR_FORCE_PC_VIEW;

		// NAV_FILE_PATH - DEFAULT
		$NAV_FILE_PATH = "$file_root_path/template/nav.user.template";
		$result->{$var_name->NAV_FILE_PATH} = $NAV_FILE_PATH;

		// DEBUG
		// $result->{$var_name->IS_DEBUG_MODE} = false;
		$result->{$var_name->IS_DEBUG_MODE} = true;

		$IS_LOG_OUT = $param->get_param_boolean($var_name->IS_LOG_OUT);
		if($IS_LOG_OUT) {
			$this->set_prop_fail($result, "\$IS_LOG_OUT:$IS_LOG_OUT");
			return;
		}


		// 아래와 같은 사전 작업을 진행합니다. 

		// 1. Log in check with cookie
		// FACEBOOK ID? or GOOGLE ID?
		$USER_INFO = null;

		$FACEBOOK_USER_ID = $param->get_param_string($var_name->FACEBOOK_USER_ID);
		$GOOGLE_USER_ID = $param->get_param_string($var_name->GOOGLE_USER_ID);
		$KAKAO_USER_ID = $param->get_param_string($var_name->KAKAO_USER_ID);

		$QUERY_PARAM = new stdClass();
		$result->{$var_name->QUERY_PARAM} = $QUERY_PARAM;

		// 1-1. 쿠키에서 FACEBOOK_USER_ID로 유저 정보를 가져옵니다.
		$COOKIE_LOGIN_FACEBOOK = CookieManager::getCookie($var_name->COOKIE_LOGIN_FACEBOOK);
		if(!empty($COOKIE_LOGIN_FACEBOOK)) {

			$result->{$var_name->COOKIE_LOGIN_FACEBOOK} = $COOKIE_LOGIN_FACEBOOK;

			$QUERY_PARAM->{$var_name->FACEBOOK_USER_ID} = $COOKIE_LOGIN_FACEBOOK;
			$USER_INFO = $mysql_interface->select_user_by_fb_id($QUERY_PARAM);

			if(!is_null($USER_INFO)) {
				$FACEBOOK_USER_ID = $COOKIE_LOGIN_FACEBOOK;
			} 
		}
		// 1-2. 파라미터로 전달받은 FACEBOOK_USER_ID로 유저 정보를 가져옵니다.
		if(is_null($USER_INFO) && !empty($FACEBOOK_USER_ID)) {
			$QUERY_PARAM->{$var_name->FACEBOOK_USER_ID} = $FACEBOOK_USER_ID;
			$USER_INFO = $mysql_interface->select_user_by_fb_id($QUERY_PARAM);
		}
		// 1-3. 파라미터로 전달받은 FACEBOOK_USER_ID로 유저 정보를 가져옵니다.
		if(!empty($FACEBOOK_USER_ID)) {
			$result->{$var_name->FACEBOOK_USER_ID} = $FACEBOOK_USER_ID;
		}


		// 2-1. 쿠키에서 GOOGLE_USER_ID로 유저 정보를 가져옵니다.
		$COOKIE_LOGIN_GOOGLE = CookieManager::getCookie($var_name->COOKIE_LOGIN_GOOGLE);
		if(!empty($COOKIE_LOGIN_GOOGLE)) {

			$result->{$var_name->COOKIE_LOGIN_GOOGLE} = $COOKIE_LOGIN_GOOGLE;

			$QUERY_PARAM->{$var_name->GOOGLE_USER_ID} = $COOKIE_LOGIN_GOOGLE;
			$USER_INFO = $mysql_interface->select_user_by_google_id($QUERY_PARAM);

			if(!is_null($USER_INFO)) {
				$GOOGLE_USER_ID = $COOKIE_LOGIN_GOOGLE;
			} 
		}
		// 2-2. 파라미터로 전달받은 GOOGLE_USER_ID 유저 정보를 가져옵니다.
		if(is_null($USER_INFO) && !empty($GOOGLE_USER_ID)) {
			$QUERY_PARAM->{$var_name->GOOGLE_USER_ID} = $GOOGLE_USER_ID;
			$USER_INFO = $mysql_interface->select_user_by_google_id($QUERY_PARAM);
		}
		// 2-3.
		if(!empty($GOOGLE_USER_ID)) {
			$result->{$var_name->GOOGLE_USER_ID} = $GOOGLE_USER_ID;	
		}



		// 1-3. 쿠키에서 KAKAO_USER_ID로 유저 정보를 가져옵니다.
		$COOKIE_LOGIN_KAKAO = CookieManager::getCookie($var_name->COOKIE_LOGIN_KAKAO);
		if(!empty($COOKIE_LOGIN_KAKAO)) {

			$result->{$var_name->COOKIE_LOGIN_KAKAO} = $COOKIE_LOGIN_KAKAO;

			$QUERY_PARAM->{$var_name->KAKAO_USER_ID} = $COOKIE_LOGIN_KAKAO;
			$USER_INFO = $mysql_interface->select_user_by_google_id($QUERY_PARAM);

			if(!is_null($USER_INFO)) {
				$KAKAO_USER_ID = $COOKIE_LOGIN_KAKAO;
			} 
		}
		// 2-2. 파라미터로 전달받은 KAKAO_USER_ID 유저 정보를 가져옵니다.
		if(is_null($USER_INFO) && !empty($KAKAO_USER_ID)) {
			$QUERY_PARAM->{$var_name->KAKAO_USER_ID} = $KAKAO_USER_ID;
			$USER_INFO = $mysql_interface->select_user_by_kakao_id($QUERY_PARAM);
		}
		// 2-3.
		if(!empty($KAKAO_USER_ID)) {
			$result->{$var_name->KAKAO_USER_ID} = $KAKAO_USER_ID;	
		}

		if(!is_null($USER_INFO)) {
			$result->{$var_name->USER_INFO} = $USER_INFO;	
		}

		// Not the facebook user nor google user. Back to the log in page.
		if(is_null($USER_INFO)) {
			$this->set_prop_fail($result, $const->REASON_NO_USER);
			return;
		}

		// 2. user status check
		if(Checker::is_same_str($USER_INFO->__status, $const->USER_STATUS_NOT_IN_ACTION)) {
			$this->set_prop_fail($result, $const->REASON_NOT_VALID_USER_STATUS);
			return;
		}

		// 4. permission check 
		$USER_ID = intval($USER_INFO->__id);
		if(!empty($USER_ID)) {
			$result->{$var_name->USER_ID} = $USER_ID;
		}

		// 5. USER STATUS
		$USER_STATUS = 
		$param->get_param_string(
			// $tossed_key
			$var_name->USER_STATUS
			// $placeholder=null
			, $const->USER_STATUS_AVAILABLE
		);
		$result->{$var_name->USER_STATUS} = $USER_STATUS;

		$USER_PERMISSION = $USER_INFO->__permission;
		if(!empty($USER_PERMISSION)) {
			$result->{$var_name->USER_PERMISSION} = $USER_PERMISSION;	
		}

		$is_valid_permission = false;
		if(empty($permission_arr)) {
			// 퍼미션 지정을 하지 않았따면 모든 접근 가능.
			$is_valid_permission = true;			
		} else {
			for($idx=0; $idx < count($permission_arr); $idx++) {
				$permission = $permission_arr[$idx];
				if(strcmp($permission, $USER_PERMISSION) == 0) {
					$is_valid_permission = true;
					break;
				}
			}
		}
		
		if(!$is_valid_permission) {
			$this->set_prop_fail($result, $const->REASON_NOT_ALLOWED_PERMISSION);
			return;
		}

		// wonder.jung - 이런 작업을 같이 묶을 수 있는 곳은 어디?
		$USER_PERMISSION_ARR = $const->USER_PERMISSION;
		$USER_PERMISSION_NAME = "";
		for ($i=0; $i < count($USER_PERMISSION_ARR); $i++) { 
			$USER_PERMISSION_OBJ = $USER_PERMISSION_ARR[$i];

			if(Checker::is_same_str($USER_PERMISSION_OBJ->{$var_name->CODE}, $USER_PERMISSION)) {
				$USER_PERMISSION_NAME = $USER_PERMISSION_OBJ->{$var_name->NAME};
				break;
			}
		}
		if(!empty($USER_PERMISSION_NAME)) {
			$result->{$var_name->USER_PERMISSION_NAME} = $USER_PERMISSION_NAME;	
		}

		// user name to display
		$USER_NAME = $USER_INFO->__first_name . " " . $USER_INFO->__last_name . " ($USER_PERMISSION_NAME)";
		if($IS_MOBILE) {
			$USER_NAME = $USER_INFO->__first_name;	
		}
		if(!empty($USER_NAME)) {
			$result->{$var_name->USER_NAME} = $USER_NAME;
		}
		$USER_NICKNAME = $USER_INFO->__nickname;
		if(!empty($USER_NICKNAME)) {
			$result->{$var_name->USER_NICKNAME} = $USER_NICKNAME;
		} else {
			$USER_NICKNAME = $USER_INFO->__first_name;
		}
		$result->{$var_name->USER_NICKNAME_N_PERMISSION} = "$USER_NICKNAME ($USER_PERMISSION_NAME)";


		// VIEW - NAV
		$NAV_FILE_PATH = "";
		if(Checker::is_same_str($USER_PERMISSION, $const->USER_PERMISSION[0][$var_name->CODE])) {
			$NAV_FILE_PATH = "$file_root_path/template/nav.manager.template";
		} else if(Checker::is_same_str($USER_PERMISSION, $const->USER_PERMISSION[1][$var_name->CODE])) {
			$NAV_FILE_PATH = "$file_root_path/template/nav.user.template";
		}
		if(!empty($NAV_FILE_PATH)) {
			$result->{$var_name->NAV_FILE_PATH} = $NAV_FILE_PATH;
		}

		// TIME & DATE
		$START_DATE = $param->get_param_string($var_name->START_DATE, TimeManager::get_days_ago(5));
		if(!empty($START_DATE)) {
			$result->{$var_name->START_DATE} = $START_DATE;
		}		
		$END_DATE = $param->get_param_string($var_name->END_DATE, TimeManager::get_now_YYYYMMDD());
		if(!empty($END_DATE)) {
			$result->{$var_name->END_DATE} = $END_DATE;
		}
		$END_DATE_DAY_AFTER = TimeManager::get_days_after(1, $END_DATE); // $END_DATE_DAY_AFTER
		if(!empty($END_DATE_DAY_AFTER)) {
			$result->{$var_name->END_DATE_DAY_AFTER} = $END_DATE_DAY_AFTER;
		}

		// PAGINATION
		$PAGE_NUM = $param->get_param_number($var_name->PAGE_NUM, $const->PAGE_NUM_DEFAULT);
		if(Checker::is_natural_number($PAGE_NUM)) {
			$result->{$var_name->PAGE_NUM} = $PAGE_NUM;
		}
		$ROW_CNT = $param->get_param_number($var_name->ROW_CNT, $const->ROW_CNT_DEFAULT);
		if(Checker::is_natural_number($ROW_CNT)) {
			$result->{$var_name->ROW_CNT} = $ROW_CNT;
		}		
		$PAGE_RANGE = $param->get_param_number($var_name->PAGE_RANGE, $const->PAGE_RANGE_DEFAULT);
		if(Checker::is_natural_number($PAGE_RANGE)) {
			$result->{$var_name->PAGE_RANGE} = $PAGE_RANGE;
		}
		$IS_DESC = $param->get_param_boolean($var_name->IS_DESC, $const->IS_DESC_DEFAULT);
		if(is_bool($IS_DESC)) {
			$result->{$var_name->IS_DESC} = $IS_DESC;
		}

		// USER
		$TARGET_USER_ID = $param->get_param_number($var_name->TARGET_USER_ID, -1);
		if(Checker::is_natural_number($TARGET_USER_ID)) {
			$result->{$var_name->TARGET_USER_ID} = $TARGET_USER_ID;
		}
		$TARGET_AUTHOR_NAME = $param->get_param_string($var_name->TARGET_AUTHOR_NAME, "");
		if(!empty($TARGET_AUTHOR_NAME)) {
			$result->{$var_name->TARGET_AUTHOR_NAME} = $TARGET_AUTHOR_NAME;
		}

		// decode URI
		$SEARCH_KEYWORD = $param->get_param_string($var_name->SEARCH_KEYWORD, "");
		if(!empty($SEARCH_KEYWORD)) {
			$result->{$var_name->SEARCH_KEYWORD} = urldecode($SEARCH_KEYWORD);
		}






		// VIEW COMPONENT - INIT

		// COMBO BOX SET - INIT
		$result->{$var_name->COMBO_BOX_SET} = new stdClass();

		// COMBO BOX - ROW CNT
		$ROW_CNT_ARR = $param->get_row_cnt_array();
		$combobox_category = new ComboBoxManager();
		foreach ($ROW_CNT_ARR as $row_cnt) {
			$key = $row_cnt . " rows";
			$value = $row_cnt;
			$combobox_category->add_combobox_option($key, $value);
		}// wonder.jung

		$selected_key = $param->get_param_number($var_name->ROW_CNT, $const->ROW_CNT_DEFAULT);
		$width = "150px";
		if($IS_MOBILE) {
			$width = "40%";
		}
		$combobox_id = Checker::get_hash_key($var_name->COMBO_BOX_ROW_CNT);
		$combobox_std = $combobox_category->get_combobox($selected_key, $width, $combobox_id);

		$result->{$var_name->COMBO_BOX_SET}->{$var_name->COMBO_BOX_ROW_CNT} = $combobox_std;

		// COMBO BOX - USER LIST / "ALL USER"를 포함하지 않는 경우.
		$user_list = $mysql_interface->select_user_list($result);
		$combobox_user_list = new ComboBoxManager();
		foreach ($user_list as $user) {
			$key = $user->__nickname;
			$value = $user->__id;
			$combobox_user_list->add_combobox_option($key, $value);
		}

		$selected_key = $TARGET_USER_ID;
		$width = "150px";
		if($IS_MOBILE) {
			$width = "40%";
		}
		$combobox_id = Checker::get_hash_key($var_name->COMBO_BOX_USER_LIST);
		$combobox_std = $combobox_user_list->get_combobox($selected_key, $width, $combobox_id);

		$result->{$var_name->COMBO_BOX_SET}->{$var_name->COMBO_BOX_USER_LIST} = $combobox_std;


		// COMBO BOX - USER LIST / "ALL USER"를 포함하는 경우.
		$combobox_user_list = new ComboBoxManager();
		// ADD ALL USER
		$combobox_user_list->add_combobox_option($var_name->ALL_USERS);
		foreach ($user_list as $user) {
			$key = $user->__nickname;
			$value = $user->__id;
			$combobox_user_list->add_combobox_option($key, $value);
		}

		$selected_key = $TARGET_USER_ID;
		$width = "150px";
		if($IS_MOBILE) {
			$width = "40%";
		}
		$combobox_id = Checker::get_hash_key($var_name->COMBO_BOX_USER_LIST_ALL_USER);
		$combobox_std = $combobox_user_list->get_combobox($selected_key, $width, $combobox_id);

		$result->{$var_name->COMBO_BOX_SET}->{$var_name->COMBO_BOX_USER_LIST_ALL_USER} = $combobox_std;
		// COMBO BOX SET - DONE









		// BUTTONS - INIT
		$result->BUTTON_SET = new stdClass();
		// VIEW - CONTROL BOX DONE
		$btn_show = new ButtonManager("Show");
		$result->BUTTON_SET->BTN_SHOW = $btn_show->get_button();
		// VIEW - CONTROL BOX DONE
		$btn_export = new ButtonManager("Export", ButtonManager::$COLOR_GREEN);
		$result->BUTTON_SET->BTN_EXPORT = $btn_export->get_button();
		// BUTTONS - DONE


		// VIEW COMPONENT - DONE





		// DONE
		$this->props = $result;
	}
}

?>
