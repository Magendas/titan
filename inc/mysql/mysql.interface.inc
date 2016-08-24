<?php

class MYSQLInterface{

	private $mysql_manager=null;

	private $param_manager=null;
	private $const=null;
	private $var_name=null;

	private $feedback_manager=null;

	public function MYSQLInterface($new_mysql_manager=null, $param_manager=null, $feedback_manager=null){

		if(is_null($new_mysql_manager)) {
			echo "!Error! / MYSQLInterface / is_null(\$new_mysql_manager)<br/>";
			return;
		}
		if(is_null($param_manager)) {
			echo "!Error! / MYSQLInterface / is_null(\$param_manager)<br/>";
			return;
		}

		$this->mysql_manager = $new_mysql_manager;
		$this->param_manager = $param_manager;
		$this->const = $param_manager->get_service_const();
		$this->var_name = $param_manager->get_service_var_name();

		if(is_null($feedback_manager)) {
			$this->feedback_manager = new MYSQLFeedback(__FUNCTION__);
		} else {
			$this->feedback_manager = $feedback_manager;	
		}
		$this->feedback_manager->set_mysql_interface($this);
	}

	public function close(){
		$this->mysql_manager->closeConnection();
	}

	public function get_mysql_feedback() {
		return $this->feedback_manager;
	}

	public function get_feedback() {
		return $this->feedback_manager->get();
	}

	public function get_query_manager($scope=null) {

		if(is_null($scope)) {
			echo "!Error! / get_query_manager / is_null(\$scope)<br/>";
		}

		$query_manager = 
		new MYSQLQueryManager(
			// $mysql_manager=null
			$this->mysql_manager
			// $mysql_interface=null
			, $this
			// $scope=""
			, $scope
			// $feedback_manager=null
			, $this->feedback_manager
		);

		return $query_manager;

	}
	

























	//    .aMMMb  dMP dMP dMMMMMP dMMMMb  dMP dMP 
	//   dMP"dMP dMP dMP dMP     dMP.dMP dMP.dMP  
	//  dMP.dMP dMP dMP dMMMP   dMMMMK"  VMMMMP   
	// dMP.MMP dMP.aMP dMP     dMP"AMF dA .dMP    
	// VMMP"MP VMMMP" dMMMMMP dMP dMP  VMMMP"  



