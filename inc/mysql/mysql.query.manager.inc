<?php

class MYSQLQueryManager{

	private $mysql_manager=null;
	private $mysql_interface=null;
	private $scope=null;
	private $query_str_arr=null;
	private $feedback_manager=null;
	private $result_std=null;

	private $micro_begin=null;
	private $micro_end=null;
	private $diff_micro=null;

	private $slow_query_sec=3;
	private $param_manager=null;

	private $has_check_slow_query=true;

	private $is_feedback_manager_on=true;

	public function MYSQLQueryManager($mysql_manager=null, $mysql_interface=null, $scope="", $feedback_manager=null){
		if(!is_null($feedback_manager)) {
			$this->feedback_manager = $feedback_manager;
		}
		if(!empty($scope)) {
			$this->scope = $scope;
		}
		if(!is_null($mysql_manager)) {
			$this->mysql_manager = $mysql_manager;
		}
		if(!is_null($mysql_interface)) {
			$this->mysql_interface = $mysql_interface;
		}

		$this->micro_begin = TimeManager::get_micro();

		$this->param_manager = new ParamManager();
	}

	public function set_check_slow_query($has_check_slow_query=true) {
		$this->has_check_slow_query = $has_check_slow_query;
	}

	public function on_feedback_manager() {
		$this->is_feedback_manager_on=true;
	}
	public function off_feedback_manager() {
		$this->is_feedback_manager_on=false;
	}

	private $query_param;
	public function add_query_param($key="", $value="") {
		if(empty($key)) {
			return;
		}

		if(is_null($this->query_param)) {
			$this->query_param = new stdClass();
		}
		$this->query_param->{$key} = $value;
	}
	public function get_query_param() {
		return $this->query_param;
	}

	public function addQueryLine($query_line){

		if(is_null($this->query_str_arr)) {
			$this->query_str_arr = array();
		}

		if(!empty($query_line)) {
			array_push($this->query_str_arr,$query_line);
		}
	}
	public function addQueryLineArr($query_line_arr){

		if(is_null($this->query_str_arr)) {
			$this->query_str_arr = array();
		}

		if(!empty($query_line_arr)) {
			$this->query_str_arr = array_merge($this->query_str_arr,$query_line_arr);
		}
	}
	// public --> private
	public function getExecuteQuery(){

		$execute_query = "";
		if(!empty($this->query_str_arr)) {
			$execute_query = implode(" ",$this->query_str_arr);
		}

		// 쿼리를 실행시키는 시점에 해당 쿼리를 쿼리 로거에 저장합니다.
		// 사용자가 사용했던 쿼리를 조회하기 위한 용도.

		return $execute_query;
	}
	public function getFormattedQuery(){

		$formatted_query = "";
		if(!empty($this->query_str_arr)) {
			$formatted_query = implode("<br/>",$this->query_str_arr);
		}

		return $formatted_query;
	}
	public function getFormattedQueryForJavascript(){

		$formatted_query = "";
		if(!empty($this->query_str_arr)) {
			$formatted_query = implode(" ",$this->query_str_arr);
		}

		return $formatted_query;
	}

	private function is_not_slow_query() {
		return !$this->is_slow_query();
	}
	private function is_slow_query() {

		if(is_null($this->diff_micro) || !(0 < $this->diff_micro)) {
			return false;
		}

		if($this->slow_query_sec < $this->diff_micro) {
			return true;
		}

		return false;

	}

	private function set_execution_micro_diff() {

		$this->micro_end = TimeManager::get_micro();
		$this->diff_micro = TimeManager::get_diff_micro($this->micro_begin, $this->micro_end);

	}

	private function insert_error_query() {
			
		$CLIENT_IP = Checker::get_client_ip();
		$CLIENT_OS = Checker::get_client_os();;
		$CLIENT_BROWSER = Checker::get_client_browser();

		$error_log = new stdClass();
		$error_log->query = $this->getFormattedQuery();
		$error_log->diff_micro = $this->diff_micro;

		// ACTION LOG
		$msg = json_encode($error_log);

		$param_set = new stdClass();
		$param_set->{$this->param_manager->CLIENT_IP} = $CLIENT_IP;
		$param_set->{$this->param_manager->CLIENT_OS} = $CLIENT_OS;
		$param_set->{$this->param_manager->CLIENT_BROWSER} = $CLIENT_BROWSER;
		$param_set->{$this->param_manager->ERROR_TYPE} = $this->param_manager->ERROR_TYPE_ERROR_QUERY;
		$param_set->{$this->param_manager->ERROR_MSG} = $msg;

		$this->mysql_interface->insert_error_log($param_set);
		
	}	

