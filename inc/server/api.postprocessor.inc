<?php

class APIPostProcessor{
	
	private $mysql_interface;
	private $scope="";
	public function APIPostProcessor($mysql_interface=null, $scope="") {

		if(is_null($mysql_interface)) {
			echo "!Error! / APIPostProcessor / is_null($mysql_interface)<br/>";
			exit();
		}
		if(empty($scope)) {
			echo "!Error! / APIPostProcessor / empty(\$scope)<br/>";
			exit();
		}

		$this->mysql_interface = $mysql_interface;
		$this->scope = $scope;

	}
	public function error($reason="", $extra_data=null) {

		if(empty($reason)) {
			echo "!Error! / APIPostProcessor / error / empty(\$reason)<br/>";
			exit();
		}

		$this->finish(
			false
			, $reason
			, $extra_data
		);
	}
	public function ok($reason="", $extra_data=null) {

		if(empty($reason)) {
			echo "!Error! / APIPostProcessor / ok / empty(\$reason)<br/>";
			exit();
		}

		$this->finish(
			true
			, $reason
			, $extra_data
		);
	}
	public function pin($reason="", $extra_data=null){

		$mysql_interface = $this->mysql_interface;
		if(is_null($mysql_interface)) {
			echo "!Error! / APIPostProcessor / pin / is_null(\$mysql_interface)<br/>";
			exit();
		}
		$scope = $this->scope;
		if(empty($scope)) {
			echo "!Error! / APIPostProcessor / pin / empty(\$scope)<br/>";
			exit();
		}
		if(empty($reason)) {
			echo "!Error! / APIPostProcessor / pin / empty(\$reason)<br/>";
			exit();
		}
		$feedback_manager = $mysql_interface->get_mysql_feedback();
		if(is_null($feedback_manager)) {
			echo "!Error! / APIPostProcessor / pin / is_null(\$feedback_manager)<br/>";
			exit();
		}

		$feedback_manager->add_pin(
			// $scope_name=""
			$scope
			// $reason=""
			, $reason
			// $extra_data=null
			, $extra_data
		);

	}
	private function finish($success=false, $reason="", $extra_data=null){
		
		$mysql_interface = $this->mysql_interface;
		if(is_null($mysql_interface)) {
			echo "!Error! / APIPostProcessor / finish / is_null(\$mysql_interface)<br/>";
			exit();
		}
		$scope = $this->scope;
		if(empty($scope)) {
			echo "!Error! / APIPostProcessor / finish / empty(\$scope)<br/>";
			exit();
		}
		if(empty($reason)) {
			echo "!Error! / APIPostProcessor / finish / empty(\$reason)<br/>";
			exit();
		}
		$feedback_manager = $mysql_interface->get_mysql_feedback();
		if(is_null($feedback_manager)) {
			echo "!Error! / APIPostProcessor / finish / is_null(\$feedback_manager)<br/>";
			exit();
		}

		if($success) {

			$feedback_manager->add_ok(
				// $scope_name=""
				$scope
				// $reason=""
				, $reason
				// $extra_data=null
				, $extra_data
			);

		} else {

			$feedback_manager->add_error(
				// $scope_name=""
				$scope
				// $reason=""
				, $reason
				// $extra_data=null
				, $extra_data
			);

		}

		// CLOSE DB
		$mysql_interface->close();

		// GET FEECBACK AND RETURN IT
		$feedback = $feedback_manager->get();

		echo json_encode($feedback);

		exit();
	}

}

?>
