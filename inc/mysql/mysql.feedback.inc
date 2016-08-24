<?php

class MYSQLFeedback{

	private $feedback_arr;
	private $param;

	public static $STATUS_CODE_OK=200;
	public static $STATUS_CODE_BAD_REQUEST=400;
	public static $STATUS_CODE_SERVER_INTERNAL_ERROR=500;

	public static $RESPONSE_MSG_JOB_COMPLETED="job completed";
	public static $RESPONSE_MSG_BAD_REQUEST="bad request";
	public static $RESPONSE_MSG_SERVER_INTERNAL_ERROR="server internal error";
	public static $RESPONSE_MSG_PARAM_NOT_VALID="parameter is not valid";	

	public function MYSQLFeedback($scope) {

		// feedback을 담을 배열을 초기화.
		$this->feedback_arr = array();

		if(empty($scope)) {
			echo "!Error! / MYSQLFeedback / empty(\$scope)<br/>";
			return;
		}

		// 만들어진 시간을 기록.
		$this->add_ok(
			// $scope_name=""
			$scope
			// $reason=""
			, MYSQLFeedback::$FEEDBACK_EVENT_INIT
		);

		// $this->param = ParamManager::get();
	}

	// error 로그를 DB에 기록하기 위해 mysql interface 객체를 받습니다.
	private $mysql_interface;
	public function set_mysql_interface($mysql_interface) {
		$this->mysql_interface = $mysql_interface;
	}
	private function add_error_to_mysql($error_std) {
		if(is_null($error_std)) {
			return;
		}
		if(is_null($this->mysql_interface)) {
			return;
		}
		if(!method_exists($this->mysql_interface,'insert_error_log')) {
			return;
		}

		$param = $this->param;

		$CLIENT_IP = Checker::get_client_ip();
		$CLIENT_OS = Checker::get_client_os();;
		$CLIENT_BROWSER = Checker::get_client_browser();

		$ERROR_TYPE = MYSQLFeedback::$FEEDBACK_EVENT_ERROR;
		$ERROR_MSG = json_encode($error_std);

		$QUERY_PARAM = new stdClass();
		$QUERY_PARAM->{$param->CLIENT_IP} = $CLIENT_IP;
		$QUERY_PARAM->{$param->CLIENT_OS} = $CLIENT_OS;
		$QUERY_PARAM->{$param->CLIENT_BROWSER} = $CLIENT_BROWSER;
		$QUERY_PARAM->{$param->ERROR_TYPE} = $ERROR_TYPE;
		$QUERY_PARAM->{$param->ERROR_MSG} = $ERROR_MSG;

		$this->mysql_interface->insert_error_log($QUERY_PARAM);
	}

	public static $FEEDBACK_EVENT_OK="OK";						// MYSQLFeedback::$FEEDBACK_EVENT_OK
	public static $FEEDBACK_EVENT_PIN="PIN";					// MYSQLFeedback::$FEEDBACK_EVENT_PIN
	public static $FEEDBACK_EVENT_ERROR="ERROR";				// MYSQLFeedback::$FEEDBACK_EVENT_ERROR
	public static $FEEDBACK_EVENT_SLOW_QUERY="SLOW_QUERY";		// MYSQLFeedback::$FEEDBACK_EVENT_SLOW_QUERY

	public static $FEEDBACK_TIME="TIME";						// MYSQLFeedback::$FEEDBACK_TIME
	public static $FEEDBACK_MICRO="MICRO";						// MYSQLFeedback::$FEEDBACK_MICRO
	public static $FEEDBACK_INTERVAL="INTERVAL";				// MYSQLFeedback::$FEEDBACK_INTERVAL

	public static $FEEDBACK_EVENT="EVENT";						// MYSQLFeedback::$FEEDBACK_EVENT

	public static $FEEDBACK_QUERY="QUERY";						// MYSQLFeedback::$FEEDBACK_QUERY
	public static $FEEDBACK_QUERY_ROW="QUERY_ROW";				// MYSQLFeedback::$FEEDBACK_QUERY_ROW
	public static $FEEDBACK_QUERY_PARAM="QUERY_PARAM";			// MYSQLFeedback::$FEEDBACK_QUERY_PARAM

	public static $FEEDBACK_MESSAGE="MESSAGE";					// MYSQLFeedback::$FEEDBACK_MESSAGE
	public static $FEEDBACK_SCOPE="SCOPE";						// MYSQLFeedback::$FEEDBACK_SCOPE
	public static $FEEDBACK_SUCCESS="SUCCESS";					// MYSQLFeedback::$FEEDBACK_SUCCESS
	public static $FEEDBACK_REASON="REASON";					// MYSQLFeedback::$FEEDBACK_REASON
	public static $FEEDBACK_EXTRA_DATA="EXTRA_DATA";			// MYSQLFeedback::$FEEDBACK_EXTRA_DATA

