<?php

class MYSQLManager{

	public static $RUN_TYPE_SELECT = 1;
	public static $RUN_TYPE_MODIFY = 2;

	private $mysql_config=null;
	private $con=null;

	public function MYSQLManager($new_mysql_config){

		if($new_mysql_config == null) {
			return;	
		}

		$this->mysql_config=$new_mysql_config;
		$this->getConnection();
	}

	private function get_con() {
		return $this->con;
	}
	
	private function getConnection(){

		if($this->con != null) return;
		if($this->mysql_config == null) return;

		$this->con=mysqli_connect(	
			$this->mysql_config->host
			,$this->mysql_config->username
			,$this->mysql_config->password
			,$this->mysql_config->database
		);
					    
		// Check connection         
		if (mysqli_connect_errno()) 
		{                           
			echo "Failed to connect to MySQL: " . mysqli_connect_errno() . " / " . mysqli_connect_error();
			$this->con = null;
		} else {
			// set encoding / 한글 깨짐 현상 방지 / '?? ??? ???' --> '분당 토스트 마스터'
			mysqli_query($this->con,"set session character_set_connection=utf8;");
			mysqli_query($this->con,"set session character_set_results=utf8;");
			mysqli_query($this->con,"set session character_set_client=utf8;");
		}

	}
	public function closeConnection(){
		
		if($this->con == null) return;

		mysqli_close($this->con);
	}

	public function getLastId() {
		if($this->con == null) -1;
		
		return mysqli_insert_id($this->con);
	}

	public function run($query, $run_type, $result_std){

		if(empty($result_std)) return null;

		if(empty($query)){
			$result_std->code = MYSQLFeedback::$STATUS_CODE_SERVER_INTERNAL_ERROR;
			$result_std->message = "query is empty";
			$result_std->success = FALSE;
			$result_std->result = null;
			return $result_std;
		}

		if(	empty($run_type) || 
			($run_type != self::$RUN_TYPE_SELECT && $run_type != self::$RUN_TYPE_MODIFY)){
			
			$result_std->code = MYSQLFeedback::$STATUS_CODE_SERVER_INTERNAL_ERROR;
			$result_std->message = "run_type is empty";
			$result_std->success = FALSE;
			$result_std->result = null;
			return $result_std;
		}

		if($run_type == self::$RUN_TYPE_SELECT){
			$query_result_obj = $this->select($query);			
		} else if($run_type == self::$RUN_TYPE_MODIFY){
			$query_result_obj = $this->modify($query);			
		}

		if($query_result_obj->success == FALSE){
			$result_std->code = MYSQLFeedback::$STATUS_CODE_SERVER_INTERNAL_ERROR;
			$result_std->message = $query_result_obj->message;
			$result_std->success = FALSE;
			$result_std->result = null;
		} else {
			$result_std->code = MYSQLFeedback::$STATUS_CODE_OK;
			$result_std->message = MYSQLFeedback::$RESPONSE_MSG_JOB_COMPLETED;
			$result_std->success = TRUE;

			if($run_type == self::$RUN_TYPE_SELECT){
				$result_std->result = $query_result_obj->list;
			} else if($run_type == self::$RUN_TYPE_MODIFY){
				$result_std->result = $query_result_obj->dump_data; // true or false;
			}
		}
		return $result_std;
	}

	private function select($query){
		if($query == null || $query == "") return null;
		
		$column_arr = $this->getColumnNames($query);
		if($column_arr == null || count($column_arr) == 0) return null;
		
		// wonder.jung
		$query_result_obj = $this->execute($query);
		
		if($query_result_obj->success){
			$query_result_json_arr = array();
			while($row = mysqli_fetch_array($query_result_obj->dump_data)){
				$query_result_json_arr = $this->putQueryResult($query_result_json_arr, $column_arr, $row);
			}
			$query_result_obj->list = $query_result_json_arr;	
		}

		return $query_result_obj;       
	}

	private function putQueryResult($query_result_json_arr, $column_arr, $row){

		$query_result_json_obj = new stdClass();
		foreach ($column_arr as $column) {
			$field_value = "";
			if(isset($row) && isset($row[$column])){
				$field_value = $row[$column];
			}	
			$query_result_json_obj->{$column} = $field_value;
		}
		array_push($query_result_json_arr, $query_result_json_obj);		

		return $query_result_json_arr;
		
	}

	private function modify($query){	// insert, update, delete
		if(is_null($query) || empty($query)) return false;

		// wonder.jung
		$query_result_obj = $this->execute($query);
		
		return $query_result_obj;	
	}

	private function getColumnNames($query){
		if($query == null || $query == "") return null;
		
		$pattern = '/SELECT\s+(.+)\s+FROM/i';
		preg_match($pattern, $query, $matches);

		$columns = $matches[1];
		if($columns == null || $columns == "") return null;

		// user defined column name : __${column_name}
		$pattern = '/(__[a-zA-Z_\-]+)\s?\,?/';
		preg_match_all($pattern, $columns, $matches);
		$column_arr = $matches[1];
		
		if($column_arr != null || count($column_arr) > 0){
			return $column_arr;
		}

		// default column name : ${column_name}
		$pattern = '/([a-zA-Z_\-]+)\,?/';
		preg_match_all($pattern, $columns, $matches);
		$column_arr = $matches[1];

		if($column_arr == null || count($column_arr) == 0) return null;

		return $column_arr;
	}

	private function execute($query){
		if($query == null || $query == "") return;
		
		$query_result_obj = $this->getQueryResultObj();
		$query_result_obj->dump_data = mysqli_query($this->con,$query);

		if(!$query_result_obj->dump_data){
			$query_result_obj->message = mysqli_error($this->con);
		} else {
			$query_result_obj->success = TRUE;
			$query_result_obj->message = "done.";
		}
	
		return $query_result_obj;
	}

	public function escape_json_str($json_str) {
		return mysqli_real_escape_string($this->con, $json_str);
	}

	private function getQueryResultObj(){

		$query_result_obj = new stdClass();
		$query_result_obj->success = FALSE;
		$query_result_obj->dump_data = null;
		$query_result_obj->list = null;
		$query_result_obj->message = null;

		return $query_result_obj;
	}

}

?>