	private function insert_slow_query() {

		if($this->is_not_slow_query()) {
			return;
		}

		$CLIENT_IP = Checker::get_client_ip();
		$CLIENT_OS = Checker::get_client_os();;
		$CLIENT_BROWSER = Checker::get_client_browser();

		$error_log = new stdClass();
		$error_log->query = $this->getFormattedQuery();
		$error_log->diff_micro = $this->diff_micro;

		// ACTION LOG
		$msg = json_encode($error_log);

		$param_set = new stdClass();
		$param_set->{$this->param_manager->CLIENT_IP} = $CLIENT_IP;
		$param_set->{$this->param_manager->CLIENT_OS} = $CLIENT_OS;
		$param_set->{$this->param_manager->CLIENT_BROWSER} = $CLIENT_BROWSER;
		$param_set->{$this->param_manager->ERROR_TYPE} = $this->param_manager->ERROR_TYPE_SLOW_QUERY;
		$param_set->{$this->param_manager->ERROR_MSG} = $msg;

		$this->mysql_interface->insert_error_log($param_set);
	}

	public function get_execution_micro_diff() {	
		return $this->diff_micro;
	}

	// Execution Area
	public function select(){

		$result_std = new stdClass();
		$result_std->query = $this->getExecuteQuery();

		// Error handling
		try {

			$result_std = $this->mysql_manager->run($query, MYSQLManager::$RUN_TYPE_SELECT, $result_std);
			$this->result_std=$result_std;

		} catch (Exception $e) {
			// wonder.jung
		    if(!is_null($this->feedback_manager)) {

				$extra_data = new stdClass();
				$extra_data->{MYSQLFeedback::$FEEDBACK_MESSAGE} = $e->getMessage();
				$extra_data->{MYSQLFeedback::$FEEDBACK_QUERY} = $this->get_query();	
				$extra_data->{MYSQLFeedback::$FEEDBACK_REQ_PARAM} = $this->get_query_param();				
				$extra_data->from = "select / try-catch";

				$this->feedback_manager->add_error(
					// $scope=""
					$this->scope
					// $reason=""
					, MYSQLFeedback::$FEEDBACK_EVENT_QUERY_FAIL
					// $extra_data=null
					, $extra_data
				);

		    } // end if

		} // end catch

	}

	public function select_list(){

		$result_std = new stdClass();
		$result_std->query = $this->getExecuteQuery();

		// Error handling
		try {

			$result_std = $this->mysql_manager->run($query, MYSQLManager::$RUN_TYPE_SELECT, $result_std);
			if(strcmp($result_std->message,"query is empty") == 0) {
				$result_std->result = array();
			}
			$this->result_std=$result_std;

		} catch (Exception $e) {
			// wonder.jung
		    if(!is_null($this->feedback_manager)) {

				$extra_data = new stdClass();
				$extra_data->{MYSQLFeedback::$FEEDBACK_MESSAGE} = $e->getMessage();
				$extra_data->{MYSQLFeedback::$FEEDBACK_QUERY} = $this->get_query();	
				$extra_data->{MYSQLFeedback::$FEEDBACK_REQ_PARAM} = $this->get_query_param();				
				$extra_data->from = "select_list / try-catch";

				$this->feedback_manager->add_error(
					// $scope=""
					$this->scope
					// $reason=""
					, MYSQLFeedback::$FEEDBACK_EVENT_QUERY_FAIL
					// $extra_data=null
					, $extra_data
				);

		    } // end if

		} // end catch

	}

	public function select_one(){

		$result_std = new stdClass();
		$result_std->query = $this->getExecuteQuery();

		// Error handling
		try {

			$result_std = $this->mysql_manager->run($query, MYSQLManager::$RUN_TYPE_SELECT, $result_std);
			$result_list = $result_std->result;
			if(!empty($result_list)) {
				$result_one = $result_list[0];
				$result_std->result = $result_one;
			} else if(!is_null($result_list) && count($result_list) == 0) {
				// 조회 결과가 없을 경우의 처리.
				$result_std->result = null;
			}
			$this->result_std=$result_std;

		} catch (Exception $e) {
			// wonder.jung
		    if(!is_null($this->feedback_manager)) {

				$extra_data = new stdClass();
				$extra_data->{MYSQLFeedback::$FEEDBACK_MESSAGE} = $e->getMessage();
				$extra_data->{MYSQLFeedback::$FEEDBACK_QUERY} = $this->get_query();	
				$extra_data->{MYSQLFeedback::$FEEDBACK_REQ_PARAM} = $this->get_query_param();				
				$extra_data->from = "select_one / try-catch";

				$this->feedback_manager->add_error(
					// $scope=""
					$this->scope
					// $reason=""
					, MYSQLFeedback::$FEEDBACK_EVENT_QUERY_FAIL
					// $extra_data=null
					, $extra_data
				);

		    } // end if

		} // end catch

	}