	public static $FEEDBACK_FLOW="FLOW";						// MYSQLFeedback::$FEEDBACK_FLOW
	public static $FEEDBACK_FLOW_IDX="FLOW_IDX";				// MYSQLFeedback::$FEEDBACK_FLOW_IDX
	public static $FEEDBACK_FLOW_SUCCESS="FLOW_SUCCESS";		// MYSQLFeedback::$FEEDBACK_FLOW_SUCCESS
	public static $FEEDBACK_FLOW_ERROR="FLOW_ERROR";			// MYSQLFeedback::$FEEDBACK_FLOW_ERROR

	public static $FEEDBACK_TIME_TOTAL="TIME_TOTAL";			// MYSQLFeedback::$FEEDBACK_TIME_TOTAL
	public static $FEEDBACK_TIME_AVG="TIME_AVG";				// MYSQLFeedback::$FEEDBACK_TIME_AVG

	public static $FEEDBACK_CUSTOM_KEY_VALUE="CUSTOM_KEY_VALUE";// MYSQLFeedback::$FEEDBACK_CUSTOM_KEY_VALUE

	// MYSQLFeedback::$FEEDBACK_EVENT_INIT
	public static $FEEDBACK_EVENT_INIT="MYSQLFeedback has been created.";
	// MYSQLFeedback::$FEEDBACK_EVENT_DONE
	public static $FEEDBACK_EVENT_DONE="Job done.";
	// MYSQLFeedback::$FEEDBACK_EVENT_QUERY_FAIL
	public static $FEEDBACK_EVENT_QUERY_FAIL="DB Query is failed.";
	// MYSQLFeedback::$FEEDBACK_EVENT_QUERY_SLOW
	public static $FEEDBACK_EVENT_QUERY_SLOW="DB Query is slow.";

	// MYSQLFeedback::$FEEDBACK_ERROR_SCOPE
	public static $FEEDBACK_ERROR_SCOPE="Scope is not valid.";
	public static $FEEDBACK_ERROR_REASON="Reason is not valid.";



	public function add_ok($scope_name="", $reason="", $extra_data=null) {

		if(empty($scope_name)) {
			echo "!Error! / MYSQLFeedback / add_ok / empty(\$scope_name)<br/>";
			exit();
		}

		if(empty($reason)) {
			echo "!Error! / MYSQLFeedback / add_ok / empty(\$reason)<br/>";
			exit();
		}

		$this->add(
			// $event_name=""
			MYSQLFeedback::$FEEDBACK_EVENT_OK
			// $scope_name=""
			, $scope_name
			// $success=false
			, true
			// $reason=""
			, $reason
			// $extra_data=null
			, $extra_data
		);
	}

	public function add_pin($scope_name="", $reason="", $extra_data=null) {

		if(empty($scope_name)) {
			echo "!Error! / MYSQLFeedback / add_pin / empty(\$scope_name)<br/>";
			exit();
		}

		if(empty($reason)) {
			echo "!Error! / MYSQLFeedback / add_pin / empty(\$reason)<br/>";
			exit();
		}

		$this->add(
			// $event_name=""
			MYSQLFeedback::$FEEDBACK_EVENT_PIN
			// $scope_name=""
			, $scope_name
			// $success=false
			, true
			// $reason=""
			, $reason
			// $extra_data=null
			, $extra_data
		);
	}	

	public function add_error($scope_name="", $reason="", $extra_data=null) {

		if(empty($scope_name)) {
			echo "!Error! / MYSQLFeedback / add_error / empty(\$scope_name)<br/>";
			exit();
		}

		if(empty($reason)) {
			echo "!Error! / MYSQLFeedback / add_error / empty(\$reason)<br/>";
			exit();
		}		

		$this->add(
			// $event_name=""
			MYSQLFeedback::$FEEDBACK_EVENT_ERROR
			// $scope_name=""
			, $scope_name
			// $success=false
			, false
			// $reason=""
			, $reason
			// $extra_data=null
			, $extra_data
		);

		// 지금 추가된 에러 로그 객체를 포함한 전체 피드백을 DB에 저장합니다.
		$this->add_error_to_mysql($this->get());
	}

	public function add_slow_query($scope_name="", $reason="", $extra_data=null) {

		if(empty($scope_name)) {
			$this->add_error(
				// $scope_name=""
				__FUNCTION__
				// $reason=""
				, MYSQLFeedback::$FEEDBACK_ERROR_SCOPE
				// $extra_data=null
			);
			return;
		}

		if(empty($reason)) {
			$this->add_error(
				// $scope_name=""
				__FUNCTION__
				// $reason=""
				, MYSQLFeedback::$FEEDBACK_ERROR_REASON
				// $extra_data=null
			);
			return;
		}

		$this->add(
			// $event_name=""
			MYSQLFeedback::$FEEDBACK_EVENT_SLOW_QUERY
			// $scope_name=""
			, $scope_name
			// $success=false
			, false
			// $reason=""
			, $reason
			// $extra_data=null
			, $extra_data
		);
	}