  	public function select_user_by_fb_id($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->FACEBOOK_USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$FACEBOOK_USER_ID = $param->{$this->var_name->FACEBOOK_USER_ID};
		// CHECK PARAM VALIDATION - DONE

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `USER`.`id` AS __id,"
				,"CONCAT(`USER`.first_name,\" \",`USER`.last_name) AS __user_name,"
				,"`USER`.`first_name` AS __first_name,"
				,"`USER`.`last_name` AS __last_name,"
				,"IFNULL(`USER`.`nickname`,\"\") AS __nickname,"
				,"`USER`.`status` AS __status,"
				,"`USER`.`permission` AS __permission,"
				,"IFNULL(`USER`.`fb_id`, \"\") AS __fb_id,"
				,"IFNULL(`USER`.`google_id`, \"\") AS __google_id,"
				,"`USER`.`mobile` AS __mobile,"
				,"`USER`.`email` AS __email,"
				,"`USER`.`password` AS __password,"
				,"`USER`.`quota` AS __quota,"
				,"`USER`.`category_access` AS __category_access,"
				,"`USER`.`date_created` AS __date_created,"
				,"`USER`.`date_updated` AS __date_updated"
				,"FROM `USER`"
				,"WHERE `USER`.`fb_id`=\"$FACEBOOK_USER_ID\""
			)
		);
		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();		

		return $result;
	}

  	public function select_user_by_google_id($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->GOOGLE_USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$GOOGLE_USER_ID = $param->{$this->var_name->GOOGLE_USER_ID};
		// CHECK PARAM VALIDATION - DONE


		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `USER`.`id` AS __id,"
				,"CONCAT(`USER`.first_name,\" \",`USER`.last_name) AS __user_name,"
				,"`USER`.`first_name` AS __first_name,"
				,"`USER`.`last_name` AS __last_name,"
				,"IFNULL(`USER`.`nickname`,\"\") AS __nickname,"
				,"`USER`.`status` AS __status,"
				,"`USER`.`permission` AS __permission,"
				,"IFNULL(`USER`.`fb_id`, \"\") AS __fb_id,"
				,"IFNULL(`USER`.`google_id`, \"\") AS __google_id,"
				,"`USER`.`mobile` AS __mobile,"
				,"`USER`.`email` AS __email,"
				,"`USER`.`password` AS __password,"
				,"`USER`.`quota` AS __quota,"
				,"`USER`.`category_access` AS __category_access,"
				,"`USER`.`date_created` AS __date_created,"
				,"`USER`.`date_updated` AS __date_updated"
				,"FROM `USER`"
				,"WHERE `USER`.`google_id`=\"$GOOGLE_USER_ID\""
			)
		);
		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();		

		return $result;
	}	

  	public function select_user_by_kakao_id($param=null) {

  		// need to implement
  		return null;

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->GOOGLE_USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$GOOGLE_USER_ID = $param->{$this->var_name->GOOGLE_USER_ID};
		// CHECK PARAM VALIDATION - DONE


		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `USER`.`id` AS __id,"
				,"CONCAT(`USER`.first_name,\" \",`USER`.last_name) AS __user_name,"
				,"`USER`.`first_name` AS __first_name,"
				,"`USER`.`last_name` AS __last_name,"
				,"IFNULL(`USER`.`nickname`,\"\") AS __nickname,"
				,"`USER`.`status` AS __status,"
				,"`USER`.`permission` AS __permission,"
				,"IFNULL(`USER`.`fb_id`, \"\") AS __fb_id,"
				,"IFNULL(`USER`.`google_id`, \"\") AS __google_id,"
				,"`USER`.`mobile` AS __mobile,"
				,"`USER`.`email` AS __email,"
				,"`USER`.`password` AS __password,"
				,"`USER`.`quota` AS __quota,"
				,"`USER`.`category_access` AS __category_access,"
				,"`USER`.`date_created` AS __date_created,"
				,"`USER`.`date_updated` AS __date_updated"
				,"FROM `USER`"
				,"WHERE `USER`.`google_id`=\"$GOOGLE_USER_ID\""
			)
		);
		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();		

		return $result;
	}		
	
  	public function select_user_by_email($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->USER_EMAIL
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}
		$USER_EMAIL = $param->{$this->var_name->USER_EMAIL};
		// CHECK PARAM VALIDATION - DONE  		

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `USER`.`id` AS __id,"
				,"CONCAT(`USER`.first_name,\" \",`USER`.last_name) AS __user_name,"
				,"`USER`.`first_name` AS __first_name,"
				,"`USER`.`last_name` AS __last_name,"
				,"IFNULL(`USER`.`nickname`,\"\") AS __nickname,"
				,"`USER`.`status` AS __status,"
				,"`USER`.`permission` AS __permission,"
				,"IFNULL(`USER`.`fb_id`, \"\") AS __fb_id,"
				,"IFNULL(`USER`.`google_id`, \"\") AS __google_id,"
				,"`USER`.`mobile` AS __mobile,"
				,"`USER`.`email` AS __email,"
				,"`USER`.`quota` AS __quota"
				,"FROM `USER`"
				,"WHERE `USER`.`email`=\"$USER_EMAIL\""
			)
		);
		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();		

		return $result;
	}	

	public function select_user_list($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->USER_STATUS
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}
		$USER_STATUS = $param->{$this->var_name->USER_STATUS};
		// CHECK PARAM VALIDATION - DONE  		


		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `USER`.`id` AS __id,"
				,"CONCAT(`USER`.first_name,\" \",`USER`.last_name) AS __user_name,"
				,"`USER`.`first_name` AS __first_name,"
				,"`USER`.`last_name` AS __last_name,"
				,"IFNULL(`USER`.`nickname`,\"\") AS __nickname,"
				,"`USER`.`status` AS __status,"
				,"`USER`.`permission` AS __permission,"
				,"IFNULL(`USER`.`fb_id`, \"\") AS __fb_id,"
				,"IFNULL(`USER`.`google_id`, \"\") AS __google_id,"
				,"`USER`.`mobile` AS __mobile,"
				,"`USER`.`email` AS __email,"
				,"`USER`.`password` AS __password,"
				,"`USER`.`quota` AS __quota,"
				,"`USER`.`category_access` AS __category_access,"
				,"`USER`.`date_created` AS __date_created,"
				,"`USER`.`date_updated` AS __date_updated"
				,"FROM `USER`"
				,"WHERE `USER`.`status`=\"$USER_STATUS\""
			)
		);
		$query_manager->select();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();

		return $result;
	}

	public function select_all_user_list($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->USER_STATUS
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}
		$USER_STATUS = $param->{$this->var_name->USER_STATUS};
		// CHECK PARAM VALIDATION - DONE  		


		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `USER`.`id` AS __id,"
				,"CONCAT(`USER`.first_name,\" \",`USER`.last_name) AS __user_name,"
				,"`USER`.`first_name` AS __first_name,"
				,"`USER`.`last_name` AS __last_name,"
				,"IFNULL(`USER`.`nickname`,\"\") AS __nickname,"
				,"`USER`.`status` AS __status,"
				,"`USER`.`permission` AS __permission,"
				,"IFNULL(`USER`.`fb_id`, \"\") AS __fb_id,"
				,"IFNULL(`USER`.`google_id`, \"\") AS __google_id,"
				,"`USER`.`mobile` AS __mobile,"
				,"`USER`.`email` AS __email,"
				,"`USER`.`password` AS __password,"
				,"`USER`.`quota` AS __quota,"
				,"`USER`.`category_access` AS __category_access,"
				,"`USER`.`date_created` AS __date_created,"
				,"`USER`.`date_updated` AS __date_updated"
				,"FROM `USER`"
			)
		);
		$query_manager->select();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();

		return $result;
	}	

  	public function select_user_by_id($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$USER_ID = $param->{$this->var_name->USER_ID};
		// CHECK PARAM VALIDATION - DONE  		

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `USER`.`id` AS __id,"
				,"CONCAT(`USER`.first_name,\" \",`USER`.last_name) AS __user_name,"
				,"`USER`.`first_name` AS __first_name,"
				,"`USER`.`last_name` AS __last_name,"
				,"IFNULL(`USER`.`nickname`,\"\") AS __nickname,"
				,"`USER`.`status` AS __status,"
				,"`USER`.`permission` AS __permission,"
				,"IFNULL(`USER`.`fb_id`, \"\") AS __fb_id,"
				,"IFNULL(`USER`.`google_id`, \"\") AS __google_id,"
				,"`USER`.`mobile` AS __mobile,"
				,"`USER`.`email` AS __email,"
				,"`USER`.`password` AS __password,"
				,"`USER`.`quota` AS __quota,"
				,"`USER`.`category_access` AS __category_access,"
				,"`USER`.`date_created` AS __date_created,"
				,"`USER`.`date_updated` AS __date_updated"
				,"FROM `USER`"
				,"WHERE `USER`.`id`=$USER_ID"
			)
		);
		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();

		return $result;
	}	

  	public function select_user_simple_by_fb_id($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->FACEBOOK_USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$FACEBOOK_USER_ID = $param->{$this->var_name->FACEBOOK_USER_ID};
		// CHECK PARAM VALIDATION - DONE		

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `USER`.`id` AS __id,"
				,"CONCAT(`USER`.first_name,\" \",`USER`.last_name) AS __user_name,"
				,"`USER`.`first_name` AS __first_name,"
				,"`USER`.`last_name` AS __last_name,"
				,"IFNULL(`USER`.`nickname`,\"\") AS __nickname,"
				,"`USER`.`status` AS __status,"
				,"`USER`.`permission` AS __permission,"
				,"IFNULL(`USER`.`fb_id`, \"\") AS __fb_id,"
				,"IFNULL(`USER`.`google_id`, \"\") AS __google_id,"
				,"`USER`.`quota` AS __quota,"
				,"`USER`.`category_access` AS __category_access"
				,"FROM `USER`"
				,"WHERE `USER`.`fb_id`=\"$FACEBOOK_USER_ID\""
			)			
		);
		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();

		return $result;
	} 

	public function select_user_simple_by_google_id($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->GOOGLE_USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$GOOGLE_USER_ID = $param->{$this->var_name->GOOGLE_USER_ID};
		// CHECK PARAM VALIDATION - DONE		

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `USER`.`id` AS __id,"
				,"CONCAT(`USER`.first_name,\" \",`USER`.last_name) AS __user_name,"
				,"`USER`.`first_name` AS __first_name,"
				,"`USER`.`last_name` AS __last_name,"
				,"IFNULL(`USER`.`nickname`,\"\") AS __nickname,"
				,"`USER`.`status` AS __status,"
				,"`USER`.`permission` AS __permission,"
				,"IFNULL(`USER`.`fb_id`, \"\") AS __fb_id,"
				,"IFNULL(`USER`.`google_id`, \"\") AS __google_id,"
				,"`USER`.`quota` AS __quota,"
				,"`USER`.`category_access` AS __category_access"
				,"FROM `USER`"
				,"WHERE `USER`.`google_id`=\"$GOOGLE_USER_ID\""
			)			
		);
		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();

		return $result;
	} 

	public function insert_user_from_fb_user($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->FACEBOOK_USER_FIRST_NAME
				, $this->var_name->FACEBOOK_USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$FACEBOOK_USER_FIRST_NAME = $param->{$this->var_name->FACEBOOK_USER_FIRST_NAME};
		$FACEBOOK_USER_LAST_NAME = $param->{$this->var_name->FACEBOOK_USER_LAST_NAME};
		$FACEBOOK_USER_EMAIL = $param->{$this->var_name->FACEBOOK_USER_EMAIL}; // CAN BE EMPTY
		$FACEBOOK_USER_ID = $param->{$this->var_name->FACEBOOK_USER_ID};
		$CATEGORY_ACCESS_ARR = $param->{$this->var_name->CATEGORY_ACCESS_ARR};
		// CHECK PARAM VALIDATION - DONE

		$CATEGORY_ACCESS_ARR_STR = "";
		if(!empty($CATEGORY_ACCESS_ARR)) {
			$CATEGORY_ACCESS_ARR_STR = implode(",", $CATEGORY_ACCESS_ARR);
		}

		$USER_STATUS_NOT_IN_ACTION = $this->param_manager->USER_STATUS_NOT_IN_ACTION;
		$USER_STATUS_AVAILABLE = $this->param_manager->USER_STATUS_AVAILABLE;
		$USER_QUOTA_DEFAULT_CNT = $this->param_manager->USER_QUOTA_DEFAULT_CNT;
		$USER_PERMISSION_TESTER = $this->param_manager->USER_PERMISSION_TESTER;

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"INSERT INTO `USER`("
				,"`first_name`"
				,",`last_name`"
				,",`nickname`"
				,",`status`"
				,",`fb_id`"
				,",`email`"
				,",`password`"
				,",`date_created`"
				,",`date_updated`"
				,",`quota`"
				,",`category_access`"
				,",`permission`)"
				,"VALUES("
				,"\"$FACEBOOK_USER_FIRST_NAME\""
				,",\"$FACEBOOK_USER_LAST_NAME\""
				,",\"$FACEBOOK_USER_FIRST_NAME\""
				,",\"$USER_STATUS_AVAILABLE\""
				,",\"$FACEBOOK_USER_ID\""
				,",\"$FACEBOOK_USER_EMAIL\""
				,",\"0000\""
				,",CURRENT_TIMESTAMP"
				,",CURRENT_TIMESTAMP"
				,",$USER_QUOTA_DEFAULT_CNT"
				,",\"$CATEGORY_ACCESS_ARR_STR\""
				,",\"$USER_PERMISSION_TESTER\""
				,")"
			)
		);

		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}

	public function insert_user_from_google_user($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->GOOGLE_USER_FIRST_NAME
				, $this->var_name->GOOGLE_USER_EMAIL
				, $this->var_name->GOOGLE_USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$GOOGLE_USER_FIRST_NAME = $param->{$this->var_name->GOOGLE_USER_FIRST_NAME};
		$GOOGLE_USER_LAST_NAME = $param->{$this->var_name->GOOGLE_USER_LAST_NAME};
		$GOOGLE_USER_EMAIL = $param->{$this->var_name->GOOGLE_USER_EMAIL};
		$GOOGLE_USER_ID = $param->{$this->var_name->GOOGLE_USER_ID};
		$CATEGORY_ACCESS_ARR = $param->{$this->var_name->CATEGORY_ACCESS_ARR};
		// CHECK PARAM VALIDATION - DONE

		$CATEGORY_ACCESS_ARR_STR = "";
		if(!empty($CATEGORY_ACCESS_ARR)) {
			$CATEGORY_ACCESS_ARR_STR = implode(",", $CATEGORY_ACCESS_ARR);
		}

		$USER_STATUS_NOT_IN_ACTION = $this->param_manager->USER_STATUS_NOT_IN_ACTION;
		$USER_STATUS_AVAILABLE = $this->param_manager->USER_STATUS_AVAILABLE;
		$USER_QUOTA_DEFAULT_CNT = $this->param_manager->USER_QUOTA_DEFAULT_CNT;
		$USER_PERMISSION_TESTER = $this->param_manager->USER_PERMISSION_TESTER;

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"INSERT INTO `USER`("
				,"`first_name`"
				,",`last_name`"
				,",`nickname`"
				,",`status`"
				,",`google_id`"
				,",`email`"
				,",`password`"
				,",`date_created`"
				,",`date_updated`"
				,",`quota`"
				,",`category_access`"
				,",`permission`)"
				,"VALUES("
				,"\"$GOOGLE_USER_FIRST_NAME\""
				,",\"$GOOGLE_USER_LAST_NAME\""
				,",\"$GOOGLE_USER_FIRST_NAME\""
				,",\"$USER_STATUS_AVAILABLE\""
				,",\"$GOOGLE_USER_ID\""
				,",\"$GOOGLE_USER_EMAIL\""
				,",\"0000\""
				,",CURRENT_TIMESTAMP"
				,",CURRENT_TIMESTAMP"
				,",$USER_QUOTA_DEFAULT_CNT"
				,",\"$CATEGORY_ACCESS_ARR_STR\""
				,",\"$USER_PERMISSION_TESTER\""
				,")"
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}		

	}

	public function update_user_status($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->USER_ID
				, $this->var_name->USER_STATUS
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$USER_ID = $param->{$this->var_name->USER_ID};
		$USER_STATUS = $param->{$this->var_name->USER_STATUS};
		// CHECK PARAM VALIDATION - DONE	

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"UPDATE `USER` SET `status` = \"$USER_STATUS\" WHERE id=$USER_ID"
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}

	public function update_user_permission($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->USER_ID
				, $this->var_name->USER_PERMISSION
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$USER_ID = $param->{$this->var_name->USER_ID};
		$USER_PERMISSION = $param->{$this->var_name->USER_PERMISSION};
		// CHECK PARAM VALIDATION - DONE	

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"UPDATE `USER` SET `permission` = \"$USER_PERMISSION\" WHERE id=$USER_ID"
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}

	public function update_user_detail_by_facebook($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->FACEBOOK_USER_ID
				, $this->var_name->FACEBOOK_USER_PROFILE_PICTURE
				, $this->var_name->FACEBOOK_USER_GENDER
				, $this->var_name->FACEBOOK_USER_LOCALE
				, $this->var_name->FACEBOOK_USER_AGE_RANGE
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$FACEBOOK_USER_ID = $param->{$this->var_name->FACEBOOK_USER_ID};
		$FACEBOOK_USER_PROFILE_PICTURE = $param->{$this->var_name->FACEBOOK_USER_PROFILE_PICTURE};
		$FACEBOOK_USER_GENDER = $param->{$this->var_name->FACEBOOK_USER_GENDER};
		$FACEBOOK_USER_LOCALE = $param->{$this->var_name->FACEBOOK_USER_LOCALE};
		$FACEBOOK_USER_AGE_RANGE = $param->{$this->var_name->FACEBOOK_USER_AGE_RANGE};
		// CHECK PARAM VALIDATION - DONE

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"UPDATE `USER`"
				," SET `gender`=\"$FACEBOOK_USER_GENDER\""
				,",`locale`=\"$FACEBOOK_USER_LOCALE\""
				,",`age_range`=$FACEBOOK_USER_AGE_RANGE"
				,",`thumbnail`=\"$FACEBOOK_USER_PROFILE_PICTURE\""
				,",`date_updated` = CURRENT_TIMESTAMP"
				," WHERE fb_id=\"$FACEBOOK_USER_ID\""
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}	

	public function update_user_nickname($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->USER_ID
				, $this->var_name->USER_NICKNAME
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$USER_ID = $param->{$this->var_name->USER_ID};
		$USER_NICKNAME = $param->{$this->var_name->USER_NICKNAME};
		// CHECK PARAM VALIDATION - DONE  		


		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"UPDATE `USER` SET `nickname` = \"$USER_NICKNAME\" WHERE id=$USER_ID"
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}	

	public function update_user_fb_id($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->FACEBOOK_USER_ID
				, $this->var_name->USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$FACEBOOK_USER_ID = $param->{$this->var_name->FACEBOOK_USER_ID};
		$USER_ID = $param->{$this->var_name->USER_ID};
		// CHECK PARAM VALIDATION - DONE		

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"UPDATE `USER` SET `date_updated` = CURRENT_TIMESTAMP, fb_id=\"$FACEBOOK_USER_ID\" WHERE id=$USER_ID"
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}

	public function update_user_email($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->USER_ID
				, $this->var_name->USER_EMAIL
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$USER_ID = $param->{$this->var_name->USER_ID};
		$USER_EMAIL = $param->{$this->var_name->USER_EMAIL};
		// CHECK PARAM VALIDATION - DONE		

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"UPDATE `USER` SET `date_updated` = CURRENT_TIMESTAMP, email=\"$USER_EMAIL\" WHERE id=$USER_ID"
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}	

	public function update_user_google_id($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->GOOGLE_USER_ID
				, $this->var_name->USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$GOOGLE_USER_ID = $param->{$this->var_name->GOOGLE_USER_ID};
		$USER_ID = $param->{$this->var_name->USER_ID};
		// CHECK PARAM VALIDATION - DONE		

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"UPDATE `USER` SET `date_updated` = CURRENT_TIMESTAMP, google_id=\"$GOOGLE_USER_ID\" WHERE id=$USER_ID"
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}

	public function insert_access_log($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->CLIENT_OS
				, $this->var_name->CLIENT_IP
				, $this->var_name->CLIENT_BROWSER
				, $this->var_name->ACCESS_MSG
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		
		$CLIENT_OS = $param->{$this->var_name->CLIENT_OS};
		$CLIENT_IP = $param->{$this->var_name->CLIENT_IP};
		$CLIENT_BROWSER = $param->{$this->var_name->CLIENT_BROWSER};
		$ACCESS_MSG = $param->{$this->var_name->ACCESS_MSG};
		// CHECK PARAM VALIDATION - DONE

		$msg_url_encode = urlencode($ACCESS_MSG);
		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->off_feedback_manager();
		$query_manager->addQueryLineArr(
			array(
				"INSERT INTO `ACCESS_LOG`(`time_start`,`os`,`browser`,`ip`,`msg`)"
				,"VALUES(CURRENT_TIMESTAMP,\"$CLIENT_OS\",\"$CLIENT_BROWSER\",\"$CLIENT_IP\",\"$msg_url_encode\")"
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}

	public function select_access_log_total_cnt($param=null) {

		$TARGET_USER_ID = -1;
		if(isset($param->{$this->var_name->TARGET_USER_ID})) {
			$TARGET_USER_ID = intval($param->{$this->var_name->TARGET_USER_ID});
		}
		
		$platform_id = "";
		if(Checker::is_natural_number($TARGET_USER_ID)){

			$param->{$this->var_name->USER_ID} = $TARGET_USER_ID;
			$user_info_obj = $this->select_user_by_id($param);
			if(!is_null($user_info_obj)) {
				
				if(!empty($user_info_obj->__fb_id)) {
					$platform_id = $user_info_obj->__fb_id;
				} else if(!empty($user_info_obj->__google_id)) {
					$platform_id = $user_info_obj->__google_id;
				}   // end inner if

			}  // end inner if

		} // end outer if

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT COUNT(`ACCESS_LOG`.`id`) AS __cnt"
				,"FROM `ACCESS_LOG`"
			)
		);

		if(!empty($platform_id)) {
			$query_manager->addQueryLineArr(
				array(
					"WHERE msg LIKE \"%$platform_id%\""
				)
			);
		}

		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}		

		$result = $query_manager->get_result();		

		return intval($result->__cnt);
	}
	
  	public function select_access_log($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->PAGE_NUM
				, $this->var_name->ROW_CNT
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}
		$PAGE_NUM = intval($param->{$this->var_name->PAGE_NUM});
		$ROW_CNT = intval($param->{$this->var_name->ROW_CNT});
		$TARGET_USER_ID = -1;
		if(isset($param->{$this->var_name->TARGET_USER_ID})) {
			$TARGET_USER_ID = intval($param->{$this->var_name->TARGET_USER_ID});	
		}
		// CHECK PARAM VALIDATION - DONE
		
		$platform_id = "";
		if(Checker::is_natural_number($TARGET_USER_ID)){

			$param->{$this->var_name->USER_ID} = $TARGET_USER_ID;
			$user_info_obj = $this->select_user_by_id($param);
			if(!is_null($user_info_obj)) {
				
				if(!empty($user_info_obj->__fb_id)) {
					$platform_id = $user_info_obj->__fb_id;
				} else if(!empty($user_info_obj->__google_id)) {
					$platform_id = $user_info_obj->__google_id;
				}   // end inner if

			}  // end inner if

		} // end outer if



		// page begins from 1
		$row_num = $ROW_CNT;
		$offset = ($PAGE_NUM - 1) * $row_num;  		

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `ACCESS_LOG`.`id` AS __id,"
				,"`ACCESS_LOG`.`time_start` AS __time_start,"
				,"`ACCESS_LOG`.`os` AS __os,"
				,"`ACCESS_LOG`.`browser` AS __browser,"
				,"`ACCESS_LOG`.`ip` AS __ip,"
				,"`ACCESS_LOG`.`msg` AS __msg"
				,"FROM `ACCESS_LOG`"
			)
		);

		if(!empty($platform_id)) {
			$query_manager->addQueryLineArr(
				array(
					"WHERE msg LIKE \"%$platform_id%\""
				)
			);
		}

		$query_manager->addQueryLineArr(
			array(
				"ORDER BY `ACCESS_LOG`.`id` DESC"
				,"LIMIT $offset, $row_num"
			)
		);

		$query_manager->select();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();		

		for($idx=0; $idx < count($result); $idx++) {
			$log = $result[$idx];
			$log->__msg = urldecode($log->__msg);
		}

		return $result;
	}
	
	public function insert_action_log($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->CLIENT_OS
				, $this->var_name->CLIENT_IP
				, $this->var_name->CLIENT_BROWSER
				, $this->var_name->ACTION_TYPE
				, $this->var_name->ACTION_MSG
				, $this->var_name->USER_ID
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		
		$CLIENT_OS = $param->{$this->var_name->CLIENT_OS};
		$CLIENT_IP = $param->{$this->var_name->CLIENT_IP};
		$CLIENT_BROWSER = $param->{$this->var_name->CLIENT_BROWSER};
		$ACTION_TYPE = $param->{$this->var_name->ACTION_TYPE};
		$ACTION_MSG = $param->{$this->var_name->ACTION_MSG};
		$USER_ID = intval($param->{$this->var_name->USER_ID});
		// CHECK PARAM VALIDATION - DONE

		$msg_url_encode = urlencode($ACTION_MSG);

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->off_feedback_manager();
		$query_manager->addQueryLineArr(
			array(
				"INSERT INTO `ACTION_LOG`(`time_start`,`os`,`browser`,`ip`,`type`,`user_id`,`msg`)"
				,"VALUES(CURRENT_TIMESTAMP,\"$CLIENT_OS\",\"$CLIENT_BROWSER\",\"$CLIENT_IP\",\"$ACTION_TYPE\",$USER_ID,\"$msg_url_encode\")"
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}

	public function select_action_log_total_cnt($param=null) {

		$TARGET_USER_ID = -1;
		if(isset($param->{$this->var_name->TARGET_USER_ID})) {
			$TARGET_USER_ID = intval($param->{$this->var_name->TARGET_USER_ID});
		}

		$platform_id = "";
		if(Checker::is_natural_number($TARGET_USER_ID)){

			$param->{$this->var_name->USER_ID} = $TARGET_USER_ID;
			$user_info_obj = $this->select_user_by_id($param);
			if(!is_null($user_info_obj)) {
				
				if(!empty($user_info_obj->__fb_id)) {
					$platform_id = $user_info_obj->__fb_id;
				} else if(!empty($user_info_obj->__google_id)) {
					$platform_id = $user_info_obj->__google_id;
				}   // end inner if

			}  // end inner if

		} // end outer if		

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT COUNT(`ACTION_LOG`.`id`) AS __cnt"
				,"FROM `ACTION_LOG`"
			)
		);

		if(!empty($platform_id)) {
			$query_manager->addQueryLineArr(
				array(
					"WHERE msg LIKE \"%$platform_id%\""
				)
			);
		}

		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}		

		$result = $query_manager->get_result();		

		return intval($result->__cnt);
	}
	
  	public function select_action_log($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->PAGE_NUM
				, $this->var_name->ROW_CNT
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}
		$PAGE_NUM = intval($param->{$this->var_name->PAGE_NUM});
		$ROW_CNT = intval($param->{$this->var_name->ROW_CNT});
		$TARGET_USER_ID = -1;
		if(isset($param->{$this->var_name->TARGET_USER_ID})) {
			$TARGET_USER_ID = intval($param->{$this->var_name->TARGET_USER_ID});	
		}
		
		// CHECK PARAM VALIDATION - DONE
		
		$platform_id = "";
		if(Checker::is_natural_number($TARGET_USER_ID)){

			$param->{$this->var_name->USER_ID} = $TARGET_USER_ID;
			$user_info_obj = $this->select_user_by_id($param);
			if(!is_null($user_info_obj)) {
				
				if(!empty($user_info_obj->__fb_id)) {
					$platform_id = $user_info_obj->__fb_id;
				} else if(!empty($user_info_obj->__google_id)) {
					$platform_id = $user_info_obj->__google_id;
				}   // end inner if

			}  // end inner if

		} // end outer if		

		// page begins from 1
		$row_num = $ROW_CNT;
		$offset = ($PAGE_NUM - 1) * $row_num;

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `ACTION_LOG`.`id` AS __id"
				,",CONCAT(`USER`.first_name,\" \",`USER`.last_name) AS __user_name"
				,",`ACTION_LOG`.`user_id` AS __user_id"
				,",`ACTION_LOG`.`time_start` AS __time_start"
				,",`ACTION_LOG`.`os` AS __os"
				,",`ACTION_LOG`.`browser` AS __browser"
				,",`ACTION_LOG`.`ip` AS __ip"
			    ,",`ACTION_LOG`.`type` AS __type"
				,",`ACTION_LOG`.`msg` AS __msg"
				,"FROM `ACTION_LOG`"
				,"LEFT JOIN `USER` ON `USER`.id=`ACTION_LOG`.`user_id`"
			)
		);

		if(!empty($platform_id)) {
			$query_manager->addQueryLineArr(
				array(
					"WHERE msg LIKE \"%$platform_id%\""
				)
			);
		}

		$query_manager->addQueryLineArr(
			array(
				"ORDER BY `ACTION_LOG`.`id` DESC"
				,"LIMIT $offset, $row_num"
			)
		);

		$query_manager->select();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();		

		for($idx=0; $idx < count($result); $idx++) {
			$log = $result[$idx];
			$log->__msg = urldecode($log->__msg);
		}

		return $result;
	}

	public function insert_error_log($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->CLIENT_OS
				, $this->var_name->CLIENT_IP
				, $this->var_name->CLIENT_BROWSER
				, $this->var_name->ERROR_TYPE
				, $this->var_name->ERROR_MSG
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}
		
		$CLIENT_OS = $param->{$this->var_name->CLIENT_OS};
		$CLIENT_IP = $param->{$this->var_name->CLIENT_IP};
		$CLIENT_BROWSER = $param->{$this->var_name->CLIENT_BROWSER};
		$ERROR_TYPE = $param->{$this->var_name->ERROR_TYPE};
		$ERROR_MSG = $param->{$this->var_name->ERROR_MSG};
		// CHECK PARAM VALIDATION - DONE		

		$msg_url_encode = urlencode($ERROR_MSG);

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->off_feedback_manager();
		$query_manager->set_check_slow_query(false);
		$query_manager->addQueryLineArr(
			array(
				"INSERT INTO `ERROR_LOG`(`time_start`,`os`,`browser`,`ip`,`type`,`msg`)"
				,"VALUES(CURRENT_TIMESTAMP,\"$CLIENT_OS\",\"$CLIENT_BROWSER\",\"$CLIENT_IP\",\"$ERROR_TYPE\",\"$msg_url_encode\")"
			)
		);
		$query_manager->modify();
		if($query_manager->is_failed()) {
			return;
		}

	}	

	public function select_error_log_total_cnt($param=null) {

		$ERROR_TYPE = "";
		if(isset($param->{$this->var_name->ERROR_TYPE})) {
			$ERROR_TYPE = $param->{$this->var_name->ERROR_TYPE};	
		}

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT COUNT(`ERROR_LOG`.`id`) AS __cnt"
				,"FROM `ERROR_LOG`"
			)
		);
		if(!empty($ERROR_TYPE)) {
			$query_manager->addQueryLineArr(
				array(
					"WHERE type=\"$ERROR_TYPE\""
				)
			);
		}
		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}		

		$result = $query_manager->get_result();		

		return intval($result->__cnt);
	}

	public function select_error_log($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->PAGE_NUM
				, $this->var_name->ROW_CNT
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}
		$PAGE_NUM = intval($param->{$this->var_name->PAGE_NUM});
		$ROW_CNT = intval($param->{$this->var_name->ROW_CNT});

		$ERROR_TYPE = "";
		if(isset($param->{$this->var_name->ERROR_TYPE})) {
			$ERROR_TYPE = $param->{$this->var_name->ERROR_TYPE};	
		}
		// CHECK PARAM VALIDATION - DONE

		// page begins from 1
		$row_num = $ROW_CNT;
		$offset = ($PAGE_NUM - 1) * $row_num;

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT `ERROR_LOG`.`id` AS __id,"
				,"`ERROR_LOG`.`time_start` AS __time_start,"
				,"`ERROR_LOG`.`os` AS __os,"
				,"`ERROR_LOG`.`browser` AS __browser,"
				,"`ERROR_LOG`.`ip` AS __ip,"
				,"`ERROR_LOG`.`type` AS __type,"
				,"`ERROR_LOG`.`msg` AS __msg"
				,"FROM `ERROR_LOG`"
			)
		);
		if(!empty($ERROR_TYPE)) {
			$query_manager->addQueryLineArr(
				array(
					"WHERE type=\"$ERROR_TYPE\""
				)
			);
		}
		$query_manager->addQueryLineArr(
			array(
				"ORDER BY `ERROR_LOG`.`id` DESC"
				,"LIMIT $offset, $row_num"
			)
		);

		$query_manager->select();
		if($query_manager->is_failed()) {
			return;
		}
		
		$result = $query_manager->get_result();

		for($idx=0; $idx < count($result); $idx++) {
			$log = $result[$idx];
			$log->__msg = urldecode($log->__msg);
		}

		return $result;
	}

	public function select_next_id($param=null) {

		// CHECK PARAM VALIDATION - INIT
		$is_not_valid = 
		$this->param_manager->is_not_valid(
			// $param_std=null
			$param
			// $key_arr=null
			, array(
				$this->var_name->QUERY_TABLE_NAME
			)
			// $feedback_manager=null
			, $this->feedback_manager
			// $scope=null
			, __FUNCTION__
		);
		if($is_not_valid) {
			return;
		}

		$QUERY_TABLE_NAME = $param->{$this->var_name->QUERY_TABLE_NAME};
		// CHECK PARAM VALIDATION - DONE

		$query_manager = $this->get_query_manager(__FUNCTION__);
		$query_manager->addQueryLineArr(
			array(
				"SELECT AUTO_INCREMENT AS __next_id FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = '$QUERY_TABLE_NAME'"
			)
		);

		$query_manager->select_one();
		if($query_manager->is_failed()) {
			return;
		}

		$result = $query_manager->get_result();

		return intval($result->__next_id);	
	}	

}

?>