	public function modify(){

		$result_std = new stdClass();
		$result_std->query = $this->getExecuteQuery();

		// Error handling
		try {

		    $result_std = $this->mysql_manager->run($query, MYSQLManager::$RUN_TYPE_MODIFY, $result_std);
		    $this->result_std=$result_std;

		} catch (Exception $e) {
			// wonder.jung
		    if(!is_null($this->feedback_manager)) {

				$extra_data = new stdClass();
				$extra_data->{MYSQLFeedback::$FEEDBACK_MESSAGE} = $e->getMessage();
				$extra_data->{MYSQLFeedback::$FEEDBACK_QUERY} = $this->get_query();	
				$extra_data->{MYSQLFeedback::$FEEDBACK_REQ_PARAM} = $this->get_query_param();
				$extra_data->from = "modify / try-catch";

				$this->feedback_manager->add_error(
					// $scope=""
					$this->scope
					// $reason=""
					, MYSQLFeedback::$FEEDBACK_EVENT_QUERY_FAIL
					// $extra_data=null
					, $extra_data
				);

		    } // end if

		} // end catch
		
	}
	public function is_failed() {

		$is_failed = !$this->is_success();
		if($is_failed) {

			if(!is_null($this->feedback_manager)) {

				$extra_data = new stdClass();
				$extra_data->{MYSQLFeedback::$FEEDBACK_MESSAGE} = $this->get_message();
				$extra_data->{MYSQLFeedback::$FEEDBACK_QUERY} = $this->get_query();	
				$extra_data->{MYSQLFeedback::$FEEDBACK_REQ_PARAM} = $this->get_query_param();
				$extra_data->from = "is_failed";

				if($this->is_feedback_manager_on) {
					$this->feedback_manager->add_error(
						// $scope=""
						$this->scope
						// $reason=""
						, MYSQLFeedback::$FEEDBACK_EVENT_QUERY_FAIL
						// $extra_data=null
						, $extra_data
					);
				}

				$this->insert_error_query();

			}

		}

		return $is_failed;
	}
	public function is_success() {

		if(	!is_null($this->result_std->success) && 
			$this->result_std->success == true) {

			if(!is_null($this->feedback_manager)) {
				$extra_data = new stdClass();
				$extra_data->{MYSQLFeedback::$FEEDBACK_MESSAGE} = $this->get_message();
				$extra_data->{MYSQLFeedback::$FEEDBACK_QUERY} = $this->get_query();
				$extra_data->{MYSQLFeedback::$FEEDBACK_QUERY_ROW} = $this->get_row_cnt();
				$extra_data->{MYSQLFeedback::$FEEDBACK_REQ_PARAM} = $this->get_query_param();

				$this->set_execution_micro_diff();

				if($this->is_feedback_manager_on) {
					if($this->is_slow_query()) {

						$this->feedback_manager->add_slow_query(
							// $scope=""
							$this->scope
							// $reason=""
							, MYSQLFeedback::$FEEDBACK_EVENT_QUERY_SLOW
							// $extra_data=null
							, $extra_data
						);

						$this->insert_slow_query();

					} else {

						$this->feedback_manager->add_ok(
							// $scope=""
							$this->scope
							// $reason=""
							, MYSQLFeedback::$FEEDBACK_EVENT_DONE
							// $extra_data=null
							, $extra_data
						);

					}
				}
			}

			return true;
		}
		return false;
	}
	public function get_feedback() {

		/*
		$feedback_std = new stdClass();
		$feedback_std->{MYSQLFeedback::$FEEDBACK_QUERY} = $this->get_query();
		$feedback_std->{MYSQLFeedback::$FEEDBACK_SCOPE} = $this->get_scope();
		*/
	}
	private function show_err() {
		$this->show_scope();
		$this->show_message();
		$this->show_query();
	}
	private function show_scope() {
		$scope = $this->scope;
		echo "<br/>[SCOPE]$scope";
	}
	private function get_scope() {
		return $this->scope;
	}
	/*
	private function show_query() {
		echo "<br/>[QUERY]";
		$this->printFormattedQuery();
	}
	*/
	private function get_query() {
		return $this->getFormattedQueryForJavascript();
	}
	private function show_message() {
		$message = $this->get_message();
		echo "<br/>[MESSAGE]$message";
	}
	private function get_message() {
		if(	!is_null($this->result_std->message) && 
			!is_null($this->result_std->message)) {

			return $this->result_std->message;
		}
		return "";
	}

	private function get_row_cnt() {
		if(empty($this->result_std->result)) {
			return 0;
		}

		return count($this->result_std->result);
	}