	private function add($event_name="", $scope_name="", $success=false, $reason="", $extra_data=null) {

		if(empty($event_name)) {
			return;
		}
		if(empty($scope_name)) {
			return;
		}
		if(empty($reason)) {
			return;
		}

		$feedback_std = new stdClass();

		$feedback_std->{MYSQLFeedback::$FEEDBACK_TIME} = TimeManager::get_now_YYYYMMDDHHMMSSU();
		$feedback_std->{MYSQLFeedback::$FEEDBACK_MICRO} = TimeManager::get_micro();
		
		$feedback_std->{MYSQLFeedback::$FEEDBACK_EVENT} = $event_name;
		$feedback_std->{MYSQLFeedback::$FEEDBACK_SCOPE} = $scope_name;
		$feedback_std->{MYSQLFeedback::$FEEDBACK_SUCCESS} = $success;
		$feedback_std->{MYSQLFeedback::$FEEDBACK_REASON} = $reason;
		$feedback_std->{MYSQLFeedback::$FEEDBACK_EXTRA_DATA} = $extra_data;

		$feedback_std->{MYSQLFeedback::$FEEDBACK_INTERVAL} = 0;

		$prev_feedback_std = null;
		$feedback_cnt = count($this->feedback_arr);
		if(0 < $feedback_cnt) {
			$prev_feedback_std = $this->feedback_arr[($feedback_cnt - 1)];
		}
		if(!is_null($prev_feedback_std)) {

			$diff_micro = 
			TimeManager::get_diff_micro(
				// $micro_begin
				$prev_feedback_std->{MYSQLFeedback::$FEEDBACK_MICRO}
				// $micro_end
				, $feedback_std->{MYSQLFeedback::$FEEDBACK_MICRO}
			);

			$feedback_std->{MYSQLFeedback::$FEEDBACK_INTERVAL} = $this->round_float($diff_micro, 5);
		}

		$feedback_std->{MYSQLFeedback::$FEEDBACK_FLOW_IDX} = $feedback_cnt;
		array_push($this->feedback_arr, $feedback_std);
		
	}

	private function round_float($target_float=-1, $shift_cnt=5) {

		if($target_float < 0) {
			return $target_float;
		}

		$offset_shift_amount = pow(10,$shift_cnt);
		$float_offsetted = round($target_float*$offset_shift_amount)/$offset_shift_amount;

		return $float_offsetted;
	}

	public function get() {

		$param = $this->param;

		$feedback_std = new stdClass();
		$feedback_std->{MYSQLFeedback::$FEEDBACK_FLOW} = $this->feedback_arr;

		$feedback_arr = $this->feedback_arr;
		$feedback_time_total = 0;

		$feedback_success_arr = array();
		$feedback_fail_arr = array();
		$success = true;
		$feedback_scope = "";
		for($idx=0; $idx < count($feedback_arr); $idx++) {
			$feedback_element = $feedback_arr[$idx];
			$feedback_time_total += $feedback_element->{MYSQLFeedback::$FEEDBACK_INTERVAL};

			if($feedback_element->{MYSQLFeedback::$FEEDBACK_SUCCESS}) {
				array_push($feedback_success_arr, $feedback_element);
			} else {
				// FEEDBACK_FLOW_IDX
				array_push($feedback_fail_arr, $feedback_element);
				$success = false;
			}

			$feedback_scope = $feedback_element->{MYSQLFeedback::$FEEDBACK_SCOPE};
		}
		$feedback_std->{MYSQLFeedback::$FEEDBACK_SUCCESS} = $success;
		$feedback_std->{MYSQLFeedback::$FEEDBACK_SCOPE} = $feedback_scope;
		$feedback_time_avg = $feedback_time_total/count($feedback_arr);

		$feedback_std->{MYSQLFeedback::$FEEDBACK_FLOW_SUCCESS} = $feedback_success_arr;
		$feedback_std->{MYSQLFeedback::$FEEDBACK_FLOW_ERROR} = $feedback_fail_arr;

		$feedback_std->{MYSQLFeedback::$FEEDBACK_TIME_TOTAL} = $this->round_float($feedback_time_total);
		$feedback_std->{MYSQLFeedback::$FEEDBACK_TIME_AVG} = $this->round_float($feedback_time_avg);

		$feedback_std->{MYSQLFeedback::$FEEDBACK_CUSTOM_KEY_VALUE} = $this->custom_key_value;

		// ADD CUSTOM KEY & VALUE
		if(!empty($this->custom_key_value)) {
			foreach ($this->custom_key_value as $key => $value) {
				$feedback_std->{$key} = $value;
			}
		}

		return $feedback_std;
	}

	// @ desc : 사용자가 돌려 받았을 때 바로 참조해야 할 값들을 추가할 때 사용합니다.
	private $custom_key_value;
	public function add_custom_key_value($key=null, $value=null) {

		if(is_null($key)) {
			return;
		}

		if(is_null($this->custom_key_value)) {
			$this->custom_key_value = new stdClass();
		}

		$this->custom_key_value->{$key} = $value;

	}

}

?>
