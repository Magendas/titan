<?php

class TitanLinkManager{

	private static $SERVICE_ROOT_PATH="";
	public static function set_service_root_path($service_root_path=""){
		self::$SERVICE_ROOT_PATH = $service_root_path;
	}	
	public static function get_service_root_path(){
		return self::$SERVICE_ROOT_PATH;
	}

	private static $FILE_ROOT_PATH="";
	public static function set_file_root_path($file_root_path=""){
		self::$FILE_ROOT_PATH = $file_root_path;
	}	
	public static function get_file_root_path(){
		return self::$FILE_ROOT_PATH;
	}

	public static $LOG_IN="/view/log_in.php";
	public static $LOG_OUT="/view/log_out.php";

	// 개인정보보호 & 이용약관
	public static $TOS="/view/tos.html";
	public static $POS="/view/privacy.html";

	public static $ADMIN_LOG_IN="/view/sim/admin_log_in.php";
	public static $ADMIN_LOG_OUT="/view/sim/admin_log_out.php";	

	public static $ADMIN_QUIZ_HOME="/view/sim/quiz_home.php";
	public static $ADMIN_QUIZ_SIMULATOR="/view/sim/quiz_simulator.php";
	public static $ADMIN_QUIZ_SIMULATOR_MOBILE="/view/sim/quiz_simulator_mobile.php";

	public static function go($type="", $param_arr=null) {
		$link = self::get($type, $param_arr);
		header("Location:$link");
	}
	public static function get($type="", $param_arr=null) {
		$link = self::get_service_root_path() . $type;
		if(is_null($param_arr)){
			return $link;
		}
		return self::get_link_with_param($link, $param_arr);
	}
	private static function get_link_with_param($link="", $param_arr=null) {
		$idx = 0;
		if(is_null($param_arr)) {
			return $link;
		}

		foreach ($param_arr as $key => $value) {
			if($idx == 0) {
				$link .= "?";
			}
			$link .= $key . "=" . $value . "&";
			$idx++;
		}

		return $link;
	}

}

?>