	public function get_result() {
		// echo "<br/>get_result<br/>";
		// print_r($this->result_std);
		// echo "<br/>get_result<br/>";

		return $this->result_std->result;
	}

	public function show_result() {
		$result = $this->get_result;
		$this->show_object_tree($result);
	}

	/*
		@ Private
		@ Desc : 오브젝트의 구조를 html 문서 위에 맞게 정렬해서 보여줍니다.(디버깅 용도)
	*/
	private function get_html_tabs($repeat_cnt=0) {
		$html_tabs = "&nbsp;&nbsp;";
		$tabs_returning = "";
		for($idx=0; $idx < $repeat_cnt; $idx++) {
			$tabs_returning .= $html_tabs;
		}

		return $tabs_returning;
	}
	public function show_object_tree($param_obj, $param_obj_name="", $depth=0) {

		if(is_null($param_obj)) {
			return;
		}

		echo "\$depth :: $depth<br/>";

		if($depth > 10) {
			echo "!Error! / \$depth is too deep :: $depth<br/>";
			return;
		}

		$tabs = $this->get_html_tabs($depth);
		$inner_tabs = $this->get_html_tabs($depth + 1);
		if(is_array($param_obj)) {

			// 배열 내부를 순회하며 내부 엘리먼트를 조회합니다.
			// 엘리먼트가 객체일 경우, 재귀 호출로 내용을 보여줍니다.
			if($depth == 0 && !empty($param_obj_name)) {
				echo "$tabs [ARRAY] $param_obj_name<br/>";
			} else {
				echo "$tabs [ARRAY]<br/>";
			}
			
			for($idx=0; $idx < count($param_obj); $idx++) {
				$element = $param_obj[$idx];

				if(is_null($element)) {
					continue;
				} else if(is_array($element) || is_object($element)) {
					$this->show_object_tree($element, null, $depth + 1);
				} else {
					echo "$inner_tabs $param_obj<br/>";
				}
			}

		} else if(is_object($param_obj)) {

			// 일반 객체인 경우
			if($depth == 0 && !empty($param_obj_name)) {
				echo "$tabs [OBJECT] $param_obj_name<br/>";
			} else {
				echo "$tabs [OBJECT]<br/>";
			}
			
			foreach($param_obj as $key => $value) {

				$tabs = $this->get_html_tabs($depth);
				if(is_null($value)) {
					continue;
				} else if(is_array($value) || is_object($value)) {
					$this->show_object_tree($value, null, $depth + 1);
				} else {
					echo "$inner_tabs [KEY]$key<br/>";
					echo "$inner_tabs [VALUE]$value<br/>";
				}
			    
			}		

		} else {

			// primitive 객체인 경우
			echo "$tabs [VALUE] $param_obj<br/>";

		}

		if($depth == 0) {
			echo "<br/>";
		}

	}	

	public function get_in_clause_number_id($rows=null){
		if(is_null($rows) || empty($rows)) {
			return "0";
		}

		$in_clause_number_id = "";
		for ($i=0; $i < count($rows); $i++) { 
			$row = $rows[$i];

			if(isset($row->__quiz_id) && 0 < intval($row->__quiz_id)) {

				$__quiz_id = intval($row->__quiz_id);

				if(empty($in_clause_number_id)) {
					$in_clause_number_id .= "".$__quiz_id;
				} else {
					$in_clause_number_id .= ",".$__quiz_id;
				} // end inner if

			} else if(isset($row->__id) && 0 < intval($row->__id)) {

				$__quiz_id = intval($row->__id);

				if(empty($in_clause_number_id)) {
					$in_clause_number_id .= "".$__quiz_id;
				} else {
					$in_clause_number_id .= ",".$__quiz_id;
				} // end inner if

			} // end if

		} // end for

		if(empty($in_clause_number_id)) {
			$in_clause_number_id = "0";
		}
		return $in_clause_number_id;
	}

	private function get_in_clause_number_id_arr($number_id_arr=null){

		if(is_null($number_id_arr) || empty($number_id_arr)) {
			return "0";
		}

		$in_clause_number_id = "";
		for ($i=0; $i < count($number_id_arr); $i++) { 
			$quiz_id = intval($number_id_arr[$i]);

			if(0 < $quiz_id) {

				if(empty($in_clause_number_id)) {
					$in_clause_number_id .= "".$quiz_id;
				} else {
					$in_clause_number_id .= ",".$quiz_id;
				}

			} // end if

		} // end for

		if(empty($in_clause_number_id)) {
			$in_clause_number_id = "0";
		}

		return $in_clause_number_id;

	}

	public function get_mysql_escape_str($target_str="") {

		if(is_null($target_str) || empty($target_str)) {
			return "";
		}

		return mysqli_real_escape_string($this->get_con(), $target_str);
	}
	

}

?>
