<?php

class ParamManager{

	// 1. request 및 response에서 전달하고 받는 파라미터들에 대한 관리
	// 1-1. 파라미터들을 stdClass에 key, value형태로 전달받습니다.
	// 1-2. 원하는 파라미터 이름을 배열 형태로 전달하면, 배열을 읽어 GET/POST에서 해당 파라미터들을 읽습니다.
	// 1-3. 파라미터를 읽어들이면서, 사용자가 지정한 검증 로직을 태웁니다.
	// 1-4. 파라미터 중, 

	private static $param_manager;
	// @ usage : ParamManager::get($feedback=null, $service_const_json=null, $service_var_name_json=null);
	public static function get($feedback=null, $service_const_json=null, $service_var_name_json=null){

		if(!is_null(self::$param_manager)) {
			// 피드백 매니저에게 기존에 만들어진 객체를 가져온 것을 기록.
			return self::$param_manager;
		}

		if(is_null($feedback)) {
			echo "ParamManager::get / \$feedback is not valid!<br/>";
			return;
		} else if(is_null($service_const_json)) {
			echo "ParamManager::get / \$service_const_json is not valid!<br/>";
			return null;
		} else if(is_null($service_var_name_json)) {
			echo "ParamManager::get / \$service_var_name_json is not valid!<br/>";
			return null;
		} else if(!isset($service_var_name_json->CHECKER) || is_null($service_var_name_json->CHECKER)) {
			echo "ParamManager::get / \$service_var_name_json->CHECKER is not valid!<br/>";
			return null;
		}

		$param_manager = 
		new ParamManager(
			$feedback
			, $service_const_json
			, $service_var_name_json
		);

		if(is_null($param_manager)) {
			echo "ParamManager::get / \$param_manager is not valid!<br/>";
			return null;
		}

		self::$param_manager = $param_manager;

		return self::$param_manager;
	}

	private $service_const_json;
	public function get_service_const(){
		return $this->service_const_json;
	}

	private $service_var_name_json;
	public function get_service_var_name(){
		return $this->service_var_name_json;
	}

	private function ParamManager($feedback=null, $service_const_json=null, $service_var_name_json=null){

		if(!is_null(self::$param_manager)) {
			// 피드백 매니저에게 기존에 만들어진 객체를 가져온 것을 기록.
			return self::$param_manager;
		}

		if(is_null($feedback)) {
			echo "ParamManager / \$feedback is not valid!<br/>";
			return;
		} else if(is_null($service_const_json)) {
			echo "ParamManager / \$service_const_json is not valid!<br/>";
			return null;
		} else if(is_null($service_var_name_json)) {
			echo "ParamManager / \$service_var_name_json is not valid!<br/>";
			return null;
		} else if(!isset($service_var_name_json->CHECKER) || is_null($service_var_name_json->CHECKER)) {
			echo "ParamManager / \$service_var_name_json->CHECKER is not valid!<br/>";
			return null;
		}

		$this->service_const_json = $service_const_json;
		$this->service_var_name_json = $service_var_name_json;

	}

	private function has_no_key($tossed_key){
		return !$this->has_key($tossed_key);
	}

	private function has_key($tossed_key) {

		// 모든 값은 param string으로 가져옵니다.
		// 가져온 뒤, 타입에 맞게 값을 변경합니다. 

		if(empty($this->service_var_name_json)) {
			return false;
		}

		foreach ($this->service_var_name_json as $key => $value) {
		    if(strcmp($key,$tossed_key) == 0) {
		    	return true;
		    }
		}
		return false;

	}

	public function get_param_string($tossed_key, $placeholder=null){

		if( $this->has_no_key($tossed_key) ) {
			return "";
		}

		$value = $this->get_param($tossed_key);
		// 값이 비어있고, 기본값이 있다면 이를 반환합니다.
		if(!is_null($placeholder) && is_null($value)) {
			$value = $placeholder;
		}

		return $value;
	}

	public function get_param_number($tossed_key, $placeholder=null){

		if( $this->has_no_key($tossed_key) ) {
			if(!is_null($placeholder)) {
				return $placeholder;
			}
			return -1;
		}

		$value = $this->get_param($tossed_key);

		// 문자열 "0"으로 전달한 경우를 먼저 처리.
		if(Checker::is_same_str($value, "0")) {
			return 0;
		}

		// 값이 비어있고, 기본값이 있다면 이를 반환합니다.
		if( (!is_null($placeholder) && is_null($value)) ||
			(!is_null($placeholder) && ((0 != $value) && empty($value))) ) {

			$value = $placeholder;
		}

		return intval($value);
	}

	public function get_param_boolean($tossed_key, $placeholder=null){

		if( $this->has_no_key($tossed_key) ) {
			if(!is_null($placeholder)) {
				return $placeholder;
			}
			return null;
		}
		$value = $this->get_param($tossed_key);

		if(is_bool($value)) {
			return $value;
		}

		// 값이 비어있고, 기본값이 있다면 이를 반환합니다.
		if( (!is_null($placeholder) && is_null($value)) ) {
			$value = $placeholder;
			return $value;
		}

		// 문자열 true, false를 감지해야 함.
	    switch ($value) {

	        // case $var == '1': 
	        // no need for this, because we used
			// $val == 1 not $var === 1

	        case (strcmp(strtolower($value),'true') == 0): 
	        case (strcmp(strtolower($value),'on') == 0):
	        case (strcmp(strtolower($value),'yes') == 0):
	        case (strcmp(strtolower($value),'y') == 0):
	            $value = true;
	            break;

	        case (strcmp(strtolower($value),'false') == 0): 
	        case (strcmp(strtolower($value),'off') == 0):
	        case (strcmp(strtolower($value),'no') == 0):
	        case (strcmp(strtolower($value),'n') == 0):
	            $value = false;
	            break;

	        case $value == 1:
	            $value = true;
	            break;

	        case $value == 0:
	            $value = false;
	            break;

	        default: $value = null;
	    }

		return $value;
	}	

	public function get_param($key){
		$value = $_GET[$key];
		if(is_null($value)) {
			$value = $_POST[$key];
		}

		return $value;
	}

	public function is_not_valid($param_std=null, $key_arr=null, $feedback_manager=null, $scope=null){
		return !$this->is_valid();
	}
	public function is_valid($param_std=null, $key_arr=null, $feedback_manager=null, $scope=null){

		return false;
	}

	/*
	$this->param_manager->is_not_valid(
		// $param_std=null
		$param
		// $key_arr=null
		, array(
			$this->param_manager->CLIENT_OS
			, $this->param_manager->CLIENT_IP
			, $this->param_manager->CLIENT_BROWSER
			, $this->param_manager->ERROR_TYPE
			, $this->param_manager->ERROR_MSG
		)
		// $feedback_manager=null
		, $this->feedback_manager
		// $scope=null
		, __FUNCTION__
	);
	*/



}

?>
